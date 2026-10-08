//
// Created by bvasseur on 10/5/26.
//

#include "platform/Audio.hpp"

#include <algorithm>
#include <stdexcept>
#include <SDL2/SDL_mixer.h>



Audio	*Audio::_instance = nullptr;
double	Audio::jukeboxMusicFadeTime = 5.;

std::unordered_map<std::string, Mix_Chunk*>	Audio::_sounds	= {};
std::unordered_map<std::string, Mix_Music*>	Audio::_musics	= {};
std::unordered_map<std::string, disc>		Audio::_discs	= {};
std::vector<std::string>					Audio::_playlist= {};


/* ==================== CONSTRUCTORS ==================== */


Audio::Audio() {
	if (_instance)
		return ;
	_instance = this;
	init();
	loadFiles();
}

Audio::~Audio() {
	_checkInstance();

	if(Mix_PlayingMusic())
		Mix_HaltMusic();
	for (auto& sound: _sounds)
		Mix_FreeChunk(sound.second);
	_sounds.clear();

	for (auto& music: _musics)
		Mix_FreeMusic(music.second);
	_musics.clear();

	for (auto& disc: _discs) {
		Mix_FreeChunk(disc.second.chunk);
	}
	_discs.clear();

	Mix_CloseAudio();	_instance = nullptr;
	PRINT BOLD "Audio Destroyed" CENDL;
}


/* ==================== METHODS ==================== */


void Audio::init() {
	_checkInstance();

	if (SDL_InitSubSystem(SDL_INIT_AUDIO) != 0)
		throw std::runtime_error(std::string("SDL_Init: ") + SDL_GetError());
	if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1 )
		throw std::runtime_error("OpenAudio loading failed");
}

void Audio::loadFiles() {
	_checkInstance();

	try {
		_loadSound("assets/sounds/UI/click_0.wav", "click");

		_loadMusic("assets/sounds/music/theme.wav", "theme");
	} catch (std::exception& e) {
		PRERR e.what() ENDL;
	}
}

void Audio::playSound(const std::string& name) {
	_checkInstance();
	if (_sounds.find(name) == _sounds.end())
		throw std::runtime_error("Sound: `" + name + "' doesn't exist");

	if(Mix_PlayChannel( -1, _sounds[name], 0 ) == -1)
		throw std::runtime_error("Sound: `" + name + "' couldn't play");
}

static Sint16 getAngle(const vec3f& facing, const vec3f& dist) {
	float fx = facing.x(),	fz = facing.z();
	float rx = -fz,				rz = fx;

	float front = dist.x() * fx + dist.z() * fz;   // how far in front
	float right = dist.x() * rx + dist.z() * rz;   // how far to the right

	float deg = std::atan2(right, front) * 180.0f / M_PI;   // -180..180
	if (deg < 0) deg += 360.0f;
	return (static_cast<Sint16>(deg));
}

void Audio::playSoundFrom(const std::string& name, const vec3f& playerFront, const vec3f& playerPos,
	const vec3f& worldPos, float maxHearingDist) {
	_checkInstance();
	if (_sounds.find(name) == _sounds.end())
		throw std::runtime_error("Sound: `" + name + "' doesn't exist");
	vec3f dist = worldPos - playerPos;
	float length = dist.length();
	if (length >= maxHearingDist)
		return ;

	int chan = Mix_PlayChannel( -1, _sounds[name], 0 );
	if (chan == -1) throw std::runtime_error("Sound: `" + name + "' couldn't play");
	if (Mix_SetPosition(chan, getAngle(playerFront, dist),  length / maxHearingDist * 255.) == 0)
		throw std::runtime_error("Sound: `" + name + "' error in positioning");
}

void Audio::playDiscFrom(const std::string& fileName, const std::string& name, const vec3f& worldPos, float maxHearingDist) {
	_checkInstance();

	if (_discs.find(name) == _discs.end())
		_loadDisc(fileName, name);

	disc& disc = _discs[name];
	int chan = Mix_PlayChannel( -1, disc.chunk, 0 );
	if (chan == -1) throw std::runtime_error("Sound: `" + name + "' couldn't play");
	disc.actives.push_back({ chan, worldPos, maxHearingDist });
}


void Audio::playMusic(const std::string& name) {
	_checkInstance();
	if (_musics.find(name) == _musics.end())
		throw std::runtime_error("Music: `" + name + "' doesn't exist");

	if(Mix_PlayMusic( _musics[name], -1 ) == -1)
		throw std::runtime_error("Music: `" + name + "' couldn't play");
}

void Audio::continuePlaylist() {
	_checkInstance();
	if (_playlist.empty())
		throw std::runtime_error("Music: no musics in playlist");

	if(Mix_PlayingMusic() == 0) {
		if( Mix_PlayMusic( _musics[_playlist[0]], -1 ) == -1 )
			throw std::runtime_error("Music: `" + _playlist[0] + "' couldn't play");
		_playlist.erase(_playlist.begin());
	}
}

void Audio::update_discs(const vec3f& playerFront, const vec3f& playerPos, const double& deltaTime) {
	bool jukeboxNearby = false;

	for (auto it = _discs.begin(); it != _discs.end(); ) {
		disc& d = it->second;

		std::erase_if(d.actives, [&](const discInfos& a) {
			// finished (or channel reused by something else) -> drop it
			if (!Mix_Playing(a.chan) || Mix_GetChunk(a.chan) != d.chunk) {
				PRINT "Removing a " << it->first ENDL;
				return true;
			}

			vec3f v = a.pos - playerPos;
			float ratio = std::min(v.length() / a.maxDist, 1.0f);
			if (ratio != 1.f)
				jukeboxNearby = true;

			Mix_SetPosition(a.chan, static_cast<Sint16>(getAngle(playerFront, v)),
				static_cast<Uint8>(ratio * 255.0f));
			return false;
		});

		if (d.actives.empty()) {
			PRINT "All " << it->first << " are done playing, removing them" ENDL;
			Mix_FreeChunk(d.chunk);
			it = _discs.erase(it);
		} else
			++it;
	}

	static double cur = jukeboxMusicFadeTime;
	if ((!jukeboxNearby && cur >= jukeboxMusicFadeTime) || (jukeboxNearby && cur <= 0))
		return;
	cur = std::clamp(cur + (jukeboxNearby ? -deltaTime : deltaTime), 0., jukeboxMusicFadeTime);
	Mix_VolumeMusic(static_cast<int>(MIX_MAX_VOLUME * (cur / jukeboxMusicFadeTime)));
}
/* ==================== PRIVATE METHODS ==================== */

Mix_Chunk* Audio::_loadWAV(const std::string& fileName, const std::string& name, const std::string& typeName) {
	Mix_Chunk* sound = Mix_LoadWAV(fileName.c_str());
	if (sound == nullptr)
		throw std::runtime_error(typeName + ": `" + name + "' with path `" + fileName + "' couldn't be loaded");
	return sound;
}

void Audio::_loadSound(const std::string& fileName, const std::string& name) {
	if (_sounds.find(name) != _sounds.end())
		throw std::runtime_error("Sound: `" + name + "' already exist");
	_sounds.emplace(name, _loadWAV(fileName, name, "Sound"));
}

void Audio::_loadMusic(const std::string& fileName, const std::string& name) {
	if (_musics.find(name) != _musics.end())
		throw std::runtime_error("Music: `" + name + "' already exist");

	Mix_Music* music = Mix_LoadMUS(fileName.c_str());
	if (music == nullptr)
		throw std::runtime_error("Music: `" + name + "' with path `" + fileName + "' couldn't be loaded");
	_musics.emplace(name, music);
}

void Audio::_loadDisc(const std::string& fileName, const std::string& name) {
	if (_discs.find(name) != _discs.end())
		throw std::runtime_error("Disc: `" + name + "' already exist");
	_discs[name] = {.chunk = _loadWAV(fileName, name, "Disc")};
}

void Audio::_checkInstance() {
	if (!_instance)
		throw (std::runtime_error(RED BOLD UNDL "Audio instance not created" CLR));
}

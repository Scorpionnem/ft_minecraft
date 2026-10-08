//
// Created by bvasseur on 10/5/26.
//

#include "platform/Audio.hpp"
#include "app/GameOptions.hpp"
#include <SDL2/SDL_mixer.h>

#include "../../inc/app/GameOptions.hpp"


GameOptions	*Audio::_opts = nullptr;
double		Audio::jukeboxMusicFadeTime = 5.;

std::unordered_map<std::string, Mix_Chunk*>	Audio::_sounds		= {};
std::unordered_map<std::string, Mix_Music*>	Audio::_musics		= {};
std::unordered_map<std::string, disc>		Audio::_discs		= {};
std::vector<std::string>					Audio::_playlist	= {};
int											Audio::_masterVolume	= MIX_MAX_VOLUME;


/* ==================== CONSTRUCTORS ==================== */


Audio::Audio() {
}

Audio::~Audio() {
}


/* ==================== METHODS ==================== */


void Audio::init(GameOptions& opts) {
	_opts = &opts;
	if (SDL_InitSubSystem(SDL_INIT_AUDIO) != 0)
		throw std::runtime_error(std::string("SDL_Init: ") + SDL_GetError());
	if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1 )
		throw std::runtime_error("OpenAudio loading failed");
	loadFiles();

}

void Audio::cleanup() {
	if(Mix_PlayingMusic())
		Mix_HaltMusic();
	for (auto& [name, sound]: _sounds)
		Mix_FreeChunk(sound);
	_sounds.clear();

	for (auto& [name, music]: _musics)
		Mix_FreeMusic(music);
	_musics.clear();

	for (auto& [name, disc]: _discs) {
		for (auto& a: disc.actives)
			Mix_HaltChannel(a.chan);
		Mix_FreeChunk(disc.chunk);
	}
	_discs.clear();

	Mix_CloseAudio();
	PRINT BOLD "Audio Destroyed" CENDL;
}

void Audio::loadFiles() {
	try {
		_loadSound("assets/sounds/UI/click_0.wav", "click");

		_loadMusic("assets/sounds/music/theme.wav", "theme");
	} catch (std::exception& e) {
		PRERR e.what() ENDL;
	}
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

void Audio::playSound(const std::string& name) {
	if (_sounds.find(name) == _sounds.end())
		throw std::runtime_error("Sound: `" + name + "' doesn't exist");

	int chan = Mix_PlayChannel( -1, _sounds[name], 0 );
	if(chan == -1) throw std::runtime_error("Sound: `" + name + "' couldn't play");
	Mix_Volume(chan, _masterVolume * (_opts->ambient_volume / 100.));
}

void Audio::playSoundFrom(const std::string& name, const vec3f& playerFront, const vec3f& playerPos,
	const vec3f& worldPos, float maxHearingDist) {
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
	Mix_Volume(chan, _masterVolume * (_opts->ambient_volume / 100.));
}

void Audio::playDiscFrom(const std::string& fileName, const std::string& name, const vec3f& worldPos, float maxHearingDist) {
	if (_discs.find(name) == _discs.end())
		_loadDisc(fileName, name);

	disc& disc = _discs[name];
	int chan = Mix_PlayChannel( -1, disc.chunk, 0 );
	if (chan == -1) throw std::runtime_error("Sound: `" + name + "' couldn't play");
	disc.actives.push_back({ chan, worldPos, maxHearingDist });
}


void Audio::playMusic(const std::string& name) {
	if (_musics.find(name) == _musics.end())
		throw std::runtime_error("Music: `" + name + "' doesn't exist");

	if(Mix_PlayMusic( _musics[name], -1 ) == -1)
		throw std::runtime_error("Music: `" + name + "' couldn't play");
}

void Audio::continuePlaylist() {
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

			Mix_Volume(a.chan, _masterVolume * (_opts->discs_volume / 100.));

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
	// if ((!jukeboxNearby && cur >= jukeboxMusicFadeTime) || (jukeboxNearby && cur <= 0))
		// return;
	cur = std::clamp(cur + (jukeboxNearby ? -deltaTime : deltaTime), 0., jukeboxMusicFadeTime);
	Mix_VolumeMusic(static_cast<int>((_masterVolume * (_opts->music_volume / 100.)) * (cur / jukeboxMusicFadeTime)));
}

void Audio::setMasterVolume() {
	_masterVolume = MIX_MAX_VOLUME * (std::clamp(_opts->master_volume, 0, 100) / 100.);
	setMusicVolume();
}

void Audio::setMusicVolume() {
	Mix_VolumeMusic(_masterVolume * (std::clamp(_opts->music_volume, 0, 100) / 100.));
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

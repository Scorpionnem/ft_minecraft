//
// Created by bvasseur on 10/5/26.
//

#include "platform/Audio.hpp"

#include <stdexcept>
#include <SDL2/SDL_mixer.h>


Audio	*Audio::_instance = nullptr;
std::unordered_map<std::string, Mix_Chunk*>	Audio::_sounds	= {};
std::unordered_map<std::string, Mix_Music*>	Audio::_musics	= {};
std::unordered_map<std::string, Disc>		Audio::_discs	= {};
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

		_loadDisc("assets/sounds/music/jazz_theme.wav", "disc");
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

void Audio::playDiscFrom(const std::string& name, const vec3f& playerFront, const vec3f& playerPos, const vec3f& worldPos, float maxHearingDist) {
	_checkInstance();
	if (_discs.find(name) == _discs.end())
		throw std::runtime_error("Disc: `" + name + "' doesn't exist");

	Disc& disc = _discs[name];
	int chan = Mix_PlayChannel( -1, disc.chunk, 0 );
	if (chan == -1) throw std::runtime_error("Sound: `" + name + "' couldn't play");
	disc.chans.push_back(chan);
	disc.poses.push_back(worldPos);
	disc.maxDists.push_back(maxHearingDist);

	update_discs(playerFront, playerPos);
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

void Audio::update_discs(const vec3f &playerFront, const vec3f &playerPos) {
	for (auto& it : _discs) {
		Disc& disc = it.second;
		for (size_t i = 0; i < disc.chans.size(); i++) {
			vec3f dist = disc.poses[i] - playerPos;
			float length = dist.length();
			if (length >= disc.maxDists[i])
				length = disc.maxDists[i];
			Mix_SetPosition(disc.chans[i], getAngle(playerFront, dist),  length / disc.maxDists[i] * 255.);
			// TODO : remove from disc when over
			// TODO : remove mute music when in jukebox
		}
	}
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
	Disc disc;
	disc.chunk = _loadWAV(fileName, name, "Disc");
	_discs.emplace(name, disc);
}

void Audio::_checkInstance() {
	if (!_instance)
		throw (std::runtime_error(RED BOLD UNDL "Audio instance not created" CLR));
}

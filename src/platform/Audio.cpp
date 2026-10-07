//
// Created by bvasseur on 10/5/26.
//

#include "platform/Audio.hpp"


Audio	*Audio::_instance = nullptr;
std::unordered_map<std::string, Mix_Music*>	Audio::_musics = {};
std::unordered_map<std::string, Mix_Chunk*>	Audio::_sounds = {};
std::vector<std::string>					Audio::_playlist = {};


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

void Audio::playMusic(const std::string& name) {
	_checkInstance();
	if (_musics.find(name) == _musics.end())
		throw std::runtime_error("Music: `" + name + "' doesn't exist");

	if(Mix_PlayingMusic())
		Mix_HaltMusic();
	if( Mix_PlayMusic( _musics[name], -1 ) == -1 )
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


/* ==================== PRIVATE METHODS ==================== */


void Audio::_loadSound(const std::string& fileName, const std::string& name) {
	if (_sounds.find(name) != _sounds.end())
		throw std::runtime_error("Sound: `" + name + "' already exist");

	Mix_Chunk* sound = Mix_LoadWAV(fileName.c_str());
	if (sound == nullptr)
		throw std::runtime_error("Sound: `" + name + "' with path `" + fileName + "' couldn't be loaded");
	_sounds.emplace(name, sound);
}

void Audio::_loadMusic(const std::string& fileName, const std::string& name) {
	if (_musics.find(name) != _musics.end())
		throw std::runtime_error("Music: `" + name + "' already exist");

	Mix_Music* music = Mix_LoadMUS(fileName.c_str());
	if (music == nullptr)
		throw std::runtime_error("Music: `" + name + "' with path `" + fileName + "' couldn't be loaded");
	_musics.emplace(name, music);
}

void Audio::_checkInstance() {
	if (!_instance)
		throw (std::runtime_error(RED BOLD UNDL "Audio instance not created" CLR));
}

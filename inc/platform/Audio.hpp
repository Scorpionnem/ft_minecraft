//
// Created by bvasseur on 10/5/26.
//

#ifndef MINECRAFT_AUDIO_HPP
# define MINECRAFT_AUDIO_HPP

# include <SDL2/SDL.h>
# include <string>
# include <vector>
# include <stdexcept>
# include <unordered_map>
# include <iostream>
# include <ostream>
# include <stdexcept>
# include "SDL2/SDL_mixer.h"

# include "colors.hpp"

class Audio {
public:
	static Audio* _instance;

	Audio();
	~Audio();

	static void init();
	static void loadFiles();
	static void playSound(const std::string& name);
	static void playMusic(const std::string& name);
	static void continuePlaylist();

private:
	static void _loadSound(const std::string& fileName, const std::string& name);
	static void _loadMusic(const std::string& fileName, const std::string& name);

	static void	_checkInstance();


	static std::unordered_map<std::string, Mix_Chunk*>	_sounds;
	static std::unordered_map<std::string, Mix_Music*>	_musics;
	static std::vector<std::string>						_playlist;

};

#endif //MINECRAFT_AUDIO_HPP
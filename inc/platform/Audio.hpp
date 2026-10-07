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
# include "math/vec.hpp"

# include "colors.hpp"

struct Disc {
	Mix_Chunk*			chunk;
	std::vector<int>	chans;
	std::vector<vec3f>	poses;
	std::vector<float>	maxDists;
};

class Audio {
public:
	static Audio* _instance;

	Audio();
	~Audio();

	static void init();
	static void loadFiles();
	static void playSound(const std::string& name);
	static void playSoundFrom(const std::string& name, const vec3f& playerFront, const vec3f& playerPos, const vec3f& worldPos, float maxHearingDist);
	static void playDiscFrom(const std::string& name, const vec3f& playerFront, const vec3f& playerPos, const vec3f& worldPos, float maxHearingDist);
	static void playMusic(const std::string& name);
	static void continuePlaylist();

	static void update_discs(const vec3f& playerFront, const vec3f& playerPos);

private:
	static Mix_Chunk*	_loadWAV(const std::string& fileName, const std::string& name, const std::string& typeName);
	static void			_loadSound(const std::string& fileName, const std::string& name);
	static void			_loadMusic(const std::string& fileName, const std::string& name);
	static void			_loadDisc(const std::string& fileName, const std::string& name);

	static void	_checkInstance();


	static std::unordered_map<std::string, Mix_Chunk*>	_sounds;
	static std::unordered_map<std::string, Mix_Music*>	_musics;
	static std::unordered_map<std::string, Disc>		_discs;
	static std::vector<std::string>						_playlist;

};

#endif //MINECRAFT_AUDIO_HPP

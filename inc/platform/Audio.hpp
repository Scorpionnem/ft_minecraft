//
// Created by bvasseur on 10/5/26.
//

#ifndef MINECRAFT_AUDIO_HPP
# define MINECRAFT_AUDIO_HPP

struct GameOptions;

# include <SDL2/SDL.h>
# include <string>
# include <vector>
# include <stdexcept>
# include <unordered_map>
# include <iostream>
# include <ostream>
# include <stdexcept>
# include <algorithm>
# include <SDL2/SDL_mixer.h>
# include "math/vec.hpp"
# include "colors.hpp"

struct discInfos {
	int		chan;
	vec3f	pos;
	float	maxDist;
};

struct disc {
	Mix_Chunk*				chunk;
	std::vector<discInfos>	actives;
};

class Audio {
public:
	Audio();
	~Audio();

	static void init(GameOptions& opts);
	static void cleanup();
	static void loadFiles();
	static void playSound(const std::string& name);
	static void playSoundFrom(const std::string& name, const vec3f& playerFront, const vec3f& playerPos, const vec3f& worldPos, float maxHearingDist);
	static void playDiscFrom(const std::string& FileName, const std::string& name, const vec3f& worldPos, float maxHearingDist);
	static void playMusic(const std::string& name);
	static void continuePlaylist();

	static void update_discs(const vec3f& playerFront, const vec3f& playerPos, const double& deltaTime);

	static void setMasterVolume();
	static void setMusicVolume();
	static void updateMusicVolume();

	static double jukeboxMusicFadeTime;

private:
	static Mix_Chunk*	_loadWAV(const std::string& fileName, const std::string& name, const std::string& typeName);
	static void			_loadSound(const std::string& fileName, const std::string& name);
	static void			_loadMusic(const std::string& fileName, const std::string& name);
	static void			_loadDisc(const std::string& fileName, const std::string& name);

	static void			_checkInstance();


	static int											_masterVolume;
	static std::unordered_map<std::string, Mix_Chunk*>	_sounds;
	static std::unordered_map<std::string, Mix_Music*>	_musics;
	static std::unordered_map<std::string, disc>		_discs;
	static std::vector<std::string>						_playlist;
	static GameOptions*									_opts; // ptr to client.opts()


};

#endif //MINECRAFT_AUDIO_HPP

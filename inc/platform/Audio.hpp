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
# include <atomic>
# include <SDL2/SDL_mixer.h>

# include "math/vec.hpp"
# include "platform/AudioShortcuts.hpp"
# include "platform/AudioEffects.hpp"
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
	Audio() {}
	~Audio() {}

	static void init(GameOptions& opts);
	static void cleanup();
	static void playSound(const std::string& name);
	static void playSoundFrom(const std::string& name, const vec3f& playerFront, const vec3f& playerPos, const vec3f& worldPos, float maxHearingDist);
	static void playDiscFrom(const std::string& name, const vec3f& worldPos, float maxHearingDist);
	static void playMusic(const std::string& name);
	static void addToPlaylist(const std::string& name);

	static void update(const vec3f& playerFront, const vec3f& playerPos, const double& deltaTime);

	static void setDelay(const bool on)					{ AudioEffects::setDelay(on); }
	static void setReverb(const bool on)				{ AudioEffects::setReverb(on); }
	static void setUnderwater(const bool isUnderwater)	{ AudioEffects::setUnderwater(isUnderwater); }
	static void setMasterVolume();
	static void setMusicVolume();
	static void updateMusicVolume();

	static double jukeboxMusicFadeTime;

private:
	static void _initUnderwaterFilter();
	static void _muffleEffect(int, void* stream, int len, void*);
	static void			_loadFiles();
	static Mix_Chunk*	_loadWAV(const std::pair<std::string, std::string>& shortcut, const std::string& typeName);
	static void			_loadSound(const std::pair<std::string, std::string>& shortcut);
	static void			_loadMusic(const std::pair<std::string, std::string>& shortcut);
	static void			_loadDisc(const std::pair<std::string, std::string>& shortcut);

	static void			_onMusicFinished() { _musicFinished = true; }
	static void			_continuePlaylist();
	static void			_update_discs(const vec3f& playerFront, const vec3f& playerPos, const double& deltaTime);


	static int											_masterVolume;
	static std::unordered_map<std::string, Mix_Chunk*>	_sounds;
	static std::pair<std::string, Mix_Music*>			_music;
	static std::atomic<bool>							_musicFinished; // written by audio thread
	static std::unordered_map<std::string, disc>		_discs;
	static std::vector<std::string>						_playlist;
	static GameOptions*									_opts; // ptr to client.opts()

};

#endif //MINECRAFT_AUDIO_HPP

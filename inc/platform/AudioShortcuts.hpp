//
// Created by bvasseur on 10/9/26.
//

#ifndef MINECRAFT_AUDIOSHORTCUTS_HPP
# define MINECRAFT_AUDIOSHORTCUTS_HPP

# include <string>
# include <vector>
# include <unordered_map>

// name, pathToFile
inline static std::unordered_map<std::string, std::string> shortcuts = {
	{"click", "assets/sounds/UI/click_0.wav"},

	{"theme", "assets/sounds/music/theme.wav"},

	{"disc", "assets/sounds/music/jazz_theme.wav"},
};

/*
 * Returns the path for a given file
 *
 * \param name, the (arbitrary) name of the sound/music/disc you want to get. Look at platform/AudioShortcuts.hpp to see what name is associated with what shortcut.
 * \returns a pair containing name, pathToFile.
 */
inline std::pair<std::string, std::string> getPathShortcut(const std::string& name) {
	return { name, shortcuts[name] };
}


#endif //MINECRAFT_AUDIOSHORTCUTS_HPP
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Game.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 12:25:27 by mbatty            #+#    #+#             */
/*   Updated: 2025/10/05 11:33:49 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GAME_HPP
# define GAME_HPP

# include "libs.hpp"
# include "Window.hpp"
# include "Input.hpp"
# include "Camera.hpp"
# include "TextureManager.hpp"
# include "ShaderManager.hpp"
# include "UIElement.hpp"
# include "Scene.hpp"

# define SERVER_LIST_EXPORT_FILE "saves/server_list.txt"

struct	ServerInfo
{
	ServerInfo(const std::string &name, const std::string &ip)
	{
		this->name = name;
		this->ip = ip;
	}
	ServerInfo()
	{
		name = "";
		ip = "";
	}
	std::string	name;
	std::string	ip;
};

class	Game
{
	public:
		Game();
		~Game();

		void	run();

		Window	&getWindow() {return (this->_window);}
		Input	&getInput() {return (this->_input);}
		TextureManager	&getTextures() {return (this->_textures);}
		ShaderManager	&getShaders() {return (this->_shaders);}

		void	setRunning(bool state)
		{
			_running = state;
		}
		
		void	deleteServer(ServerInfo *info);
		void	exportServerList();
		void	importServerList();
		
		std::vector<ServerInfo>	&getServerInfos() {return (_serverInfos);}
		void	addServerInfo(ServerInfo info) {_serverInfos.push_back(info);}
		ServerInfo	*getCurrentServer() {return (_currentServer);}
		void	setCurrentServer(ServerInfo *info) {this->_currentServer = info;}
	private:
		void	_init();
		void	_loadTextures();
		void	_loadShaders();
		void	_stop();

		void	_processInput();

		void	_update(float deltaTime);

		void	_render();

		void	_swapScene(Scene *scene);

		bool	_running = true;
		Window	_window;
		Input	_input;

		TextureManager	_textures;
		ShaderManager	_shaders;

		Scene	*_currentScene;
		std::map<std::string, Scene*>	_scenes;
		std::vector<ServerInfo>			_serverInfos;
		ServerInfo						*_currentServer = NULL;
};

#endif

#pragma once

#include "GameOptions.hpp"
#include "mbl.hpp"
#include "app/scene/SceneManager.hpp"
#include "app/GameOptions.hpp"

class   Client
{
    #define TARGET_WINDOW_WIDTH 860
    #define TARGET_WINDOW_HEIGHT 520

    private:
        enum class State
        {
            RUNNING,
        };
    public:
    	Client() {}
        ~Client()
        {
         	_sceneManager.unload(*this);
        	mbl::ui::destroy();
        }

        void    run()
        {
            init();
            loop();
        }
    public:
        void    init();
        void    loop();

        bool&	singleplayer() {return (_singleplayer);}
        std::string&	addr() {return (_addr);}
        int&			port() {return (_port);}
        mbl::platform::Window&	window() {return (_win);}
        GameOptions&    opts() {return (_opts);}
        mbl::render::Texture&	background_tex() {return (_background_tex);}
    private:
     	bool			_singleplayer = true;
     	std::string		_addr;
      	int				_port = 0;

	   	SceneManager	_sceneManager;

		bool			_running = false;

		mbl::utils::Chrono  _time;

		mbl::platform::Window  _win;

		std::string			_username;
		std::string			_last_error;

		mbl::render::Texture	_background_tex;

		GameOptions _opts;
};

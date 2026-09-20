#pragma once

#include "mbl.hpp"
#include "app/scene/SceneManager.hpp"

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
        ~Client() {mbl::ui::destroy();}

        void    run()
        {
            init();
            loop();
        }
    private:
        void    init();
        void    loop();

        void    updateCamera(const mbl::platform::Input& input);

        void	genScreenMesh();
        void	genDebugCrosshair();
    private:
	   	SceneManager	_sceneManager;

		bool			_running = false;

		mbl::utils::Chrono  _time;

		mbl::platform::Window  _win;

		mbl::render::Camera  _cam;

		mbl::net::Client	_client;

		std::string			_username;

		std::string	_last_error;
};

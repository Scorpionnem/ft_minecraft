#include "app/scenes/GameScene.hpp"
#include "app/scene/SceneManager.hpp"
#include "app/Client.hpp"
#include "net/LAN.hpp"

void GameScene::init(Client& client)
{
	if (client.singleplayer())
	{
		_server = std::make_shared<Server>();
		_server->init();

		_serverThread = std::thread(
			[this](){_server->loop();}
		);

		if (_netClient.connect(_server->netServer().addr().c_str(), _server->netServer().port()) == -1)
			throw std::runtime_error(strerror(errno));
	}
	else
	{
		if (_netClient.connect(client.addr().c_str(), client.port()) == -1)
			throw std::runtime_error(strerror(errno));
	}

	if (_chunkThreads.threads() == 0)
		_chunkThreads.add(2);

	_world = {};
	_world.setAtlas(&_atlas);
	_world.loadBlocks();
	_atlas.upload();

	_world.setThreadPool(&_chunkThreads);

	_paused = false;
	client.window().captureMouse(!_paused);

	_fp_cam = {};
    _transition_cam = {};
    _tp_cam = {};

    _render_cam = &_fp_cam;

    mbl::render::renderer::AABBRenderer::gen_render_data();
    mbl::render::renderer::RayRenderer::gen_render_data();
    ChunkRender::load_shader();

    // SDL_GL_SetSwapInterval(0);

    _render_buffer.resize(client.window().width(), client.window().height());

    vec2f verts[] = {
		{-1.0f, -1.0f},
		{ 1.0f, -1.0f},
		{ 1.0f,  1.0f},
	};

	_screen_mesh.set_sizeof_layout(sizeof(vec2f));
	_screen_mesh.add_vertex_layout(0, 2, GL_FLOAT, 0);
	_screen_mesh.add_vertex_data(reinterpret_cast<u8*>(verts), sizeof(verts));
	_screen_mesh.upload();

	_post_shader.load("assets/shaders/post/screen.vert", "assets/shaders/post/screen.frag");

	// _time.start();
}

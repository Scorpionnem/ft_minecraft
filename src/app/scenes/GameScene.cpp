#include "app/scenes/GameScene.hpp"
#include "app/scene/SceneManager.hpp"
#include "app/Client.hpp"
#include "net/LAN.hpp"

void GameScene::init(Client& client)
{
	if (client.singleplayer())
	{
		_server = std::make_shared<Server>();
		_serverThread = std::thread(
			[this](){_server->run();}
		);

		while (!_server->running())
		{
			usleep(500);
		}

		if (_netClient.connect(_server->netServer().addr().c_str(), _server->netServer().port()) == -1)
			throw std::runtime_error(strerror(errno));
	}
	else
	{
		if (_netClient.connect(client.addr().c_str(), client.port()) == -1)
			throw std::runtime_error(strerror(errno));
	}

	_entities.clear();

	_paused = false;
	client.window().captureMouse(!_paused);

	_cam = {};
	_cam.near = 0.01;
    _cam.far = 1000.0;
    _cam.fov = 70;

    _mesh_shader.load("assets/shaders/mesh.vert", "assets/shaders/mesh.frag");
    mbl::loader::mesh::obj::load("assets/models/teapot.obj", _mesh, _atlas);

    _mesh.upload();
    _atlas.upload();
}

SceneCommand GameScene::update(Client& client, const mbl::platform::Input& input)
{
	if (input.close())
		return { .action = SceneAction::QUIT };

	if (input.wasPressed(SDLK_ESCAPE))
	{
		_paused = !_paused;
		client.window().captureMouse(!_paused);
	}

	if (_server_updt_time.get() > 0.05)
	{
		Packet::EntityPos	en_pos = {};

		en_pos.pos = _cam.pos;
		en_pos.yaw = _cam.yaw;
		en_pos.pitch = _cam.pitch;
		_netClient.send(&en_pos, sizeof(en_pos));
		_server_updt_time.start();
	}

	_updateCamera(input);

	try
	{
		_update_net(client);
	} catch (const std::exception& e)
	{
		return {.action = SceneAction::SWITCH, .targetScene = SceneTag::MULTIPLAYER};
	}

	mbl::ui::text(std::to_string(_netClient.rtt()), 0, vec2f(0.5, 0.0));

	if (_paused)
	{
		if (mbl::ui::button("Save and Quit to Title", 0, vec2f(200, 20), ANCHOR_CENTER))
			return {.action = SceneAction::SWITCH, .targetScene = SceneTag::MAIN};
	}
	return {};
}

void	GameScene::_update_net(Client& client)
{
	mbl::net::Client::Event	event;
	u8					buf[4096] = {};
	u64					size;

	_netClient.update();
	do
	{
		if (_netClient.recv(buf, sizeof(buf), event, size) == -1)
			throw std::runtime_error(strerror(errno));
		if (event == mbl::net::Client::Event::DISCONNECT)
		{
			_netClient.disconnect();
			throw std::runtime_error("Disconnected");
		}
		if (event == mbl::net::Client::Event::RECV)
		{
			mbl::ui::text(std::to_string(size), 0, ANCHOR_TOP_RIGHT);
			_dispatch_packet(client, buf, size);
		}
	} while (event != mbl::net::Client::Event::NONE);
}

void	GameScene::_dispatch_packet(Client& client, u8 *data, u64 size)
{
	if (size < sizeof(Packet::Header))
		return ;

	Packet::Header*	hdr = reinterpret_cast<Packet::Header*>(data);
	if (hdr->magic != MINECRAFT_PCKT_MAGIC) // invalid packet
		return ;

	switch (hdr->type)
	{
		case ENTITYPOS_TYPE:
		{
			Packet::EntityPos*	en_pckt = reinterpret_cast<Packet::EntityPos*>(data);

			Entity*	en_p = _entities.insert(en_pckt->id, {});
			en_p->id = en_pckt->id;
			en_p->pos = en_pckt->pos;
			en_p->pitch = en_pckt->pitch;
			en_p->yaw = en_pckt->yaw;
		}
		default :
			return ;
	}
}

void GameScene::render(Client& client)
{
	for (const auto& en : _entities.get_all())
	{
		_mesh_shader.bind();
	    _mesh_shader.setMat4("uProj", _cam.getProjectionMatrix());
	    _mesh_shader.setMat4("uView", _cam.getViewMatrix());
	    _mesh_shader.setMat4("uModel", mat4f::translate(en.second.pos) * mat4f::rotateX(radians(en.second.pitch)) * mat4f::rotateZ(radians(en.second.yaw)));
	    _atlas.bind(0);
	    _mesh_shader.setInt("uTex", 0);
	    _mesh.draw(GL_TRIANGLES);
	}
}

void GameScene::unload(Client& client)
{
	client.window().captureMouse(false);

	_netClient.disconnect();

	if (_server)
	{
		_server->stop();
		_serverThread.join();
		_server = nullptr;
	}
}

void    GameScene::_updateCamera(const mbl::platform::Input& input)
{
	_cam.aspect = input.aspect();

	if (_paused)
		return ;

    float   move_speed = 20 * input.delta();
    float   speed = 100 * input.delta();
    float	sensitivity = 0.3;

    if (input.isDown(SDLK_w))
        _cam.pos = _cam.pos + (_cam.front() * move_speed);
    if (input.isDown(SDLK_s))
        _cam.pos = _cam.pos + vec3f(-1.0) * (_cam.front() * move_speed);
    if (input.isDown(SDLK_SPACE))
        _cam.pos = _cam.pos + (vec3f(0, 1, 0) * move_speed);
    if (input.isDown(SDLK_LSHIFT))
        _cam.pos = _cam.pos + vec3f(-1.0) * (vec3f(0, 1, 0) * move_speed);

    vec3f right = vec3f(cos(radians(_cam.yaw)), 0.0f, sin(radians(_cam.yaw)));

    if (input.isDown(SDLK_a))
        _cam.pos = _cam.pos - right * move_speed;
    if (input.isDown(SDLK_d))
        _cam.pos = _cam.pos + right * move_speed;

    _cam.pitch += -input.mouseDY() * sensitivity;
    _cam.yaw += input.mouseDX() * sensitivity;

    _cam.pitch = std::clamp(_cam.pitch, -90.0f, 90.0f);

    if (_cam.yaw > 360)
        _cam.yaw = 0;
    if (_cam.yaw < 0)
        _cam.yaw = 360;
}

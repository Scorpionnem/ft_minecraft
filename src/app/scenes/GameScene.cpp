#include "app/scenes/GameScene.hpp"
#include "app/scene/SceneManager.hpp"
#include "app/Client.hpp"
#include "net/LAN.hpp"

void GameScene::init(Client& client)
{
	// SDL_GL_SetSwapInterval(0);
	glEnable(GL_DEPTH_TEST);
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

	if (_chunkThreads.threads() == 0)
		_chunkThreads.add(2);

	_world = {};
	_entities = {};

	_world.setThreadPool(&_chunkThreads);

	_paused = false;
	client.window().captureMouse(!_paused);

	_fp_cam = {};
	_fp_cam.near = 0.01;
    _fp_cam.far = 1000.0;
    _fp_cam.fov = 70;
    _fp_cam.pos = vec3f(0);
    _fp_cam.yaw = 0;
    _fp_cam.pitch = 0;
    _transition_cam = {};
    _tp_cam = {};
    _render_cam = &_fp_cam;

    _mesh_shader.load("assets/shaders/mesh.vert", "assets/shaders/mesh.frag");
    mbl::loader::mesh::obj::load("assets/models/teapot.obj", _mesh, _atlas);

    mbl::render::renderer::AABBRenderer::gen_render_data();
    mbl::render::renderer::RayRenderer::gen_render_data();

    Chunk::load_shader();

    _entities.clear();

    _mesh.upload();
    _atlas.upload();
}

void	GameScene::_show_f3(Client& client, const mbl::platform::Input& input)
{
	int	i = 0;
	float	text_y_size = mbl::ui::getFontSizeY();

	std::string	fps_str = std::to_string(_fps) + " fps";
	mbl::ui::text(fps_str, vec2f(0, text_y_size * i++), ANCHOR_TOP_LEFT);

	std::string	rtt_str = std::to_string(_netClient.rtt()) + " ms " + _netClient.addr() + ":" + std::to_string(_netClient.port());
	mbl::ui::text(rtt_str, vec2f(0, text_y_size * i++), ANCHOR_TOP_LEFT);

	std::string	net_data_str = "RX/TX: " + std::to_string(_rx_pckt) + " / " + std::to_string(_tx_pckt);
	mbl::ui::text(net_data_str, vec2f(0, text_y_size * i++), ANCHOR_TOP_LEFT);
	i++;

	std::string	pos_str = "XYZ: " + std::to_string(_fp_cam.pos.x()) + " / " + std::to_string(_fp_cam.pos.y()) + " / " + std::to_string(_fp_cam.pos.z());
	mbl::ui::text(pos_str, vec2f(0, text_y_size * i++), ANCHOR_TOP_LEFT);

	std::string	yawpitch_str = "Yaw/Pitch: " + std::to_string(_fp_cam.yaw) + " / " + std::to_string(_fp_cam.pitch);
	mbl::ui::text(yawpitch_str, vec2f(0, text_y_size * i++), ANCHOR_TOP_LEFT);

	std::string	dir_str = "Facing: " + to_string(static_cast<mbl::utils::FacingCardinal>(mbl::utils::facing(_fp_cam.front()))) + " (" + to_string(mbl::utils::facing(_fp_cam.front())) + ")";
	mbl::ui::text(dir_str, vec2f(0, text_y_size * i++), ANCHOR_TOP_LEFT);
}

SceneCommand GameScene::update(Client& client, const mbl::platform::Input& input)
{
	if (input.close())
		return { .action = SceneAction::QUIT };

	_show_f3(client, input);

	if (input.wasPressed(SDLK_ESCAPE))
	{
		_paused = !_paused;
		client.window().captureMouse(!_paused);
	}

	if (_server_updt_time.get() > (1.0 / 20.0))
	{
		_world.requestInRange(worldToChunkWorld(_fp_cam.pos, Chunk::SIZE), RENDER_DISTANCE, _netClient);

		_fps = 1.0 / input.delta();
		_rx_pckt = 0; _tx_pckt = 0;

		Packet::EntityInfo	en_pos = {};

		en_pos.entity = Player();
		en_pos.entity.pos = _fp_cam.pos;
		en_pos.entity.yaw = _fp_cam.yaw;
		en_pos.entity.pitch = _fp_cam.pitch;
		netSend(&en_pos, sizeof(en_pos));
		_server_updt_time.start();
	}

	_updateCamera(input);

	try
	{
		_update_net(client);
	} catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
		return {.action = SceneAction::SWITCH, .targetScene = SceneTag::MULTIPLAYER};
	}

	if (_paused)
	{
		if (mbl::ui::button("Save and Quit to Title", 0, vec2f(200, 20), ANCHOR_CENTER))
			return {.action = SceneAction::SWITCH, .targetScene = SceneTag::MAIN};
	}
	return {};
}

void GameScene::render(Client& client)
{
	vec3f	size = vec3f(0.5);
    mbl::utils::aabb3f	cam_box = {.pos = _fp_cam.pos - (size / 2), .size = size};
    mbl::render::renderer::AABBRenderer::draw(cam_box, *_render_cam, vec3f(1));
    mbl::render::renderer::RayRenderer::draw(_fp_cam.pos, _fp_cam.pos + _fp_cam.front(), *_render_cam, vec3f(0, 0, 1));

    _world.draw(worldToChunkWorld(_fp_cam.pos, Chunk::SIZE), RENDER_DISTANCE, *_render_cam);

    _entities.draw(*_render_cam);
}

void	GameScene::_update_net(Client& client)
{
	mbl::net::Client::Event	event;
	u8					buf[4096] = {};
	u64					size;

	int	packets_recvd = 0;

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
			_dispatch_packet(client, buf, size);

			packets_recvd++;
		}
	} while (event != mbl::net::Client::Event::NONE && packets_recvd < MAX_PACKETS_PER_FRAME);
}

void	GameScene::_dispatch_packet(Client& client, u8 *data, u64 size)
{
	if (size < sizeof(Packet::Header))
		return ;

	Packet::Header*	hdr = reinterpret_cast<Packet::Header*>(data);
	if (hdr->magic != MINECRAFT_PCKT_MAGIC) // invalid packet
		return ;

	_rx_pckt++;

	switch (hdr->type)
	{
		case ENTITYINFO_TYPE:
		{
			Packet::EntityInfo*	pos_pckt = reinterpret_cast<Packet::EntityInfo*>(data);
			Entity	en = pos_pckt->entity;

			_entities.set(en.id, en);
			break ;
		}
		case CHUNKDATA_TYPE:
		{
			Packet::ChunkData*	chunk_pckt = reinterpret_cast<Packet::ChunkData*>(data);

			_world.netChunkData(chunk_pckt);
			break ;
		}
		case CHUNKDATASPECIAL_TYPE:
		{
			Packet::ChunkDataSpecial*	chunk_pckt = reinterpret_cast<Packet::ChunkDataSpecial*>(data);

			_world.netChunkDataSpecial(chunk_pckt);
			break ;
		}
		default :
			return ;
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

void    rayBoxDst(float& dstToBox, float& dstInsideBox, mbl::utils::aabb3f bounds, vec3f rayOrig, vec3f rayDir)
{
    vec3f	t0 = (bounds.pos - rayOrig) / rayDir;
    vec3f	t1 = ((bounds.pos + bounds.size) - rayOrig) / rayDir;
    vec3f	tmin = min(t0, t1);
    vec3f	tmax = max(t0, t1);

    float   dstA = std::max(std::max(tmin.x(), tmin.y()), tmin.z());
    float   dstB = std::min(std::min(tmax.x(), tmax.y()), tmax.z());

    dstToBox = std::max(0.0f, dstA);
    dstInsideBox = std::max(0.0f, dstB - dstToBox);
}

vec3f	resolve_collision(const vec3f& velocity, const mbl::utils::aabb3f& a, const mbl::utils::aabb3f& b, bool slide, mbl::render::Camera& cam)
{
	vec3f	res;
	mbl::utils::aabb3f	test_box = {.pos = b.pos - (a.size / 2.0), .size = b.size + (a.size)};
	mbl::render::renderer::AABBRenderer::draw(test_box, cam, vec3f(0, 0, 1));
	for (int i = 0; i < 3; i++)
	{
		vec3f	vel = vec3f(i == 0 ? velocity.x() : 0, i == 1 ? velocity.y() : 0, i == 2 ? velocity.z() : 0);

		float	dstToBox;
		float	dstInsideBox;
		rayBoxDst(dstToBox, dstInsideBox, test_box, cam.pos, vel);

		if (dstInsideBox > 0) // hit box
			res += (min(abs(vel), abs(vel * dstToBox)) * sign(vel));
		else
			res += vel;
	}
	return (res);
}

void    GameScene::_updateCamera(const mbl::platform::Input& input)
{
	_fp_cam.aspect = input.aspect();

	if (_paused)
		return ;

    float   move_speed = 20 * input.delta();
    float   speed = 100 * input.delta();
    float	sensitivity = 0.3;

    vec3f right = vec3f(cos(radians(_fp_cam.yaw)), 0.0f, sin(radians(_fp_cam.yaw)));

    vec3f	velocity;
    if (input.isDown(SDLK_w))
        velocity += (_fp_cam.front() * move_speed);
    if (input.isDown(SDLK_s))
        velocity += vec3f(-1.0) * (_fp_cam.front() * move_speed);
    if (input.isDown(SDLK_SPACE))
        velocity += (vec3f(0, 1, 0) * move_speed);
    if (input.isDown(SDLK_LSHIFT))
        velocity += vec3f(-1.0) * (vec3f(0, 1, 0) * move_speed);
    if (input.isDown(SDLK_a))
        velocity += -(right * move_speed);
    if (input.isDown(SDLK_d))
        velocity += right * move_speed;

	_fp_cam.pos += velocity;

	_fp_cam.pitch += -input.mouseDY() * sensitivity;
	_fp_cam.yaw += input.mouseDX() * sensitivity;

	_fp_cam.pitch = std::clamp(_fp_cam.pitch, -90.0f, 90.0f);
	if (_fp_cam.yaw > 360) _fp_cam.yaw = 0;
	if (_fp_cam.yaw < 0) _fp_cam.yaw = 360;

	_tp_cam = _fp_cam;
	_tp_cam.pos = _fp_cam.pos - vec3f(_tp_distance_target_set) * _fp_cam.front();
	_transition_cam.yaw = _fp_cam.yaw;
	_transition_cam.pitch = _fp_cam.pitch;
	_transition_cam.aspect = _fp_cam.aspect;
	_transition_cam.near = _fp_cam.near;
	_transition_cam.far = _fp_cam.far;
	_transition_cam.fov = _fp_cam.fov;

	if (input.wasPressed(SDLK_F5))
	{
		_tp_toggle = !_tp_toggle;
		_moving = true;
	}

	if (input.scrollY() != 0 && _tp_toggle)
	{
		_tp_distance_target_set -= input.scrollY();
		_tp_distance_target_set = std::clamp(_tp_distance_target_set, 1.0f, 128.0f);
	}

	constexpr float	anim_speed = 8;
	constexpr float	snap_distance = (1 / 64.0f);
	if (_tp_toggle)
	{
		if (_moving)
		{
			_tp_distance = lerp(_tp_distance, _tp_distance_target_set, anim_speed * input.delta());
			_transition_cam.pos = _fp_cam.pos - vec3f(_tp_distance) * _fp_cam.front();
			if (vec3f::distance(_transition_cam.pos, _tp_cam.pos) < snap_distance)
				_moving = false;
		}
		else
			_tp_distance = _tp_distance_target_set;
	}
	else if (!_tp_toggle)
	{
		if (_moving)
		{
			_tp_distance = lerp(_tp_distance, 0, anim_speed * input.delta());
			_transition_cam.pos = _fp_cam.pos - vec3f(_tp_distance) * _fp_cam.front();
			if (vec3f::distance(_transition_cam.pos, _fp_cam.pos) < snap_distance)
				_moving = false;
		}
		else
			_tp_distance = 0;
	}

	_render_cam = (_tp_toggle && !_moving) ? &_tp_cam : (!_tp_toggle && !_moving) ? &_fp_cam : &_transition_cam;
}

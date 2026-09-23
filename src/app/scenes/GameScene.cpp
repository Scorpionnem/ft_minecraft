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

	_paused = false;
	client.window().captureMouse(!_paused);

	_cam = {};
	_cam.near = 0.01;
    _cam.far = 1000.0;
    _cam.fov = 70;
    _cam.pos = vec3f(-10);
    _cam.yaw = 0;
    _cam.pitch = 0;

    _cam2 = _cam;

    _mesh_shader.load("assets/shaders/mesh.vert", "assets/shaders/mesh.frag");
    mbl::loader::mesh::obj::load("assets/models/teapot.obj", _mesh, _atlas);

    mbl::render::renderer::AABBRenderer::gen_render_data();
    mbl::render::renderer::RayRenderer::gen_render_data();

    _mesh.upload();
    _atlas.upload();
}

void	GameScene::_show_f3(Client& client, const mbl::platform::Input& input)
{
	int	i = 0;
	float	text_y_size = mbl::ui::getFontSizeY();

	std::string	fps_str = std::to_string(static_cast<int>(1.0 / input.delta())) + " fps";
	mbl::ui::text(fps_str, vec2f(0, text_y_size * i++), ANCHOR_TOP_LEFT);

	std::string	rtt_str = std::to_string(_netClient.rtt()) + " ms " + _netClient.addr() + ":" + std::to_string(_netClient.port());
	mbl::ui::text(rtt_str, vec2f(0, text_y_size * i++), ANCHOR_TOP_LEFT);

	i++;

	std::string	pos_str = "XYZ: " + std::to_string(_cam.pos.x()) + " / " + std::to_string(_cam.pos.y()) + " / " + std::to_string(_cam.pos.z());
	mbl::ui::text(pos_str, vec2f(0, text_y_size * i++), ANCHOR_TOP_LEFT);

	std::string	yawpitch_str = "Yaw/Pitch: " + std::to_string(_cam.yaw) + " / " + std::to_string(_cam.pitch);
	mbl::ui::text(yawpitch_str, vec2f(0, text_y_size * i++), ANCHOR_TOP_LEFT);

	std::string	dir_str = "Facing: " + to_string(static_cast<mbl::utils::FacingCardinal>(mbl::utils::facing(_cam.front()))) + " (" + to_string(mbl::utils::facing(_cam.front())) + ")";
	mbl::ui::text(dir_str, vec2f(0, text_y_size * i++), ANCHOR_TOP_LEFT);
}

SceneCommand GameScene::update(Client& client, const mbl::platform::Input& input)
{
	if (input.close() || input.wasPressed(SDLK_ESCAPE))
		return { .action = SceneAction::QUIT };

	_show_f3(client, input);

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

	if (input.wasPressed(SDLK_F5))
		_f5_toggle = !_f5_toggle;
	if (input.scrollY() != 0)
	{
		_f5_distance_target -= input.scrollY();
		_f5_distance_target = std::clamp(_f5_distance_target, 1.0f, 128.0f);
	}

	mbl::render::Camera&	cam = _f5_toggle ? _cam2 : _cam;

	_updateCamera(input, _cam);

	_cam2 = _cam;
	_cam2.pos = _cam.pos - vec3f(_f5_distance) * _cam.front();
	_f5_distance = lerp(_f5_distance, _f5_distance_target, 0.33);

	vec3f	size = vec3f(0.8, 0.8, 0.8);
    mbl::utils::aabb3f	cam_box = {.pos = _cam.pos - (size / 2), .size = size};
	mbl::render::renderer::AABBRenderer::draw(cam_box, cam, vec3f(0, 1, 0));
    mbl::render::renderer::RayRenderer::draw(_cam.pos, _cam.pos + _cam.front(), cam, vec3f(1));

    mbl::render::renderer::AABBRenderer::draw(mbl::utils::aabb3f{0, 10}, cam, vec3f(1, 0, 0.5));

	try
	{
		_update_net(client);
	} catch (const std::exception& e)
	{
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

void    GameScene::_updateCamera(const mbl::platform::Input& input, mbl::render::Camera& cam)
{
	cam.aspect = input.aspect();

	if (_paused)
		return ;

    float   move_speed = 20 * input.delta();
    float   speed = 100 * input.delta();
    float	sensitivity = 0.3;

    vec3f	velocity;

    if (input.isDown(SDLK_w))
        velocity += (cam.front() * move_speed);
    if (input.isDown(SDLK_s))
        velocity += vec3f(-1.0) * (cam.front() * move_speed);
    if (input.isDown(SDLK_SPACE))
        velocity += (vec3f(0, 1, 0) * move_speed);
    if (input.isDown(SDLK_LSHIFT))
        velocity += vec3f(-1.0) * (vec3f(0, 1, 0) * move_speed);

    vec3f right = vec3f(cos(radians(cam.yaw)), 0.0f, sin(radians(cam.yaw)));

    if (input.isDown(SDLK_a))
        velocity += -(right * move_speed);
    if (input.isDown(SDLK_d))
        velocity += right * move_speed;

	cam.pos += velocity;

	cam.pitch += -input.mouseDY() * sensitivity;
	cam.yaw += input.mouseDX() * sensitivity;

	cam.pitch = std::clamp(cam.pitch, -90.0f, 90.0f);

	if (cam.yaw > 360)
		cam.yaw = 0;
	if (cam.yaw < 0)
		cam.yaw = 360;
}

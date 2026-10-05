#include "app/scenes/GameScene.hpp"
#include "app/scene/SceneManager.hpp"
#include "app/Client.hpp"

void    GameScene::_updateCamera(Client& client, const mbl::platform::Input& input)
{
	_fp_cam.aspect = input.aspect();
	_fp_cam.fov = client.opts().fov;

	if (_paused)
		return ;

    float   move_speed = 10 * input.delta();
    if (input.isDown(SDLK_LCTRL))
    	move_speed = 100 * input.delta();
    float	sensitivity = 0.3 * (client.opts().mouse_sensitivity / 100.0);

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

	_fp_cam.pitch += input.mouseDY() * (client.opts().invert_y ? 1 : -1) * sensitivity;
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

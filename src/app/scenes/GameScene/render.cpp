#include "app/scenes/GameScene.hpp"
#include "app/scene/SceneManager.hpp"
#include "app/Client.hpp"

void GameScene::render(Client& c)
{
	glEnable(GL_DEPTH_TEST);

	if (_debug)
		_show_f3(c);
	else
	{
		int	i = 0;

		if (c.opts().show_fps)
			mbl::ui::text(std::to_string(_fps) + " fps", vec2f(0, mbl::ui::getFontSizeY() * i++), ANCHOR_TOP_LEFT);
		if (c.opts().show_coords)
		{
			std::string	pos_str = "XYZ: " + std::to_string(_fp_cam.pos.x()) + " / " + std::to_string(_fp_cam.pos.y()) + " / " + std::to_string(_fp_cam.pos.z());
			mbl::ui::text(pos_str, vec2f(0, mbl::ui::getFontSizeY() * i++), ANCHOR_TOP_LEFT);
		}
	}

	_render_buffer.bind();
	mbl::render::FrameBuffer::clear();

	if (!_paused)
		mbl::ui::text("+", 0);

	if (_tp_toggle || (!_tp_toggle && _moving))
	{
		vec3f	size = vec3f(0.5);
	    mbl::utils::aabb3f	cam_box = {.pos = _fp_cam.pos - (size / 2), .size = size};
	    mbl::render::renderer::AABBRenderer::draw(cam_box, *_render_cam, vec3f(1));
	    mbl::render::renderer::RayRenderer::draw(_fp_cam.pos, _fp_cam.pos + _fp_cam.front(), *_render_cam, vec3f(0, 0, 1));
	}

    _world.draw(worldToChunkWorld(_fp_cam.pos, Chunk::SIZE), c.opts().render_distance, *_render_cam, c.opts().show_chunk_borders || _debug, octdepth);

    _render_buffer.unbind();
    glViewport(0, 0, c.window().width(), c.window().height());

    _render_buffer.bindColor(0);
	_render_buffer.bindDepth(1);
	_post_shader.bind();
	_post_shader.setInt("uColorFrameBuffer", 0);
	_post_shader.setInt("uDepthFrameBuffer", 1);
	_post_shader.setFloat("uNear", _render_cam->near);
	_post_shader.setFloat("uFar", _render_cam->far);
	_post_shader.setFloat("uTime", _time.get());
	_post_shader.setFloat("uScreenWidth", c.window().width());
	_post_shader.setFloat("uScreenHeight", c.window().height());
	_post_shader.setInt("uDim", _paused);
	_post_shader.setInt("uUnderwater", false); // need to detect wataaa soon
	_screen_mesh.draw(GL_TRIANGLES);
}

void	GameScene::_show_f3(Client& client)
{
	(void)client;
	int		i = 0;
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

#ifndef ROBOX2D_BASE_GRAPHICS_HPP
#define ROBOX2D_BASE_GRAPHICS_HPP

#include <robox2d/gui/base.hpp>
#include <robox2d/gui/magnum/glfw_application.hpp>

namespace robox2d {
  class Simu;

  namespace gui {
    namespace magnum {

    template <typename T = GlfwApplication>
    class BaseGraphics : public Base {
    public:
      BaseGraphics(const GraphicsConfiguration& configuration = GraphicsConfiguration())
        : _configuration(configuration), _frame_counter(0), _enabled(true)
      {
        Corrade::Utility::Debug magnum_silence_output{nullptr};
        //robot_dart_initialize_magnum_resources();
      }

      void set_simu(robox2d::Simu* simu) override
      {
        _world = simu->world();
        _simu = simu;
        _magnum_app.reset(make_application<T>(simu, _configuration));
      }

      ~BaseGraphics() {}

      bool done() const override
      {
        return _magnum_app->done();
      }

      void refresh() override
      {
        if (!_enabled)
          return;

        _magnum_app->render();
        _frame_counter++;
      }

      void set_enable(bool enable) override
      {
        _enabled = enable;
      }

/*
	void look_at(const Eigen::Vector3d& camera_pos,
	const Eigen::Vector3d& look_at = Eigen::Vector3d(0, 0, 0),
	const Eigen::Vector3d& up = Eigen::Vector3d(0, 0, 1))
	{
	_magnum_app->lookAt(camera_pos, look_at, up);
	}

	void clear_lights()
	{
	_magnum_app->clearLights();
	}

	void add_light(const magnum::gs::Light& light)
	{
	_magnum_app->addLight(light);
	}

	std::vector<gs::Light>& lights()
	{
	return _magnum_app->lights();
	}

	size_t num_lights() const
	{
	return _magnum_app->numLights();
	}

	magnum::gs::Light& light(size_t i)
	{
	return _magnum_app->light(i);
	}
      */
      //void set_recording(bool recording, bool recording_depth = false) { _magnum_app->record(recording, recording_depth); }
      //bool recording() { return _magnum_app->isRecording(); }
      //bool recording_depth() { return _magnum_app->isDepthRecording(); }

      //bool is_shadowed() const { return _magnum_app->isShadowed(); }
      //void enable_shadows(bool enable = true) { _magnum_app->enableShadows(enable); }

      Magnum::Image2D* magnum_image() override
      {
        if (_magnum_app->image())
          return &(*_magnum_app->image());
        return nullptr;
      }

      Image image() override
      {
        auto image = magnum_image();
        if (image)
          return rgb_from_image(image);
        return Image();
      }
      /*
	GrayscaleImage depth_image() override { return _magnum_app->depthImage(); }
	GrayscaleImage raw_depth_image() override { return _magnum_app->rawDepthImage(); }

	void set_speed(const Magnum::Vector2& speed) { _magnum_app->camera().setSpeed(speed); }
	void set_near_plane(double near_plane) { _magnum_app->camera().setNearPlane(near_plane); }
	void set_far_plane(double far_plane) { _magnum_app->camera().setFarPlane(far_plane); }
	void set_fov(double fov) { _magnum_app->camera().setFOV(fov); }
	void set_camera_params(double near_plane, double far_plane, double fov, size_t width, size_t height) { _magnum_app->camera().setCameraParameters(near_plane, far_plane, fov, width, height); }

	Magnum::Vector2 speed() const { return _magnum_app->camera().speed(); }
	double near_plane() const { return _magnum_app->camera().nearPlane(); }
	double far_plane() const { return _magnum_app->camera().farPlane(); }
	double fov() const { return _magnum_app->camera().fov(); }
      */
      BaseApplication* magnum_app() { return &*_magnum_app; }

      void record_video(const std::string& video_fname, int fps) { _magnum_app->record_video(video_fname, fps); }



    protected:
      GraphicsConfiguration _configuration;
      std::shared_ptr<b2World> _world;
      size_t _render_period, _frame_counter;

      bool _enabled;

      std::unique_ptr<BaseApplication> _magnum_app;
    };

    } // namespace magnum
  } // namespace gui
} // namespace robox2d

#endif // ROBOX2D_BASE_GRAPHICS_HPP

#include "glfw_application.hpp"

#include <iostream>

#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/GL/Version.h>

#include <Magnum/PixelFormat.h>

namespace robox2d {
    namespace gui {
      namespace magnum {
      GlfwApplication::GlfwApplication(int argc, char** argv, robox2d::Simu* simu, const GraphicsConfiguration& configuration)
	: BaseApplication(configuration),
          Magnum::Platform::Application({argc, argv}, Magnum::NoCreate),
          _speedMove(0.f),
          _speedStrafe(0.f),
          _bg_color(configuration.bg_color[0],
                    configuration.bg_color[1],
                    configuration.bg_color[2],
                    configuration.bg_color[3])
      {

	/* Try 16x MSAA */
	Configuration conf;
	GLConfiguration glConf;
	conf.setTitle(configuration.title);
	conf.setSize({static_cast<int>(configuration.width), static_cast<int>(configuration.height)});
	conf.setWindowFlags(Configuration::WindowFlag::Resizable);
	glConf.setSampleCount(8);
	if (!tryCreate(conf, glConf))
	  create(conf, glConf.setSampleCount(0));
	//ROBOT_DART_EXCEPTION_ASSERT(Magnum::GL::Context::current().version() >= Magnum::GL::Version::GL320, "robot_dart requires at least OpenGL 3.2 for rendering!");

	/* Initialize Robox2d world */
        GraphicsConfiguration config = configuration;
        config.width = Magnum::GL::defaultFramebuffer.viewport().size()[0];
        config.height = Magnum::GL::defaultFramebuffer.viewport().size()[1];

        init(simu, config);

	/* Loop at 60 Hz max */
	setSwapInterval(1);

	redraw();
      }

      GlfwApplication::~GlfwApplication()
      {
	GLCleanUp();
	kill_video();
      }

      void GlfwApplication::render()
      {
	mainLoopIteration();
      }

      void GlfwApplication::viewportEvent(const Magnum::Vector2i& size)
      {
	Magnum::GL::defaultFramebuffer.setViewport({{}, size});

	_camera->setViewport(size);
      }

      void GlfwApplication::drawEvent()
      {
	Magnum::GL::defaultFramebuffer.clear(Magnum::GL::FramebufferClear::Color);

	/* Update graphic meshes/materials and render */
	update_graphics();

        /* Change clear color to _bg_color */
        Magnum::GL::Renderer::setClearColor(_bg_color);

	/* Populate instance data with transformations and colors */
	arrayResize(*_boxInstanceData, 0);
	arrayResize(*_circleInstanceData, 0);
	arrayResize(*_lineInstanceData, 0);
	_camera->draw(*_drawables);

	/* Upload instance data to the GPU and draw everything in a single call per shape */
	_instanceBuffer->setData(*_boxInstanceData, Magnum::GL::BufferUsage::DynamicDraw);
	_boxMesh->setInstanceCount(_boxInstanceData->size());
	_shader->setTransformationProjectionMatrix(_camera->projectionMatrix())
	  .draw(*_boxMesh);

	_instanceBuffer->setData(*_circleInstanceData, Magnum::GL::BufferUsage::DynamicDraw);
	_circleMesh->setInstanceCount(_circleInstanceData->size());
	_shader->setTransformationProjectionMatrix(_camera->projectionMatrix())
	  .draw(*_circleMesh);

	_instanceBuffer->setData(*_lineInstanceData, Magnum::GL::BufferUsage::DynamicDraw);
	_lineMesh->setInstanceCount(_lineInstanceData->size());
	_shader->setTransformationProjectionMatrix(_camera->projectionMatrix())
	  .draw(*_lineMesh);

	_image = Magnum::GL::defaultFramebuffer.read(Magnum::GL::defaultFramebuffer.viewport(), {Magnum::PixelFormat::RGB8Unorm});
	video();

	swapBuffers();
	redraw();
      }

      void GlfwApplication::keyReleaseEvent(KeyEvent& event)
      {
	event.setAccepted();
      }

      void GlfwApplication::keyPressEvent(KeyEvent& event)
      {
	event.setAccepted();
      }

      void GlfwApplication::mouseScrollEvent(MouseScrollEvent& event)
      {
	event.setAccepted();
      }

      void GlfwApplication::mouseMoveEvent(MouseMoveEvent& event)
      {
	event.setAccepted();
      }

      void GlfwApplication::exitEvent(ExitEvent& event)
      {
	_done = true;
	event.setAccepted();
      }
    } // namespace magnum
  } // namespace gui
} // namespace robox2d

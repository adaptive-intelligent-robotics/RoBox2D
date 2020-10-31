#include "glfw_application.hpp"

#include <iostream>

#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/GL/Version.h>

#include <Magnum/PixelFormat.h>

namespace robox2d {
    namespace gui {
      GlfwApplication::GlfwApplication(int argc, char** argv, robox2d::Simu* simu, size_t width, size_t height, const std::string& title)
	: Magnum::Platform::Application({argc, argv}, Magnum::NoCreate), _speedMove(0.f), _speedStrafe(0.f)
      {
	/* Try 16x MSAA */
	Configuration conf;
	GLConfiguration glConf;
	conf.setTitle(title);
	conf.setSize({static_cast<int>(width), static_cast<int>(height)});
	conf.setWindowFlags(Configuration::WindowFlag::Resizable);
	glConf.setSampleCount(8);
	if (!tryCreate(conf, glConf))
	  create(conf, glConf.setSampleCount(0));
	//ROBOT_DART_EXCEPTION_ASSERT(Magnum::GL::Context::current().version() >= Magnum::GL::Version::GL320, "robot_dart requires at least OpenGL 3.2 for rendering!");
	
	/* Initialize Robox2d world */
	init(simu, Magnum::GL::defaultFramebuffer.viewport().size()[0], Magnum::GL::defaultFramebuffer.viewport().size()[1]);
	
	/* Loop at 60 Hz max */
	setSwapInterval(1);
	
	redraw();

      // TODO : as in robot_dart:
      //  create class Graphics to encapsulate the behaviour of GlfwApplication
      //  Graphics should be the one setting the sync param of simu to true
	simu->set_sync(true);
      }
      
      GlfwApplication::~GlfwApplication()
      {
	GLCleanUp();
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

    } // namespace gui
} // namespace robox2d

#include "windowless_gl_application.hpp"

// #include <Magnum/DebugTools/Screenshot.h>
#include <Magnum/GL/RenderbufferFormat.h>
#include <Magnum/GL/Renderer.h>


#include <iostream>
#include <fstream>

#include "robox2d/gui/magnum/helper.hpp"

namespace robox2d {
    namespace gui {
        namespace magnum {
            WindowlessGLApplication::WindowlessGLApplication(int argc, char** argv, robox2d::Simu* simu, size_t width, size_t height, const std::string& title)
                : Magnum::Platform::WindowlessApplication({argc, argv}, Magnum::NoCreate)
            {
                /* Assume context is given externally, if not create it */
                if (!Magnum::GL::Context::hasCurrent()) {
                    Corrade::Utility::Debug{} << "GL::Context not provided. Creating...";
                    if (!tryCreateContext(Configuration())) {
                        Corrade::Utility::Error{} << "Could not create context!";
                        return;
                    }
                }
                // else
                // Corrade::Utility::Debug{} << "Created context with: " << Magnum::GL::Context::current().versionString();

                /* Create FrameBuffer to draw */
		_width = width;
		_height = height;
                int w = width, h = height;
                _framebuffer = Magnum::GL::Framebuffer({{}, {w, h}});
                _color.setStorage(Magnum::GL::RenderbufferFormat::RGBA8, {w, h});
                _depth.setStorage(Magnum::GL::RenderbufferFormat::DepthComponent, {w, h});

                _format = Magnum::PixelFormat::RGBA8Unorm;

                _framebuffer.attachRenderbuffer(
                    Magnum::GL::Framebuffer::ColorAttachment(0), _color);
                _framebuffer.attachRenderbuffer(
                    Magnum::GL::Framebuffer::BufferAttachment::Depth, _depth);

                /* Initialize DART world */
                init(simu, width, height);

                // TODO : as in robot_dart:
                //  create class WindowlessGraphics to encapsulate the behaviour of WindowlessApplication
                //  WindowlessGraphics should be the one setting the sync param of simu to false by default
                simu->set_sync(false);
            }

            WindowlessGLApplication::~WindowlessGLApplication()
            {
                GLCleanUp();
		kill_video();
            }

            void WindowlessGLApplication::render()
            {
                /* Update graphic meshes/materials and render */
                // updateGraphics();
                update_graphics();
                /* Update lights transformations */
                // updateLights(*_camera);

                // Magnum::GL::Renderer::enable(Magnum::GL::Renderer::Feature::DepthTest);
                // Magnum::GL::Renderer::enable(Magnum::GL::Renderer::Feature::FaceCulling);
                // Magnum::GL::Renderer::enable(Magnum::GL::Renderer::Feature::Blending);
                // Magnum::GL::Renderer::setBlendFunction(Magnum::GL::Renderer::BlendFunction::SourceAlpha, Magnum::GL::Renderer::BlendFunction::OneMinusSourceAlpha);
                // Magnum::GL::Renderer::setBlendEquation(Magnum::GL::Renderer::BlendEquation::Add);

                /* Change default clear color to black */
                Magnum::GL::Renderer::setClearColor(Magnum::Vector4{0.f, 0.f, 0.f, 1.f});

                /* Bind the framebuffer */
                _framebuffer.bind();
                /* Clear framebuffer */
                _framebuffer.clear(Magnum::GL::FramebufferClear::Color | Magnum::GL::FramebufferClear::Depth);
                
                arrayResize(*_boxInstanceData, 0);
	            arrayResize(*_circleInstanceData, 0);
                /* Draw with main camera */
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

                _image = _framebuffer.read(_framebuffer.viewport(), {Magnum::PixelFormat::RGB8Unorm});
		video();
            }
        } // namespace magnum
    } // namespace gui
} // namespace robot_dart

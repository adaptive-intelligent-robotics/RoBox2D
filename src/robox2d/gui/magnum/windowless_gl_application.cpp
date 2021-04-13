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
            WindowlessGLApplication::WindowlessGLApplication(int argc, char** argv, robox2d::Simu* simu, const GraphicsConfiguration& configuration)
                : BaseApplication(configuration),
                  Magnum::Platform::WindowlessApplication({argc, argv}, Magnum::NoCreate),
                  _bg_color(configuration.bg_color[0],
                            configuration.bg_color[1],
                            configuration.bg_color[2],
                            configuration.bg_color[3])
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
                int w = configuration.width, h = configuration.height;
                _framebuffer = Magnum::GL::Framebuffer({{}, {w, h}});
		_color = Magnum::GL::Renderbuffer();
                _depth = Magnum::GL::Renderbuffer();
                _color.setStorage(Magnum::GL::RenderbufferFormat::RGBA8, {w, h});
                _depth.setStorage(Magnum::GL::RenderbufferFormat::DepthComponent, {w, h});

                _format = Magnum::PixelFormat::RGBA8Unorm;

                _framebuffer.attachRenderbuffer(
                    Magnum::GL::Framebuffer::ColorAttachment(0), _color);
                _framebuffer.attachRenderbuffer(
                    Magnum::GL::Framebuffer::BufferAttachment::Depth, _depth);

                /* Initialize Box2d world */
                init(simu, configuration);
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

                /* Change clear color to _bg_color */
                Magnum::GL::Renderer::setClearColor(_bg_color);

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
} // namespace robox2d

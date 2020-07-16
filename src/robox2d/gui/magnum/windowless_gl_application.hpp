#ifndef ROBOT_DART_GUI_MAGNUM_GLX_APPLICATION_HPP
#define ROBOT_DART_GUI_MAGNUM_GLX_APPLICATION_HPP

#include <Magnum/GL/Renderbuffer.h>
#include <Magnum/PixelFormat.h>
#include <Magnum/GL/Framebuffer.h>
#include <robox2d/gui/magnum/base_application.hpp>


namespace robox2d {
    namespace gui {
        namespace magnum {
            class WindowlessGLApplication : public BaseApplication, public Magnum::Platform::WindowlessApplication {
            public:
                explicit WindowlessGLApplication(int argc, char** argv, const std::shared_ptr<b2World>& world, size_t width, size_t height, const std::string& title = "DART");
                ~WindowlessGLApplication();

                void render() override;

            protected:
                Magnum::GL::Framebuffer _framebuffer{Magnum::NoCreate};
                Magnum::PixelFormat _format;
                Magnum::GL::Renderbuffer _color, _depth;

                // size_t _index = 0;

                virtual int exec() override { return 0; }
            };
        } // namespace magnum
    } // namespace gui
} // namespace robot_dart

#endif
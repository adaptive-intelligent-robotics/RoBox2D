#ifndef ROBOX2D_GUI_MAGNUM_GLX_APPLICATION_HPP
#define ROBOX2D_GUI_MAGNUM_GLX_APPLICATION_HPP

#include <Magnum/GL/Renderbuffer.h>
#include <Magnum/PixelFormat.h>
#include <robox2d/gui/magnum/base_application.hpp>
#include <Magnum/GL/Framebuffer.h>

#include "robox2d/simu.hpp"

namespace robox2d {
    class Simu;

    namespace gui {
        namespace magnum {
            class WindowlessGLApplication : public BaseApplication, public Magnum::Platform::WindowlessApplication {
            public:
                explicit WindowlessGLApplication(int argc, char** argv, robox2d::Simu* simu, const GraphicsConfiguration& configuration = GraphicsConfiguration());
                ~WindowlessGLApplication();

                void render() override;

            protected:
                Magnum::GL::Framebuffer _framebuffer{Magnum::NoCreate};
                Magnum::PixelFormat _format;
                Magnum::GL::Renderbuffer _color{Magnum::NoCreate}, _depth{Magnum::NoCreate};

                // size_t _index = 0;

                virtual int exec() override { return 0; }
            };
        } // namespace magnum
    } // namespace gui
} // namespace robox2d

#endif

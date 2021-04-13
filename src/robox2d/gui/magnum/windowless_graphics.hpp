#ifndef ROBOX2D_WINDOWLESSGRAPHICS_HPP
#define ROBOX2D_WINDOWLESSGRAPHICS_HPP

#include "robox2d/gui/magnum/base_graphics.hpp"
#include "robox2d/gui/magnum/windowless_gl_application.hpp"

namespace robox2d {
  namespace gui {
    namespace magnum {
      class WindowlessGraphics : public BaseGraphics<WindowlessGLApplication>
      {
      public:
        WindowlessGraphics(const GraphicsConfiguration& configuration = default_configuration())
          : BaseGraphics<WindowlessGLApplication>(configuration)
        {}
        ~WindowlessGraphics() {}

        void set_simu(Simu* simu) override;

        static GraphicsConfiguration default_configuration();
      };
    } // namespace magnum
  }   // namespace gui
} // namespace robox2d

class WindowlessGraphics
{};

#endif // ROBOX2D_WINDOWLESSGRAPHICS_HPP

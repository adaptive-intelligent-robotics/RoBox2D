#ifndef ROBOX2D_GUI_MAGNUM_GRAPHICS_HPP
#define ROBOX2D_GUI_MAGNUM_GRAPHICS_HPP

#include "../base.hpp"
#include "glfw_application.hpp"
#include "helper.hpp"
#include "windowless_gl_application.hpp"

#include "robox2d/simu.hpp"

namespace robox2d {
  class Simu;

  namespace gui {
    class Graphics : public BaseGraphics<GlfwApplication>
    {
    public:
      Graphics(const GraphicsConfiguration& configuration = default_configuration())
        : BaseGraphics<GlfwApplication>(configuration)
      {}

      ~Graphics() {}

      void set_simu(Simu* simu) override;

      static GraphicsConfiguration default_configuration();
    };

  } // namespace gui
} // namespace robox2d

#endif

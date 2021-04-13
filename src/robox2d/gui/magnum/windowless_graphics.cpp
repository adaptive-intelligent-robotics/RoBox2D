#include "robox2d/gui/magnum/windowless_graphics.hpp"

namespace robox2d {
  namespace gui {
    namespace magnum {
      void
      WindowlessGraphics::set_simu(RobotDARTSimu* simu)
      {
        BaseGraphics<WindowlessGLApplication>::set_simu(simu);
        // we should not synchronize by default if we want windowless graphics (usually used only for sensors)
        simu->set_sync(false);
      }

      GraphicsConfiguration
      WindowlessGraphics::default_configuration()
      {
        GraphicsConfiguration config;

        return config;
      }
    } // namespace magnum
  }   // namespace gui
} // namespace robox2d
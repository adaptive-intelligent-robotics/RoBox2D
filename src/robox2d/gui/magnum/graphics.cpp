#include "robox2d/gui/magnum/graphics.hpp"

namespace robox2d {
  namespace gui {

    void
    Graphics::set_simu(RobotDARTSimu* simu)
    {
      BaseGraphics<GlfwApplication>::set_simu(simu);
      // we synchronize by default if we have the graphics activated
      simu->set_sync(true);
      // enable summary text when graphics activated
      //        simu->enable_text_panel(true);
      //        simu->enable_status_bar(true);
    }

    GraphicsConfiguration
    Graphics::default_configuration()
    {
      return GraphicsConfiguration();
    }
  } // namespace gui
} // namespace robox2d
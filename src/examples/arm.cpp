#include <iostream>

#include <box2d/box2d.h>

#include <robox2d/simu.hpp>
#include <robox2d/robot.hpp>
#ifdef GRAPHIC
#include <robox2d/gui/magnum/graphics.hpp>
#endif

int main()
  {
    // simulating
    robox2d::Simu simu;
    simu.add_floor();

    auto rob = std::make_shared<robox2d::Robot>(simu.world());
    auto ctrl = std::make_shared<robox2d::control::BaseController>(2);
    rob->add_controller(ctrl);
    simu.add_robot(rob);

#ifdef GRAPHIC
    auto graphics = std::make_shared<robox2d::gui::Graphics<>>(simu.world());
    simu.set_graphics(graphics);
#endif

    simu.run(10.0);

    return 0;
}


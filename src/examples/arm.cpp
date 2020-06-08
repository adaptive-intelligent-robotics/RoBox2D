#include <iostream>

#include <box2d/box2d.h>

#include <robox2d/simu.hpp>
#include <robox2d/robot.hpp>
#include <robox2d/common.hpp>
#ifdef GRAPHIC
#include <robox2d/gui/magnum/graphics.hpp>
#endif



class Arm : public robox2d::Robot {
  public:
    
    Arm(std::shared_ptr<b2World> world){
      
      auto base = robox2d::common::createBody(world,{0.25f, 0.25f}, b2_staticBody,  {0.0f,0.0f,0.0f} );
      auto seg1 = robox2d::common::createBody(world,{0.5f , 0.1f }, b2_dynamicBody, {0.5f,0.0f,0.0f} );
      auto seg2 = robox2d::common::createBody(world,{0.5f , 0.1f }, b2_dynamicBody, {1.5f,0.0f,0.0f} );
      
      b2Vec2 anchor = base->GetWorldCenter();
      //auto j1 =
      this->_servos.push_back(std::make_shared<robox2d::common::Servo>(world,base, seg1, anchor));
      //auto j2 =
      anchor = seg1->GetWorldCenter() + b2Vec2(0.5f , 0.0f);
      this->_servos.push_back(std::make_shared<robox2d::common::Servo>(world,seg1, seg2,  anchor));
      
    }
};



int main()
  {
    // simulating
    robox2d::Simu simu;
    simu.add_floor();

    auto rob = std::make_shared<Arm>(simu.world());
    //auto ctrl = std::make_shared<robox2d::control::BaseController>(2);
    //rob->add_controller(ctrl);
    simu.add_robot(rob);

#ifdef GRAPHIC
    auto graphics = std::make_shared<robox2d::gui::Graphics<>>(simu.world());
    simu.set_graphics(graphics);
#endif

    simu.run(10.0);

    return 0;
}


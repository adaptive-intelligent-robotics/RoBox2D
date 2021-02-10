#include <iostream>

#include <box2d/box2d.h>

#include <robox2d/simu.hpp>
#include <robox2d/robot.hpp>
#include <robox2d/common.hpp>
#include <robox2d/actuator.hpp>
#ifdef GRAPHIC
#include <robox2d/gui/magnum/graphics.hpp>
#endif



class Car : public robox2d::Robot {
public:
  
  Car(std::shared_ptr<b2World> world){

    float hull_size = 0.1;
   
    _hull = robox2d::common::createBox( world,{hull_size*0.5f, hull_size}, b2_dynamicBody,  {0.0f,0.0f,0.0f} );
    
    // body will always represent the body created in the previous iteration

    
    for(size_t i =0; i < 2; i++)
      {

	
	b2Vec2 anchor = _hull->GetWorldCenter() + (2*i-1.0)*b2Vec2({hull_size*0.75f,0});
	
	b2Body* wheel = robox2d::common::createCircle( world, hull_size*0.25f, b2_dynamicBody, {anchor.x,anchor.y,0.0f} );
	
	this->_actuators.push_back(std::make_shared<robox2d::actuator::WheelTraction>(wheel));
	robox2d::common::createWeldJoint( world, _hull, wheel, anchor);
      
      }

    
    
  }
  
  b2Vec2 get_hull_pos(){return _hull->GetWorldCenter(); }
  
private:
  b2Body* _hull;
};


int main()
  {
    // simulating
    robox2d::Simu* simu = new robox2d::Simu();
    simu->add_floor();
    Eigen::VectorXd ctrl_pos(2);
    ctrl_pos[0]=0.5;
    ctrl_pos[1]=0.25;
    
    auto rob = std::make_shared<Car>(simu->world());
    
    auto ctrl = std::make_shared<robox2d::control::ConstantPos>(ctrl_pos);
    rob->add_controller(ctrl);
    simu->add_robot(rob);

#ifdef GRAPHIC
    auto graphics = std::make_shared<robox2d::gui::Graphics<>>(simu);
    simu->set_graphics(graphics);
#endif

    simu->run(10.0);

#ifdef GRAPHIC
    auto gimage = graphics->image();
    robox2d::gui::save_png_image("camera-main.png", gimage);
#endif

    std::cout<<"Hull position:" << rob->get_hull_pos().x<<"  "<<rob->get_hull_pos().y<<std::endl;
    return 0;
}


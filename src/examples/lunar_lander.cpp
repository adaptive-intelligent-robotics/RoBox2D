
#include <box2d/box2d.h>

#include <robox2d/simu.hpp>
#include <robox2d/robot.hpp>
#include <robox2d/common.hpp>
#include <robox2d/actuator.hpp>
#ifdef GRAPHIC
#include <robox2d/gui/magnum/graphics.hpp>
#endif



class LunarLander : public robox2d::Robot {
public:
  
  LunarLander(std::shared_ptr<b2World> world){

    float hull_size = 0.1;
   
    _hull = robox2d::common::createBox( world,{hull_size, hull_size*0.75f}, b2_dynamicBody,  {0.0f,0.0f,0.0f} );
    robox2d::common::createBox( world,{10.0f, 0.5f}, b2_staticBody,  {0.0f,-1.0f,0.0f} );// ground
    
    // body will always represent the body created in the previous iteration

    
    for(size_t i =0; i < 4; i++)
      {
	b2Vec2 anchor = _hull->GetWorldCenter();
	b2Vec2 direction({0,0});
	float radius =  hull_size*0.25f;
	switch(i) {
	case 0:	  // bottom left engine
	  anchor += b2Vec2({-hull_size*0.8f,-(hull_size*0.75f+radius)});
	  direction+=b2Vec2({0,1});
	  break;
	case 1:   // bottom right engine
	  anchor += b2Vec2({hull_size*0.8f,-(hull_size*0.75f+radius)});
	  direction+=b2Vec2({0,1});
	  break;
	case 2:   // top left
	  anchor += b2Vec2({-(hull_size+radius),+hull_size*0.4f});
	  direction+=b2Vec2({1,0});
	  break;
	case 3:   // top right
	  anchor += b2Vec2({(hull_size+radius),+hull_size*0.4f});
	  direction+=b2Vec2({-1,0});
	  break;
	default:
	  assert(0);  // should not be here. 
	}
	
	
	b2Body* reactor = robox2d::common::createCircle( world, radius, b2_dynamicBody, {anchor.x,anchor.y,0.0f});
	
	this->_actuators.push_back(std::make_shared<robox2d::actuator::PonctualForce>(reactor, b2Vec2({0,0}), direction)); //{0,0} is for the center of mass
	robox2d::common::createWeldJoint( world, _hull, reactor, anchor);
      }

    
    
  }
  
  b2Vec2 get_hull_pos(){return _hull->GetWorldCenter(); }
  
private:
  b2Body* _hull;
};


int main()
  {
    // simulating
    robox2d::Simu simu;
    simu.world()->SetGravity({0, -9.81});
    simu.add_floor();
    Eigen::VectorXd ctrl_pos(4);
    ctrl_pos[0]=0.19;
    ctrl_pos[1]=0.19;
    ctrl_pos[2]=0.000;
    ctrl_pos[3]=0.000;
    
    auto rob = std::make_shared<LunarLander>(simu.world());
    
    auto ctrl = std::make_shared<robox2d::control::ConstantPos>(ctrl_pos);
    rob->add_controller(ctrl);
    simu.add_robot(rob);

#ifdef GRAPHIC
    auto graphics = std::make_shared<robox2d::gui::Graphics<>>(simu.world());
    simu.set_graphics(graphics);
#endif

    simu.run(10.0);

#ifdef GRAPHIC
    auto gimage = graphics->image();
    robox2d::gui::save_png_image("camera-main.png", gimage);
#endif

    std::cout<<"Hull position:" << rob->get_hull_pos().x<<"  "<<rob->get_hull_pos().y<<std::endl;
    return 0;
}



#include <box2d/box2d.h>

#include <robox2d/simu.hpp>
#include <robox2d/robot.hpp>
#include <robox2d/common.hpp>
#include <robox2d/actuator.hpp>
#ifdef GRAPHIC
#include <robox2d/gui/magnum/base_graphics.hpp>
#endif



class LunarLander : public robox2d::Robot {
public:
  
  LunarLander(std::shared_ptr<b2World> world){

    float hull_size = 0.1;
   
    _hull = robox2d::common::createBox( world,{hull_size, hull_size*0.75f}, b2_dynamicBody,  {0.0f,0.0f,0.0f}, 1.0f );
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
	  anchor += b2Vec2({-(hull_size+radius),+hull_size*0.3f});
	  direction+=b2Vec2({1,0});
	  break;
	case 3:   // top right
	  anchor += b2Vec2({(hull_size+radius),+hull_size*0.3f});
	  direction+=b2Vec2({-1,0});
	  break;
	default:
	  assert(0);  // should not be here. 
	}
	
	
	robox2d::common::addBoxFixture( _hull, {radius*2.0f, radius*1.0f}, {anchor.x,anchor.y, 0.0f}, 0.0f); //REACTOR
	
	this->_actuators.push_back(std::make_shared<robox2d::actuator::PonctualForce>(_hull, anchor, direction)); 
	
      }

    
    
  }
  
  b2Vec2 get_hull_pos(){return _hull->GetWorldCenter(); }
  b2Vec2 get_hull_lin_vel(){return _hull->GetLinearVelocity(); }
  float get_hull_rot_vel(){return _hull->GetAngularVelocity(); }
  
private:
  b2Body* _hull;
};


class LanderController: public robox2d::control::BaseController {
    public:
      LanderController(): robox2d::control::BaseController(4){}
      
      Eigen::VectorXd commands(double t, robox2d::Robot* robot){
	auto rob = static_cast<LunarLander*>(robot);
	Eigen::VectorXd cmd=Eigen::VectorXd::Zero(this->_nb_dofs);

	
	
	std::cout<<rob->get_hull_lin_vel().y<<" "<<rob->get_hull_rot_vel()<<std::endl;

	if(std::abs(rob->get_hull_rot_vel())>0.001)
	  {
	   if(rob->get_hull_rot_vel() > 0)
	     cmd[2]+= std::min(std::max(std::abs(rob->get_hull_rot_vel())/50.0f, 0.0f), 0.01f);
	   else
	     cmd[3]+= std::min(std::max(std::abs(rob->get_hull_rot_vel())/50.0f, 0.0f), 0.01f);
	  }
	
	if(rob->get_hull_lin_vel().x < 0)
	  cmd[2]+= std::min(std::max(std::abs(rob->get_hull_lin_vel().x)/100.0f, 0.0f), 0.01f);
	else
	  cmd[3]+= std::min(std::max(std::abs(rob->get_hull_lin_vel().x)/100.0f, 0.0f), 0.01f);

	if(std::abs(rob->get_hull_lin_vel().Length())>0.001)
	  if(rob->get_hull_lin_vel().y<0)
	    {
	      cmd[0]= std::min(std::max(-rob->get_hull_lin_vel().y/400.0f, 0.0f), 0.02f);
	      cmd[1]= std::min(std::max(-rob->get_hull_lin_vel().y/200.0f, 0.0f), 0.02f); // simulated defect in one reactor
	    }
	return cmd;
      }
  
    };
    


int main()
  {
    
    // simulating
    robox2d::Simu simu;
    simu.world()->SetGravity({0, -9.81});
    simu.add_floor();
    /* Eigen::VectorXd ctrl_pos(4);
    ctrl_pos[0]=0.002;
    ctrl_pos[1]=0.002;
    ctrl_pos[2]= 0.0001;
    ctrl_pos[3]= 0.00;
    */
    
    
    auto rob = std::make_shared<LunarLander>(simu.world());
    
    //auto ctrl = std::make_shared<robox2d::control::ConstantPos>(ctrl_pos);
    auto ctrl = std::make_shared<LanderController>();
    rob->add_controller(ctrl);
    simu.add_robot(rob);

#ifdef GRAPHIC
    auto graphics = std::make_shared<robox2d::gui::Graphics>();
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


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

    size_t nb_joints=8;
    float arm_length=1.0;
    float seg_length = arm_length / (float) nb_joints;
    
    b2Body* body = robox2d::common::createBox( world,{arm_length*0.025f, arm_length*0.025f}, b2_staticBody,  {0.0f,0.0f,0.0f} );
    b2Vec2 anchor = body->GetWorldCenter();
    // body will always represent the body created in the previous iteration

    
    for(size_t i =0; i < nb_joints; i++)
      {
	_end_effector = robox2d::common::createBox( world,{seg_length*0.5f , arm_length*0.01f }, b2_dynamicBody, {(0.5f+i)*seg_length,0.0f,0.0f} );
      this->_servos.push_back(std::make_shared<robox2d::common::Servo>(world,body, _end_effector, anchor));

      body=_end_effector;
      anchor = _end_effector->GetWorldCenter() + b2Vec2(seg_length*0.5 , 0.0f);
      }

    b2Body* ball = robox2d::common::createCircle( world,0.025f, b2_dynamicBody,  {0.5f,0.5f,0.0f} );
    
  }
  
  b2Vec2 get_end_effector_pos(){return _end_effector->GetWorldCenter(); }
  
private:
  b2Body* _end_effector;
};


int main()
  {
    // simulating
    robox2d::Simu simu;
    simu.add_floor();
    Eigen::VectorXd ctrl_pos(8);
    for(size_t i=0;i< (size_t)ctrl_pos.size();i++)
      ctrl_pos[i]=0.5*M_PI/(1+(float) i);
    
    auto rob = std::make_shared<Arm>(simu.world());
    auto ctrl = std::make_shared<robox2d::control::ConstantPos>(ctrl_pos);
    rob->add_controller(ctrl);
    simu.add_robot(rob);

#ifdef GRAPHIC
    auto graphics = std::make_shared<robox2d::gui::Graphics<>>(simu.world());
    simu.set_graphics(graphics);
#endif

    simu.run(10.0);
    std::cout<<"End effector position:" << rob->get_end_effector_pos().x<<"  "<<rob->get_end_effector_pos().y<<std::endl;
    return 0;
}


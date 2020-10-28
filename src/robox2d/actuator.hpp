#ifndef ROBOX2D_ACTUATOR_HPP
#define ROBOX2D_ACTUATOR_HPP


#include <algorithm> 
#include <memory>
#include <vector>
#include <iostream>


#include <box2d/box2d.h>


namespace robox2d {
  namespace actuator {

    /**
     * @brief Actuator is an abstract class for actuators (servos, ...) 
     * 
     * This class gives the minimal definition of an actuator. 
     * 
     */
    class Actuator{
    public:
            
      virtual void set_input(double input){_input=input;};
      
      virtual void update()=0;
      
    protected:
      double _input;
    };

    


    
    /**
     * @brief Servo is a wrapper for dynamic joints and motors combined.
     * 
     * This class allows for easy manipulation of joint movements.
     * 
     */
    class Servo : public Actuator{
    public:
      Servo( std::shared_ptr<b2World> world, b2Body* bodyA, b2Body* bodyB,  const b2Vec2 & anchor, double gain = 0.3);
      
      void update();
      
      b2RevoluteJoint* get_joint();
      const b2RevoluteJoint* get_joint()const;
    private:
      b2RevoluteJoint* _joint;
      double _gain;
      double _target_pos;
    };



    /**
     * @brief PonctualForce applies a ponctual force on body at the anchor location in following the provided direction.
     * 
     * This class can be used to implement propeller or reactors. The input defines the amplitude of the force. 
     * 
     */
    class PonctualForce : public Actuator{
    public:
      PonctualForce( b2Body* body,  const b2Vec2 & anchor, const b2Vec2 & direction):_body(body), _anchor(anchor), _direction(direction), _force(0.0){ _direction.Normalize();}
      
            
      void update();
      
    private:
      b2Body* _body;
      b2Vec2 _anchor;
      b2Vec2 _direction;
      double _force;
    };

    /**
     * @brief WheelTraction applies a ponctual force on body at the anchor location in following the provided direction.
     * 
     * This class can be used to implement the friction and the respective traction created by a wheel on the ground (in a top-down setting). The input defines wheel gas and the force is computed following the same dynamics than in the car of OpenAI GYM.
     * 
     */
    class WheelTraction : public Actuator{
    public:
      WheelTraction( b2Body* body):
	_body(body),
	_gas(0.0), 
	_omega(0.0)
      {      }
      
            
      void update();
      
    private:
      
      b2Body* _body;
      float _gas; // if gas is negative, then it is braking      
      float _omega; // angular velocity

      const float dt=0.01;
      const float size=0.001;
      const float engine_power =100000000*size*size;
      const float friction_limit = 1000000*size*size;
      const float wheel_moment_of_inertia = 4000*size*size;
      const float wheel_rad = 27*size;
      
    };


  }
}

#endif

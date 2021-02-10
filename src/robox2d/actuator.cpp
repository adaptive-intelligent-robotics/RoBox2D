#include <box2d/box2d.h>

#include <iostream>
#include "actuator.hpp"

namespace robox2d {
  namespace actuator {

/**
     * @brief Construct a new Servo object. Servo is a wrapper around a joint.
     * 
     * Servo is used to control how the joints move. Joints can move between -pi and pi rad.
     * Maximum motor torque is 1.
     * 
     * @param  world World to place the servo in.
     * @param  bodyA Body the servo will be attached to.
     * @param  bodyB Body the servo will be attached to.
     * @param  anchor Position the joint will be at when initializing the situation (can move during the simulation).
     * @param  gain factor for the movement of the Proportional Controller for the joint.
     */
    Servo::Servo( std::shared_ptr<b2World> world, b2Body* bodyA, b2Body* bodyB,  const b2Vec2 & anchor, double gain):_gain(gain), _target_pos(0.0)
    {
      b2RevoluteJointDef jointDef;
      jointDef.Initialize(bodyA, bodyB, anchor);
      jointDef.lowerAngle = -1.0f * b2_pi;
      jointDef.upperAngle = 1.0f * b2_pi;
      jointDef.enableLimit = true;
      jointDef.maxMotorTorque = 1.0f;
      jointDef.motorSpeed = 0.0f;
      jointDef.enableMotor = true;
      _joint = (b2RevoluteJoint*)world->CreateJoint(&jointDef); 
    }
    
    
    void Servo::update(){
      _joint->SetMotorSpeed(_gain * (_input - _joint->GetJointAngle() ));
    }




    
    
    void PonctualForce::update(){
      auto f = _input*_body->GetWorldVector(_direction);
      _body->ApplyLinearImpulse(f , _body->GetWorldPoint(_anchor), true);
    }

    
        
    void WheelTraction::update(){
      
      
      // gas managment 
      float gas_diff = _input - _gas;
      if (gas_diff > 0.1)
	gas_diff = 0.1; // gradually increase
      if (gas_diff < -0.1)
	gas_diff = -0.1; // gradually decrease
      _gas += gas_diff;
      

      // get velocities of the wheel
      auto forw = _body->GetWorldVector( {0,1} );
      auto side = _body->GetWorldVector( {1,0} );
      auto v = _body->GetLinearVelocity();
      float vf = forw.x*v.x + forw.y*v.y; // forward speed
      float vs = side.x*v.x + side.y*v.y; // side speed

      // update wheel rot speed based on gas
      _omega += dt*engine_power*_gas/wheel_moment_of_inertia/(std::abs(_omega)+5.0);

      // compute force applied by the wheel
      float vr = _omega*wheel_rad; // rotating wheel speed
      float f_force = -vf + vr; // force direction is direction of speed difference
      float p_force = -vs;

      f_force *= 205000*size*size;
      p_force *= 205000*size*size;
      
      // cap the force based on friction_limit
      float force = std::sqrt(f_force*f_force + p_force*p_force);
      if (std::abs(force) > friction_limit)
	{
	  f_force /= force;
	  p_force /= force;
	  force = friction_limit; // Correct physics here
	  f_force *= force;
	  p_force *= force;
	}

      // update the wheel rot speed based on force
      _omega -= dt*f_force*wheel_rad/wheel_moment_of_inertia;

      //apply force
      b2Vec2 force_vec= { p_force*side.x + f_force*forw.x,   p_force*side.y + f_force*forw.y};
      
      _body->ApplyForceToCenter( force_vec,  true);
    }

  }
}

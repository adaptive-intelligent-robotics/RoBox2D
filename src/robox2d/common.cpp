#include <box2d/box2d.h>

#include "common.hpp"

namespace robox2d {
  namespace common {
    
    /**
     * @brief Create a rectangular Box object.
     * 
     * Creates a box with given values. Friction is 0.8 by default.
     * 
     * @param  world            World to place the box in.
     * @param  halfSize         Array which contains half width and half height of the box to be created.
     * @param  type             Type of box to place (static, dynamic, kinematic).
     * @param  transformation   Position and angle at which to place the object in the world.
     * @param  density          Density of the Box.
     * @return b2Body*          Body that was created.
     */
    b2Body* createBox( std::shared_ptr<b2World> world, const b2Vec2& halfSize, const b2BodyType type, const b2Vec3& transformation, const float density) {
      b2BodyDef bodyDefinition;
      bodyDefinition.position.Set(transformation.x, transformation.y);
      bodyDefinition.angle = transformation.z;
      bodyDefinition.type = type;
      b2Body* body = world->CreateBody(&bodyDefinition);
      
      b2PolygonShape shape;
      shape.SetAsBox(halfSize.x, halfSize.y);
      
      b2FixtureDef fixture;
      fixture.friction = 0.8f;
      fixture.density = density;
      fixture.shape = &shape;
      body->CreateFixture(&fixture);
      
      return body;
    }
    
    /**
     * @brief Create a Circle object.
     * 
     * Creates a circle with given values. Friction is 0.8 by default.
     * 
     * @param  world            World to place the circle in.
     * @param  radius           Radius of the circle to be created.
     * @param  type             Type of circle to place (static, dynamic, kinematic).
     * @param  transformation   Position and angle at which to place the object in the world.
     * @param  density          Density of the circle.
     * @return b2Body*          Body that was created.
     */
    b2Body* createCircle( std::shared_ptr<b2World> world, const float radius, const b2BodyType type, const b2Vec3& transformation, const float density) {
      b2BodyDef bodyDefinition;
      bodyDefinition.position.Set(transformation.x, transformation.y);
      bodyDefinition.angle = transformation.z;
      bodyDefinition.type = type;
      b2Body* body = world->CreateBody(&bodyDefinition);
      
      b2CircleShape shape;
      shape.m_radius=radius;
      
      b2FixtureDef fixture;
      fixture.friction = 0.8f;
      fixture.density = density;
      fixture.shape = &shape;
      body->CreateFixture(&fixture);
      
      return body;
    }

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
    
    void Servo::set_target_pos(double pos){_target_pos = pos;}
    
    void Servo::update(){
      _joint->SetMotorSpeed(_gain * (_target_pos - _joint->GetJointAngle() ));
    }

    /**
     * @brief Create a Weld Joint, weld joints are joints that do not move.
     * 
     * @param  world World to place the joint in.
     * @param  bodyA Body to attach the joint to.
     * @param  bodyB Body to attach the joint to.
     * @param  anchor Starting anchor point for the joint.
     * @return b2WeldJoint* pointer to the joint.
     */
    b2WeldJoint* createWeldJoint( std::shared_ptr<b2World> world, b2Body* bodyA, b2Body* bodyB,  const b2Vec2 & anchor){
      b2WeldJointDef jointDef;
      jointDef.Initialize(bodyA, bodyB, anchor);
      return (b2WeldJoint*)world->CreateJoint(&jointDef);
    }

     
  }
}

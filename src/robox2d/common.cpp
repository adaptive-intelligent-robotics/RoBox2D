
#include <box2d/box2d.h>

#include "common.hpp"

namespace robox2d {
  namespace common {
    
    b2Body* createBody( std::shared_ptr<b2World> world, const b2Vec2& halfSize, const b2BodyType type, const b2Vec3& transformation, const float density) {
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


    Servo::Servo( std::shared_ptr<b2World> world, b2Body* bodyA, b2Body* bodyB,  const b2Vec2 & anchor, double gain):_gain(gain), _target_pos(0.0)
      {
	b2RevoluteJointDef jointDef;
	jointDef.Initialize(bodyA, bodyB, anchor);
	jointDef.lowerAngle = -1.0f * b2_pi;
	jointDef.upperAngle = 1.0f * b2_pi;
	jointDef.enableLimit = false;
	jointDef.maxMotorTorque = 10.0f;
	jointDef.motorSpeed = 0.0f;
	jointDef.enableMotor = true;
	_joint = (b2RevoluteJoint*)world->CreateJoint(&jointDef); 
      }

    void Servo::set_target_pos(double pos){_target_pos = pos;}

    void Servo::update(){
	_joint->SetMotorSpeed(_gain * (_target_pos - _joint->GetJointAngle() ));
      }

    
     
  }
}

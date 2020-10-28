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
      fixture.restitution = 0.8f;
      fixture.shape = &shape;
      body->CreateFixture(&fixture);
      
      return body;
    }

    b2Body* createRoom( std::shared_ptr<b2World> world, const b2Vec2& roomDim){
      b2BodyDef roomDefinition;
      roomDefinition.position.Set(0.f, 0.f);
      roomDefinition.type = b2_staticBody;
      b2Body* body = world->CreateBody(&roomDefinition);
      
      b2PolygonShape wall;

      b2FixtureDef fixture;
      fixture.shape = &wall;
      fixture.friction = 0.8f;

      wall.SetAsBox(roomDim.x / 2, 0.01f, {roomDim.x / 2, roomDim.y}, 0.f);
      body->CreateFixture(&fixture);
      wall.SetAsBox(roomDim.x / 2, 0.01f, {roomDim.x / 2, 0.f}, 0.f);
      body->CreateFixture(&fixture);
      wall.SetAsBox(0.01f, roomDim.y / 2, {0.f, roomDim.y / 2}, 0.f);
      body->CreateFixture(&fixture);
      wall.SetAsBox(0.01f, roomDim.y / 2, {roomDim.x, roomDim.y / 2}, 0.f);
      body->CreateFixture(&fixture);
      return body;
    }



    void addBoxFixture(b2Body* body, const b2Vec2& halfSize, const b2Vec3& transformation, const float density) {
      b2PolygonShape shape;
      shape.SetAsBox(halfSize.x, halfSize.y, {transformation.x, transformation.y}, transformation.z);

      b2FixtureDef fixture;
      fixture.friction = 0.8f;
      fixture.density = density;
      fixture.shape = &shape;
      body->CreateFixture(&fixture);
      
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

    void addCircleFixture(b2Body* body, const float radius, const b2Vec3& transformation, const float density) {
      b2CircleShape shape;
      shape.m_radius = radius;
      shape.m_p = {transformation.x, transformation.y};

      b2FixtureDef fixture;
      fixture.friction = 0.8f;
      fixture.density = density;
      fixture.shape = &shape;
      body->CreateFixture(&fixture);
      
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

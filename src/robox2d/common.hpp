#ifndef ROBOX2D_COMMON_HPP
#define ROBOX2D_COMMON_HPP


#include <algorithm> 
#include <memory>
#include <vector>


#include <box2d/box2d.h>


namespace robox2d {
  namespace common {

    b2Body* createBox( std::shared_ptr<b2World> world, const b2Vec2& halfSize, const b2BodyType type, const b2Vec3& transformation, const float density = 1.0f, const float friction = 0.8f, const float restitution = 0.f);
    b2Body* createCircle( std::shared_ptr<b2World> world, const float radius, const b2BodyType type, const b2Vec3& transformation, const float density = 1.0f, const float friction = 0.8f, const float restitution = 0.f);

    void addBoxFixture(b2Body* body, const b2Vec2& halfSize, const b2Vec3& transformation, const float density, const float friction = 0.8f, const float restitution = 0.f);
    void addCircleFixture(b2Body* body, const float radius, const b2Vec3& transformation, const float density, const float friction = 0.8f, const float restitution = 0.f);

    b2WeldJoint* createWeldJoint( std::shared_ptr<b2World> world, b2Body* bodyA, b2Body* bodyB,  const b2Vec2 & anchor);
  }
}

#endif

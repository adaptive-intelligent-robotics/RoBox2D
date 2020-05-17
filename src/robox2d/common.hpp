#ifndef ROBOX2D_COMMON_HPP
#define ROBOX2D_COMMON_HPP


#include <algorithm> 
#include <memory>
#include <vector>


#include <box2d/box2d.h>


namespace robox2d {
  namespace common {

    b2Body* createBody( std::shared_ptr<b2World> world, const b2Vec2& halfSize, const b2BodyType type, const b2Vec3& transformation, const float density = 1.0f);

    b2RevoluteJoint* createServo( std::shared_ptr<b2World> world, b2Body* bodyA, b2Body* bodyB,  const b2Vec2 & anchor);
       
  }
}

#endif

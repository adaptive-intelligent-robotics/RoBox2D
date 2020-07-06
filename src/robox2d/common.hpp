#ifndef ROBOX2D_COMMON_HPP
#define ROBOX2D_COMMON_HPP


#include <algorithm> 
#include <memory>
#include <vector>


#include <box2d/box2d.h>


namespace robox2d {
  namespace common {

    b2Body* createBox( std::shared_ptr<b2World> world, const b2Vec2& halfSize, const b2BodyType type, const b2Vec3& transformation, const float density = 1.0f);
    b2Body* createCircle( std::shared_ptr<b2World> world, const float radius, const b2BodyType type, const b2Vec3& transformation, const float density = 1.0f);

    b2RevoluteJoint* createServo( std::shared_ptr<b2World> world, b2Body* bodyA, b2Body* bodyB,  const b2Vec2 & anchor);
    b2WeldJoint* createWeldJoint( std::shared_ptr<b2World> world, b2Body* bodyA, b2Body* bodyB,  const b2Vec2 & anchor);
    /**
     * @brief Servo is a wrapper for dynamic joints and motors combined.
     * 
     * This class allows for easy manipulation of joint movements.
     * 
     */
    class Servo{
    public:
      Servo( std::shared_ptr<b2World> world, b2Body* bodyA, b2Body* bodyB,  const b2Vec2 & anchor, double gain = 0.3);
      
      void set_target_pos(double pos);
      
      void update();
      
      b2RevoluteJoint* get_joint();
      const b2RevoluteJoint* get_joint()const;
    private:
      b2RevoluteJoint* _joint;
      double _gain;
      double _target_pos;
    };

    

  }
}

#endif

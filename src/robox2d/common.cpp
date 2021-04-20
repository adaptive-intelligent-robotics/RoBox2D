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
     * @param  friction         The friction coefficient (usually between 0 and 1).
     * @param  restitution      The restitution (elasticity) coefficient, usually between 0 and 1.
     * @return b2Body*          Body that was created.
     */
    b2Body* createBox(std::shared_ptr<b2World> world,
                      const b2Vec2& halfSize,
                      const b2BodyType type,
                      const b2Vec3& transformation,
                      const float density,
                      const float friction,
                      const float restitution)
    {
      b2BodyDef bodyDefinition;
      bodyDefinition.position.Set(transformation.x, transformation.y);
      bodyDefinition.angle = transformation.z;
      bodyDefinition.type = type;
      b2Body* body = world->CreateBody(&bodyDefinition);

      b2PolygonShape shape;
      shape.SetAsBox(halfSize.x, halfSize.y);

      b2FixtureDef fixture;
      fixture.friction = friction;
      fixture.density = density;
      fixture.shape = &shape;
      fixture.restitution = restitution;
      body->CreateFixture(&fixture);

      return body;
    }

    b2Fixture* addBoxFixture(b2Body* body,
                       const b2Vec2& halfSize,
                       const b2Vec3& transformation,
                       const float density,
                       const float friction,
                       const float restitution)
    {
      b2PolygonShape shape;
      shape.SetAsBox(halfSize.x, halfSize.y, {transformation.x, transformation.y}, transformation.z);

      b2FixtureDef fixture_def;
      fixture_def.friction = friction;
      fixture_def.density = density;
      fixture_def.shape = &shape;
      fixture_def.restitution = restitution;

      b2Fixture* fixture = body->CreateFixture(&fixture_def);

      return fixture;
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
     * @param  friction         The friction coefficient (usually between 0 and 1).
     * @param  restitution      The restitution (elasticity) coefficient, usually between 0 and 1.
     * @return b2Body*          Body that was created.
     */
    b2Body* createCircle(std::shared_ptr<b2World> world,
                         const float radius,
                         const b2BodyType type,
                         const b2Vec3& transformation,
                         const float density,
                         const float friction,
                         const float restitution) {
      b2BodyDef bodyDefinition;
      bodyDefinition.position.Set(transformation.x, transformation.y);
      bodyDefinition.angle = transformation.z;
      bodyDefinition.type = type;
      b2Body* body = world->CreateBody(&bodyDefinition);

      b2CircleShape shape;
      shape.m_radius=radius;

      b2FixtureDef fixture;
      fixture.friction = friction;
      fixture.density = density;
      fixture.shape = &shape;
      fixture.restitution = restitution;
      body->CreateFixture(&fixture);

      return body;
    }

      b2Fixture* addCircleFixture(b2Body* body,
                          const float radius,
                          const b2Vec3& transformation,
                          const float density,
                          const float friction,
                          const float restitution) {
      b2CircleShape shape;
      shape.m_radius = radius;
      shape.m_p = {transformation.x, transformation.y};

      b2FixtureDef fixture_def;
      fixture_def.friction = friction;
      fixture_def.density = density;
      fixture_def.shape = &shape;
      fixture_def.restitution = restitution;

      b2Fixture* fixture = body->CreateFixture(&fixture_def);

      return fixture;
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

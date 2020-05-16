#include <iostream>

#include <box2d/box2d.h>

#include <Magnum/Platform/GlfwApplication.h>

#include <Corrade/Containers/GrowableArray.h>
#include <Corrade/Utility/Arguments.h>
#include <Magnum/GL/Context.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Buffer.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/Math/ConfigurationValue.h>
#include <Magnum/Math/DualComplex.h>
#include <Magnum/MeshTools/Compile.h>

#include <Magnum/Primitives/Square.h>
#include <Magnum/SceneGraph/Camera.h>
#include <Magnum/SceneGraph/Drawable.h>
#include <Magnum/SceneGraph/TranslationRotationScalingTransformation2D.h>
#include <Magnum/SceneGraph/Scene.h>
#include <Magnum/Shaders/Flat.h>
#include <Magnum/Trade/MeshData.h>

namespace Magnum { namespace Examples {

    typedef SceneGraph::Object<SceneGraph::TranslationRotationScalingTransformation2D> Object2D;
    typedef SceneGraph::Scene<SceneGraph::TranslationRotationScalingTransformation2D> Scene2D;

    using namespace Math::Literals;

    struct InstanceData {
      Matrix3 transformation;
      Color3 color;
    };

    class Box2DExample: public Platform::Application {
    public:
      explicit Box2DExample(const Arguments& arguments);

    private:
      void drawEvent() override;
      void mousePressEvent(MouseEvent& event) override;

      b2Body* createBody(Object2D& object, const Vector2& size, b2BodyType type, const DualComplex& transformation, Float density = 1.0f);

      GL::Mesh _mesh{NoCreate};
      GL::Buffer _instanceBuffer{NoCreate};
      Shaders::Flat2D _shader{NoCreate};
      Containers::Array<InstanceData> _instanceData;

      Scene2D _scene;
      std::unique_ptr<Object2D> _cameraObject;
      std::unique_ptr<SceneGraph::Camera2D> _camera;
      SceneGraph::DrawableGroup2D _drawables;
      Containers::Optional<b2World> _world;
    };

    class BoxDrawable: public SceneGraph::Drawable2D {
    public:
      explicit BoxDrawable(Object2D& object, Containers::Array<InstanceData>& instanceData, const Color3& color, SceneGraph::DrawableGroup2D& drawables): SceneGraph::Drawable2D{object, &drawables}, _instanceData(instanceData), _color{color} {}

    private:
      void draw(const Matrix3& transformation, SceneGraph::Camera2D&) override {
	arrayAppend(_instanceData, Containers::InPlaceInit,
		    transformation, _color);
      }

      Containers::Array<InstanceData>& _instanceData;
      Color3 _color;
    };

    b2Body* Box2DExample::createBody(Object2D& object, const Vector2& halfSize, const b2BodyType type, const DualComplex& transformation, const Float density) {
      b2BodyDef bodyDefinition;
      bodyDefinition.position.Set(transformation.translation().x(), transformation.translation().y());
      bodyDefinition.angle = Float(transformation.rotation().angle());
      bodyDefinition.type = type;
      b2Body* body = _world->CreateBody(&bodyDefinition);

      b2PolygonShape shape;
      shape.SetAsBox(halfSize.x(), halfSize.y());

      b2FixtureDef fixture;
      fixture.friction = 0.8f;
      fixture.density = density;
      fixture.shape = &shape;
      body->CreateFixture(&fixture);

      body->SetUserData(&object);
      object.setScaling(halfSize);

      return body;
    }

    Box2DExample::Box2DExample(const Arguments& arguments): Platform::Application{arguments, NoCreate} {
      /* Make it possible for the user to have some fun */
      Utility::Arguments args;
      args.addOption("transformation", "1 0 0 0").setHelp("transformation", "initial pyramid transformation")
        .addSkippedPrefix("magnum", "engine-specific options")
        .parse(arguments.argc, arguments.argv);

      const DualComplex globalTransformation = args.value<DualComplex>("transformation").normalized();

      /* Try 8x MSAA, fall back to zero samples if not possible. Enable only 2x
	 MSAA if we have enough DPI. */
      {
        const Vector2 dpiScaling = this->dpiScaling({});
        Configuration conf;
        conf.setTitle("Magnum Box2D Example")
	  .setSize(conf.size(), dpiScaling);
        GLConfiguration glConf;
        glConf.setSampleCount(dpiScaling.max() < 2.0f ? 8 : 2);
        if(!tryCreate(conf, glConf))
	  create(conf, glConf.setSampleCount(0));
      }

      /* Configure camera */
      _cameraObject.reset(new Object2D{&_scene});
      _camera.reset(new SceneGraph::Camera2D{*_cameraObject});
      _camera->setAspectRatioPolicy(SceneGraph::AspectRatioPolicy::Extend)
        .setProjectionMatrix(Matrix3::projection({20.0f, 20.0f}))
        .setViewport(GL::defaultFramebuffer.viewport().size());

      /* Create the Box2D world with the usual gravity vector */
      _world.emplace(b2Vec2{0.0f, 0.0f});

      /* Create an instanced shader */
      _shader = Shaders::Flat2D{
        Shaders::Flat2D::Flag::VertexColor|
        Shaders::Flat2D::Flag::InstancedTransformation};

      /* Box mesh with an (initially empty) instance buffer */
      _mesh = MeshTools::compile(Primitives::squareSolid());
      _instanceBuffer = GL::Buffer{};
      _mesh.addVertexBufferInstanced(_instanceBuffer, 1, 0,
				     Shaders::Flat2D::TransformationMatrix{},
				     Shaders::Flat2D::Color3{});

      /* Create the ground */
      auto ground1 = new Object2D{&_scene};
      auto ground2 = new Object2D{&_scene};
      auto ground3 = new Object2D{&_scene};
      auto ground4 = new Object2D{&_scene};
      createBody(*ground1, {8.0f, 0.5f}, b2_staticBody, DualComplex::translation(Vector2::yAxis(-8.0f)));
      createBody(*ground2, {8.0f, 0.5f}, b2_staticBody, DualComplex::translation(Vector2::yAxis(8.0f)));
      createBody(*ground3, {0.5f, 8.0f}, b2_staticBody, DualComplex::translation(Vector2::xAxis(-8.0f)));
      createBody(*ground4, {0.5f, 8.0f}, b2_staticBody, DualComplex::translation(Vector2::xAxis(8.0f)));
      new BoxDrawable{*ground1, _instanceData, 0xa5c9ea_rgbf, _drawables};
      new BoxDrawable{*ground2, _instanceData, 0xa5c9ea_rgbf, _drawables};
      new BoxDrawable{*ground3, _instanceData, 0xa5c9ea_rgbf, _drawables};
      new BoxDrawable{*ground4, _instanceData, 0xa5c9ea_rgbf, _drawables};

      /* Create one box */
      auto base = new Object2D{&_scene};
      auto bbase = createBody(*base, {0.5f, 0.5f}, b2_staticBody, DualComplex::translation({0.0f,0.0f}));
      new BoxDrawable{*base, _instanceData, 0xa5c9ea_rgbf, _drawables};
      
      auto box = new Object2D{&_scene};
      const DualComplex transformation = globalTransformation*DualComplex::translation( {2.0f, 0.0f});
      auto bbox = createBody(*box, {2.0f, 0.25f}, b2_dynamicBody, transformation);
      new BoxDrawable{*box, _instanceData, 0x2f83cc_rgbf, _drawables};

      auto box2 = new Object2D{&_scene};
      const DualComplex transformation2 = globalTransformation*DualComplex::translation( {2.5f, 2.0f});
      auto bbox2 = createBody(*box2, {0.5f, 0.5f}, b2_dynamicBody, transformation2);
      new BoxDrawable{*box2, _instanceData, 0x2f83cc_rgbf, _drawables};


      b2RevoluteJointDef jointDef;
      jointDef.Initialize(bbase, bbox, bbase->GetWorldCenter());
      jointDef.lowerAngle = -1.0f * b2_pi;
      jointDef.upperAngle = 1.0f * b2_pi;
      jointDef.enableLimit = false;
      jointDef.maxMotorTorque = 10.0f;
      jointDef.motorSpeed = 10.5f;
      jointDef.enableMotor = true;
      
      b2RevoluteJoint* joint = (b2RevoluteJoint*)_world->CreateJoint(&jointDef);
      /*
      // ... do stuff ...

      myWorld->DestroyJoint(joint);
      joint = nullptr;
      */

      
      setSwapInterval(1);
    }

    void Box2DExample::mousePressEvent(MouseEvent& event) { return; }

    void Box2DExample::drawEvent() {
      GL::defaultFramebuffer.clear(GL::FramebufferClear::Color);

      /* Step the world and update all object positions */
      _world->Step(1.0f/60.0f, 6, 2);
      for(b2Body* body = _world->GetBodyList(); body; body = body->GetNext())
        (*static_cast<Object2D*>(body->GetUserData()))
	  .setTranslation({body->GetPosition().x, body->GetPosition().y})
	  .setRotation(Complex::rotation(Rad(body->GetAngle())));

      /* Populate instance data with transformations and colors */
      arrayResize(_instanceData, 0);
      _camera->draw(_drawables);

      /* Upload instance data to the GPU and draw everything in a single call */
      _instanceBuffer.setData(_instanceData, GL::BufferUsage::DynamicDraw);
      _mesh.setInstanceCount(_instanceData.size());
      _shader.setTransformationProjectionMatrix(_camera->projectionMatrix())
        .draw(_mesh);

      swapBuffers();
      redraw();
    }

  }}

MAGNUM_APPLICATION_MAIN(Magnum::Examples::Box2DExample)

/*int main()
  {
  std::cout<<"hi"<<std::endl;

  b2Vec2 gravity(0.0f, -10.0f);
  b2World world(gravity);

  // add ground
  b2BodyDef groundBodyDef;
  groundBodyDef.position.Set(0.0f, -10.0f);
  b2Body* groundBody = world.CreateBody(&groundBodyDef);
  b2PolygonShape groundBox;
  groundBox.SetAsBox(50.0f, 10.0f);

  groundBody->CreateFixture(&groundBox, 0.0f);

  // add movable body
  b2BodyDef bodyDef;
  bodyDef.type = b2_dynamicBody;
  bodyDef.position.Set(0.0f, 4.0f);
  b2Body* body = world.CreateBody(&bodyDef);

  b2PolygonShape dynamicBox;
  dynamicBox.SetAsBox(1.0f, 1.0f);

  b2FixtureDef fixtureDef;
  fixtureDef.shape = &dynamicBox;
  fixtureDef.density = 1.0f;
  fixtureDef.friction = 0.3f;

  body->CreateFixture(&fixtureDef);

  // simulating

  float timeStep = 1.0f / 60.0f;
  int32 velocityIterations = 8;
  int32 positionIterations = 3;

  for (int32 i = 0; i < 60; ++i)
  {
  world.Step(timeStep, velocityIterations, positionIterations);
  b2Vec2 position = body->GetPosition();
  float angle = body->GetAngle();
  printf("%4.2f %4.2f %4.2f\n", position.x, position.y, angle);
  }
*/
  
//  return 0;
//}

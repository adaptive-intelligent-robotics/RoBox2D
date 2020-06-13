#include "base_application.hpp"

#include <iostream>

namespace robox2d {
  namespace gui {
    // GlobalData
    Magnum::Platform::WindowlessGLContext* GlobalData::gl_context()
    {
      std::lock_guard<std::mutex> lg(_context_mutex);
      if (_gl_contexts.size() == 0)
	_create_contexts();
      
      for (size_t i = 0; i < _max_contexts; i++) {
	if (!_used[i]) {
	  _used[i] = true;
	  return &_gl_contexts[i];
	}
      }
      
      return nullptr;
    }
    
    void GlobalData::free_gl_context(Magnum::Platform::WindowlessGLContext* context)
    {
      std::lock_guard<std::mutex> lg(_context_mutex);
      for (size_t i = 0; i < _gl_contexts.size(); i++) {
	if (&_gl_contexts[i] == context) {
	  _used[i] = false;
	  break;
	}
      }
    }
    
    void GlobalData::set_max_contexts(size_t N)
    {
      std::lock_guard<std::mutex> lg(_context_mutex);
      _max_contexts = N;
      _create_contexts();
    }
    
    void GlobalData::_create_contexts()
    {
      _used.clear();
      _gl_contexts.clear();
      _gl_contexts.reserve(_max_contexts);
      for (size_t i = 0; i < _max_contexts; i++) {
	_used.push_back(false);
	_gl_contexts.emplace_back(Magnum::Platform::WindowlessGLContext{{}});
      }
    }
    
    // BaseApplication
    
    void BaseApplication::init(const std::shared_ptr<b2World>& world, size_t width, size_t height)
    {
      _world=world;
      /* Configure camera */
      _cameraObject = new Object2D{&_scene};
      // set camera location
      _cameraObject->setTranslation({2.5f, 2.5f});
      _camera.reset(new Magnum::SceneGraph::Camera2D{*_cameraObject});
      _camera->setAspectRatioPolicy(Magnum::SceneGraph::AspectRatioPolicy::Extend)
  
  // increase to have larger pic
	.setProjectionMatrix(Magnum::Matrix3::projection({7.0f, 7.0f}))
	.setViewport(Magnum::GL::defaultFramebuffer.viewport().size());
      _drawables.reset(new Magnum::SceneGraph::DrawableGroup2D());

      /* Create an instanced shader */
      _shader = std::unique_ptr<Magnum::Shaders::Flat2D>(new Magnum::Shaders::Flat2D{
	Magnum::Shaders::Flat2D::Flag::VertexColor|
	  Magnum::Shaders::Flat2D::Flag::InstancedTransformation});

      /* Box mesh with an (initially empty) instance buffer */
      // changed this to circle wireframe
      _boxMesh.reset(new Magnum::GL::Mesh(Magnum::MeshTools::compile(Magnum::Primitives::squareSolid())));
      _circleMesh.reset(new Magnum::GL::Mesh(Magnum::MeshTools::compile(Magnum::Primitives::circle2DSolid(20))));

      
      _instanceBuffer = std::unique_ptr<Magnum::GL::Buffer>(new Magnum::GL::Buffer{});
      _boxMesh->addVertexBufferInstanced(*_instanceBuffer, 1, 0,
				     Magnum::Shaders::Flat2D::TransformationMatrix{},
				     Magnum::Shaders::Flat2D::Color3{});

      _circleMesh->addVertexBufferInstanced(*_instanceBuffer, 1, 0,
        Magnum::Shaders::Flat2D::TransformationMatrix{},
        Magnum::Shaders::Flat2D::Color3{});
      _boxInstanceData = std::unique_ptr<Magnum::Containers::Array<InstanceData>>(new Magnum::Containers::Array<InstanceData>() ) ;
      _circleInstanceData = std::unique_ptr<Magnum::Containers::Array<InstanceData>>(new Magnum::Containers::Array<InstanceData>() ) ;



      if(_world)
        for(b2Body* body = _world->GetBodyList(); body; body = body->GetNext())
        {
            auto obj = new Object2D{&_scene};
            body->SetUserData(obj);
            // todo triple check this... Very likely to not work properly as we assume the body is with rot = 0
            //here we assume a single fixture


            switch(body->GetFixtureList()->GetShape()->GetType())
            {
              case b2Shape::e_circle: // if shape is a circle
              {
                b2CircleShape* circle = static_cast<b2CircleShape*>(body->GetFixtureList()->GetShape());
                obj->setScaling(Magnum::Vector2(circle->m_radius, circle->m_radius));
                new Drawable{*obj, *_circleInstanceData, 0xeac9a5_rgbf, *_drawables};
                break;
              }
              case b2Shape::e_polygon: // if shape is a box (more advanced polygon not supported yet)
              {
                b2PolygonShape* poly =  static_cast<b2PolygonShape*>(body->GetFixtureList()->GetShape());
                auto v = poly->m_vertices;
                auto hx = (v[1]-v[0]).Length()/2;
                auto hy = (v[2]-v[1]).Length()/2;
                Magnum::Vector2 halfSize(hx, hy);
                obj->setScaling(halfSize);
                new Drawable{*obj, *_boxInstanceData, 0xa5c9ea_rgbf, *_drawables};	  
                break;
              }
              default: // not supported shapes
              {
                std::cout<<"Warning Shape Type not supported"<<std::endl;
                break;
              }
            }
            
            

        }

    }

    void BaseApplication::update_graphics()
    {
      /* update all object positions */
      if(_world)
	for(b2Body* body = _world->GetBodyList(); body; body = body->GetNext())
	  (*static_cast<Object2D*>(body->GetUserData()))
	    .setTranslation({body->GetPosition().x, body->GetPosition().y})
	    .setRotation(Magnum::Complex::rotation(Magnum::Rad(body->GetAngle())));
    }
    
    bool BaseApplication::done() const
    {
      return _done;
    }


    void BaseApplication::GLCleanUp()
    {
      /* Magnum */
      _shader.reset();
      
      
      _instanceBuffer.reset();
      _boxMesh.reset();
      _boxInstanceData.reset();
      _circleMesh.reset();
      _circleInstanceData.reset();
      
      _camera.reset();
      _drawables.reset();

      
    }

    
  } // namespace gui
} // namespace robot_dart

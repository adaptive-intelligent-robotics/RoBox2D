#include "base_application.hpp"

#include <iostream>
#include <Magnum/PixelFormat.h>
#include <Magnum/GL/Framebuffer.h>

namespace robox2d {
  namespace gui {
    namespace magnum {
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
    BaseApplication::BaseApplication(const GraphicsConfiguration& configuration)
      : _configuration(configuration)
    {}

    void BaseApplication::init(robox2d::Simu* simu, const GraphicsConfiguration& configuration)
    {
      _simu = simu;
      _configuration = configuration;

      /* Configure camera */
      _cameraObject = new Object2D{&_scene};
      _camera.reset(new Magnum::SceneGraph::Camera2D{*_cameraObject});
      _camera->setAspectRatioPolicy(Magnum::SceneGraph::AspectRatioPolicy::Extend)
	.setProjectionMatrix(Magnum::Matrix3::projection({2.0f, 2.0f}))
	.setViewport(Magnum::GL::defaultFramebuffer.viewport().size());
      _drawables.reset(new Magnum::SceneGraph::DrawableGroup2D());

      /* Create an instanced shader */
      _shader = std::unique_ptr<Magnum::Shaders::Flat2D>(new Magnum::Shaders::Flat2D{
	Magnum::Shaders::Flat2D::Flag::VertexColor|
	  Magnum::Shaders::Flat2D::Flag::InstancedTransformation});

      /* Box mesh with an (initially empty) instance buffer */
      _boxMesh.reset(new Magnum::GL::Mesh(Magnum::MeshTools::compile(Magnum::Primitives::squareSolid())));
      _circleMesh.reset(new Magnum::GL::Mesh(Magnum::MeshTools::compile(Magnum::Primitives::circle2DSolid(20))));
      _lineMesh.reset(new Magnum::GL::Mesh(Magnum::MeshTools::compile(Magnum::Primitives::line2D())));

      _instanceBuffer = std::unique_ptr<Magnum::GL::Buffer>(new Magnum::GL::Buffer{});
      _boxMesh->addVertexBufferInstanced(*_instanceBuffer, 1, 0,
				     Magnum::Shaders::Flat2D::TransformationMatrix{},
				     Magnum::Shaders::Flat2D::Color3{});
      _circleMesh->addVertexBufferInstanced(*_instanceBuffer, 1, 0,
				     Magnum::Shaders::Flat2D::TransformationMatrix{},
				     Magnum::Shaders::Flat2D::Color3{});
      _lineMesh->addVertexBufferInstanced(*_instanceBuffer, 1, 0,
				     Magnum::Shaders::Flat2D::TransformationMatrix{},
				     Magnum::Shaders::Flat2D::Color3{});
      _boxInstanceData = std::unique_ptr<Magnum::Containers::Array<InstanceData>>(new Magnum::Containers::Array<InstanceData>() ) ;
      _circleInstanceData = std::unique_ptr<Magnum::Containers::Array<InstanceData>>(new Magnum::Containers::Array<InstanceData>() ) ;
      _lineInstanceData = std::unique_ptr<Magnum::Containers::Array<InstanceData>>(new Magnum::Containers::Array<InstanceData>() ) ;


      if(_simu->world()) {

	for(b2Body* body = _simu->world()->GetBodyList(); body; body = body->GetNext()) {

          // Accessing color of body
          Magnum::Color3 color;
          bool use_default_color;
          const std::map<b2Body*, Magnum::Color3>& map_body_color = simu->get_map_body_color();
          std::map<b2Body*, Magnum::Color3>::iterator iter_map_body_color = map_body_color.find(body);

          if (iter_map_body_color != map_body_color.end()) {
            color = iter_map_body_color->second;
            use_default_color = false;
          } else {
            use_default_color = true;
          }

          // Creating drawable instances
	  for(b2Fixture* fixture = body->GetFixtureList(); fixture; fixture = fixture->GetNext())
	  {
	    auto obj = new Object2D{&_scene};
	    fixture->SetUserData(obj);
	    // todo triple check this... Very likely to not work properly as we assume the fixture is with rot = 0
	    //here we assume a single fixture


	    switch(fixture->GetShape()->GetType())
	      {
	      case b2Shape::e_circle: // if shape is a circle
		 {
                   if (use_default_color) {
                      color = 0xeac9a5_rgbf;
                   }

		   b2CircleShape* circle = static_cast<b2CircleShape*>(fixture->GetShape());
		   obj->setScaling(Magnum::Vector2(circle->m_radius, circle->m_radius));
		   new Drawable{*obj, *_circleInstanceData, color, *_drawables};
		   break;
		 }
	      case b2Shape::e_polygon: // if shape is a box (more advanced polygon not supported yet)
		{
                  if (use_default_color) {
                    color = 0xa5c9ea_rgbf;
                  }

		  b2PolygonShape* poly =  static_cast<b2PolygonShape*>(fixture->GetShape());
		  auto v = poly->m_vertices;
		  auto hx = (v[1]-v[0]).Length()/2;
		  auto hy = (v[2]-v[1]).Length()/2;
		  Magnum::Vector2 halfSize(hx, hy);
		  obj->setScaling(halfSize);
		  new Drawable{*obj, *_boxInstanceData, color, *_drawables};
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
        }

      /*for(b2Joint* joint = _simu->world()->GetJointList(); joint; joint = joint->GetNext())
	{
	  auto obj = new Object2D{&_scene};
	  joint->SetUserData(obj);
	  //Magnum::Vector2 halfSize(1, 0.1);
	  //obj->setScaling(halfSize);

	  new Drawable{*obj, *_lineInstanceData, 0xffc9ea_rgbf, *_drawables};

	  }*/

    }

    void BaseApplication::update_graphics()
    {
      /* update all object positions */
      if(_simu->world())
	{
	  for(b2Body* body = _simu->world()->GetBodyList(); body; body = body->GetNext())
	    for(b2Fixture* fixture = body->GetFixtureList(); fixture; fixture = fixture->GetNext())
	      switch(fixture->GetShape()->GetType())
		{
		case b2Shape::e_circle: // if shape is a circle
		  {
		    b2CircleShape* circle = static_cast<b2CircleShape*>(fixture->GetShape());
		    auto pos = body->GetWorldPoint(circle->m_p);
		    (*static_cast<Object2D*>(fixture->GetUserData()))
		      .setTranslation({pos.x, pos.y});
		   break;
		 }
		case b2Shape::e_polygon: // if shape is a box (more advanced polygon not supported yet)
		  {
		    b2PolygonShape* poly =  static_cast<b2PolygonShape*>(fixture->GetShape());
		    auto v = poly->m_vertices;
		    auto center = (v[2]+v[0]);
		    center*=0.5f;
		    auto pos = body->GetWorldPoint(center);
		    (*static_cast<Object2D*>(fixture->GetUserData()))
		      .setTranslation({pos.x, pos.y})
		      .setRotation(Magnum::Complex::rotation(Magnum::Rad(body->GetAngle())));

		  break;
		}
	      default: // not supported shapes
		{
		  std::cout<<"Warning Shape Type not supported"<<std::endl;
		  break;
		}
	      }


	  /*for(b2Joint* joint = _simu->world()->GetJointList(); joint; joint = joint->GetNext())
	    {
	    (*static_cast<Object2D*>(joint->GetUserData()))
	      .setTranslation({joint->GetAnchorB().x, joint->GetAnchorB().y})
	      .setScaling(Magnum::Vector2(joint->GetReactionForce(1).Length()*100, joint->GetReactionForce(1).Length()*100))
	      .setRotation(Magnum::Complex::rotation(Magnum::Rad(std::atan2(joint->GetReactionForce(1).y,joint->GetReactionForce(1).x))));
	    std::cout<<joint->GetReactionForce(1).x<<"  "<<joint->GetReactionForce(1).y<<std::endl;
	    }*/

	}
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
      _lineMesh.reset();
      _lineInstanceData.reset();

      _camera.reset();
      _drawables.reset();



    }


    void BaseApplication::record_video(const std::string& video_fname, int fps)
    {
      // we use boost process: https://www.boost.org/doc/libs/1_73_0/doc/html/boost_process/tutorial.html
#ifdef ROBOX2D_HAS_BOOST_PROCESS
      namespace bp = boost::process;

      // search for ffmpeg
      boost::filesystem::path ffmpeg = bp::search_path("ffmpeg");
      if (ffmpeg.empty()) {
        std::cout<<"Warning ffmpeg not found in the PATH. Robox2D will not be able to record videos!"<<std::endl;
	return;
      }
#endif

      _recording_video = true;

      // List options
      std::vector<std::string> args = {"-y",
	      "-f", "rawvideo",
	      "-vcodec", "rawvideo",
	      "-s", std::to_string(_configuration.width) + 'x' + std::to_string(_configuration.height),
	      "-pix_fmt", "rgb24",
	      "-r", std::to_string(fps),
	      "-i", "-",
	      "-an",
	      "-vcodec", "mpeg4",
	      "-vb", "20M",
	      video_fname};

#ifdef ROBOX2D_HAS_BOOST_PROCESS
       _ffmpeg_process = bp::child(ffmpeg, bp::args(args), bp::std_in < _video_pipe, bp::std_out > "/dev/null", bp::std_err > "/dev/null");
#else
       // we do it the old way
       pipe(_video_fd);
       //  Data written to fd[1] appears on (i.e., can be read from) fd[0].
       _video_pid = fork();
       if (_video_pid != 0) { // main process
         close(_video_fd[0]); // we close the input on this side
       }
       else { // ffmpeg process
         args.push_back("-loglevel");
	 args.push_back("quiet");
	 close(_video_fd[1]); // ffmpeg does not write here
	 dup2(_video_fd[0], STDIN_FILENO); // ffmpeg will read the fd[0] as stdin
	 char** argv = (char**)calloc(args.size() + 2, sizeof(char*)); // we need the 0 at the end AND the ffffmpeg at the beginning
	 argv[0] = (char*)"ffmpeg";
	 for (size_t i = 0; i < args.size(); ++i)
	   argv[i + 1] = (char*)args[i].c_str();
	 int ret = execvp("ffmpeg", argv);
	 if (ret == -1) {
	   std::cout<<"Warning video recording: cannot execute ffmpeg! [" << strerror(errno) << "]" << std::endl;
	   exit(0);
	 }
       }
#endif
    }

    void BaseApplication::video()
    {
      if (_recording_video) {
	    auto image = rgb_from_image(&(*_image));
#ifdef ROBOX2D_HAS_BOOST_PROCESS
	_video_pipe.write((char*)image.data.data(), image.data.size());
	_video_pipe.flush();
#else
	write(_video_fd[1], (char*)image.data.data(), image.data.size());
#endif
      }

    }
    void BaseApplication::kill_video()
    {
#ifdef ROBOX2D_HAS_BOOST_PROCESS
      if (_ffmpeg_process.id() > 0) {
	 // we let ffmpeg finish nicely by detaching it and sending the signal
	 // terminates() send a violent SIGTERM...
	 _ffmpeg_process.detach();
	 kill(_ffmpeg_process.id(), SIGINT);
      }
#else
      if (_video_pid != 0)
        kill(_video_pid, SIGINT);
#endif
    }

    } // namespace magnum
  } // namespace gui
} // namespace robox2d

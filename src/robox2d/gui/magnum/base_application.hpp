#ifndef ROBOX2D_GUI_MAGNUM_BASE_APPLICATION_HPP
#define ROBOX2D_GUI_MAGNUM_BASE_APPLICATION_HPP

#include <mutex>
#include <unistd.h>
#include <unordered_map>
#include <memory>

#include <box2d/box2d.h>

#include <Corrade/Containers/GrowableArray.h>
#include <Corrade/Utility/Arguments.h>
#include <Magnum/GL/Context.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Buffer.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/Image.h>
#include <Magnum/Math/ConfigurationValue.h>
#include <Magnum/Math/DualComplex.h>
#include <Magnum/MeshTools/Compile.h>
#include <Magnum/Primitives/Square.h>
#include <Magnum/Primitives/Circle.h>
#include <Magnum/Primitives/Line.h>
#include <Magnum/SceneGraph/Camera.h>
#include <Magnum/SceneGraph/Drawable.h>
#include <Magnum/SceneGraph/TranslationRotationScalingTransformation2D.h>
#include <Magnum/SceneGraph/Scene.h>
#include <Magnum/Shaders/Flat.h>
#include <Magnum/Trade/MeshData.h>


// This could/should be removed in the future once boost.process is on every computer...
#include <boost/version.hpp>
#if ((BOOST_VERSION / 100000) > 1) || ((BOOST_VERSION / 100000) == 1 && ((BOOST_VERSION / 100 % 1000) >= 64))
#include <boost/process.hpp> // for launching ffmpeg
#define ROBOX2D_HAS_BOOST_PROCESS
#endif

#include <Magnum/Platform/GLContext.h>
#ifndef MAGNUM_MAC_OSX
#include <Magnum/Platform/WindowlessGlxApplication.h>
#else
#include <Magnum/Platform/WindowlessCglApplication.h>
#endif

#include "robox2d/simu.hpp"

#define get_gl_context_with_sleep_robox2d(name, ms_sleep)			\
  /* Create/Get GLContext */						\
  Corrade::Utility::Debug name##_magnum_silence_output{nullptr};	\
  Magnum::Platform::WindowlessGLContext* name = nullptr;		\
  while (name == nullptr) {						\
    name = robox2d::gui::GlobalData::instance()->gl_context(); \
    /* Sleep for some ms */						\
    usleep(ms_sleep * 1000);						\
  }									\
  while (!name->makeCurrent()) {					\
    /* Sleep for some ms */						\
    usleep(ms_sleep * 1000);						\
  }									\
									\
  Magnum::Platform::GLContext name##_magnum_context;

#define get_gl_context_robox2d(name) get_gl_context_with_sleep_robox2d(name, 0)

#define release_gl_context_robox2d(name) robox2d::gui::GlobalData::instance()->free_gl_context(name);

namespace robox2d {
    class Simu;
  namespace gui {


    typedef Magnum::SceneGraph::Object<Magnum::SceneGraph::TranslationRotationScalingTransformation2D> Object2D;
    typedef Magnum::SceneGraph::Scene<Magnum::SceneGraph::TranslationRotationScalingTransformation2D> Scene2D;

    using namespace Magnum::Math::Literals;

    struct InstanceData {
      Magnum::Matrix3 transformation;
      Magnum::Color3 color;
    };
    
    
    struct GlobalData {
    public:
      static GlobalData* instance()
      {
	static GlobalData gdata;
	return &gdata;
      }
      
      GlobalData(const GlobalData&) = delete;
      void operator=(const GlobalData&) = delete;
	
      Magnum::Platform::WindowlessGLContext* gl_context();
      void free_gl_context(Magnum::Platform::WindowlessGLContext* context);
	
      /* You should call this before starting to draw or after finished */
      void set_max_contexts(size_t N);
	
    private:
      GlobalData() = default;
      ~GlobalData() = default;
	
      void _create_contexts();
	
      std::vector<Magnum::Platform::WindowlessGLContext> _gl_contexts;
      std::vector<bool> _used;
      std::mutex _context_mutex;
      size_t _max_contexts = 4;
    };
      

    class Drawable: public Magnum::SceneGraph::Drawable2D {
    public:
      explicit Drawable(Object2D& object, Magnum::Containers::Array<InstanceData>& instanceData, const Magnum::Color3& color, Magnum::SceneGraph::DrawableGroup2D& drawables): Magnum::SceneGraph::Drawable2D{object, &drawables}, _instanceData(instanceData), _color{color} {}

    private:
      void draw(const Magnum::Matrix3& transformation, Magnum::SceneGraph::Camera2D&) override {
	arrayAppend(_instanceData, Magnum::Containers::InPlaceInit,
		    transformation, _color);
      }

      Magnum::Containers::Array<InstanceData>& _instanceData;
      Magnum::Color3 _color;
    };

    
      
    class BaseApplication {
    public:
      BaseApplication(){}
      virtual ~BaseApplication() {}
	
      void init(robox2d::Simu* simu, size_t width, size_t height);
      void update_graphics();
      	
      Magnum::SceneGraph::DrawableGroup2D& drawables() { return *_drawables; }
      Scene2D& scene() { return _scene; }
      Magnum::SceneGraph::Camera2D* camera() { return &*_camera; }

      Corrade::Containers::Optional<Magnum::Image2D>& image() { return _image; }

      
      bool done() const;

     
      virtual void render() {}
      void GLCleanUp();

      void record_video(const std::string& video_fname, int fps);
      
    protected:
      /* Magnum */
      std::unique_ptr<Magnum::Shaders::Flat2D> _shader;//{Magnum::NoCreate};
      std::unique_ptr<Magnum::GL::Buffer> _instanceBuffer;//{Magnum::NoCreate};
      std::unique_ptr<Magnum::GL::Mesh> _boxMesh;//{Magnum::NoCreate};
      std::unique_ptr<Magnum::Containers::Array<InstanceData>> _boxInstanceData;
      std::unique_ptr<Magnum::GL::Mesh> _circleMesh;//{Magnum::NoCreate};
      std::unique_ptr<Magnum::Containers::Array<InstanceData>> _circleInstanceData;

      std::unique_ptr<Magnum::GL::Mesh> _lineMesh;//{Magnum::NoCreate};
      std::unique_ptr<Magnum::Containers::Array<InstanceData>> _lineInstanceData;

      // Video recording
      bool _recording_video = false;
      size_t _width;
      size_t _height;

#ifdef ROBOX2D_HAS_BOOST_PROCESS
      // pipe to write a video
      boost::process::opstream _video_pipe;
      boost::process::child _ffmpeg_process;
#else
      pid_t _video_pid = 0;
      int _video_fd[2];
#endif

      void video();
      
      
      Scene2D _scene;
      Object2D* _cameraObject;
      std::unique_ptr<Magnum::SceneGraph::Camera2D> _camera;
      std::unique_ptr<Magnum::SceneGraph::DrawableGroup2D> _drawables;
      std::shared_ptr<b2World> _world;
      //Magnum::Containers::Optional<b2World> _world;
      Corrade::Containers::Optional<Magnum::Image2D> _image;
      
      bool _done = false;
    };

    template <typename T>
    inline BaseApplication* make_application(robox2d::Simu* simu, size_t width, size_t height, const std::string& title = "ROBOX2D")
    {
      int argc = 0;
      char** argv = NULL;

      return new T(argc, argv, simu, width, height, title);
    }
    
  } // namespace gui
} // namespace robox2d

#endif

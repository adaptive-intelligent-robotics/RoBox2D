#ifndef ROBOX2D_GUI_BASE_HPP
#define ROBOX2D_GUI_BASE_HPP


#include <box2d/box2d.h>

namespace robox2d {
  namespace gui {
    class Base {
    public:
      Base() {}
      
      Base(std::shared_ptr<b2World>) {}
      
      virtual ~Base() {}
      
      virtual bool done() const
      {
	return false;
      }
      
      virtual void refresh() {}
      
      virtual void set_render_period(double) {}
      
      virtual void set_enable(bool) {}
      
      //virtual Image image() { return Image(); }
      //virtual GrayscaleImage depth_image() { return GrayscaleImage(); }
      //virtual GrayscaleImage raw_depth_image() { return GrayscaleImage(); }
    };
  } // namespace gui
} // namespace robot_dart

#endif

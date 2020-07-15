#ifndef ROBOX2D_GUI_BASE_HPP
#define ROBOX2D_GUI_BASE_HPP


#include <box2d/box2d.h>
#include <robox2d/gui/helper.hpp>

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

      virtual Magnum::Image2D* magnum_image() {}
      
      virtual Image image() {}
      //virtual GrayscaleImage depth_image() { return GrayscaleImage(); }
      //virtual GrayscaleImage raw_depth_image() { return GrayscaleImage(); }
    };
  } // namespace gui
} // namespace robot_dart

#endif

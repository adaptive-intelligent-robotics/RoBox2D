#ifndef ROBOX2D_GUI_BASE_HPP
#define ROBOX2D_GUI_BASE_HPP

#include <robox2d/gui/magnum/helper.hpp>
#include "robox2d/simu.hpp"

namespace robox2d {
    class Simu;

  namespace gui {
    class Base {
    public:
      Base() {}
      
      Base(robox2d::Simu* simu) { _simu = simu; }

      virtual void set_simu(robox2d::Simu* simu) { _simu = simu; }
      const robox2d::Simu* simu() const { return _simu; }
      
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

    protected:
        robox2d::Simu* _simu = nullptr;
    };
  } // namespace gui
} // namespace robot_dart

#endif

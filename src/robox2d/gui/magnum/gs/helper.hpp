#ifndef ROBOT_DART_GUI_MAGNUM_GS_HELPER_HPP
#define ROBOT_DART_GUI_MAGNUM_GS_HELPER_HPP

#include <robox2d/gui/helper.hpp>

#include <Magnum/Image.h>

namespace robox2d {
  namespace gui {
    namespace magnum {
      namespace gs {
        Image rgb_from_image(Magnum::Image2D* image);
      }
    }
  }
}

#endif // ROBOT_DART_GUI_MAGNUM_GS_HELPER_HPP

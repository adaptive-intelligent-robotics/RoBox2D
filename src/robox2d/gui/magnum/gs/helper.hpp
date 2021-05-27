#ifndef ROBOX2D_GUI_MAGNUM_GS_HELPER_HPP
#define ROBOX2D_GUI_MAGNUM_GS_HELPER_HPP

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

#endif // ROBOX2D_GUI_MAGNUM_GS_HELPER_HPP

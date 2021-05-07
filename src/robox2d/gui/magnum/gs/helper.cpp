#include "helper.hpp"

#include <Corrade/Containers/ArrayViewStl.h>
#include <Corrade/Containers/StridedArrayView.h>
#include <Corrade/Utility/Algorithms.h>

#include <Magnum/Math/Color.h>
#include <Magnum/Math/PackingBatch.h>

namespace robox2d {
  namespace gui {
    namespace magnum {
      namespace gs {
        Image rgb_from_image(Magnum::Image2D* image)
        {
          Image img;

          img.width = image->size().x();
          img.height = image->size().y();
          img.channels = 3;
          img.data.resize(image->size().product() * sizeof(Magnum::Color3ub));
          Corrade::Containers::StridedArrayView2D<const Magnum::Color3ub> src = image->pixels<Magnum::Color3ub>().flipped<0>();
          Corrade::Containers::StridedArrayView2D<Magnum::Color3ub> dst{Corrade::Containers::arrayCast<Magnum::Color3ub>(Corrade::Containers::arrayView(img.data)), {std::size_t(image->size().y()), std::size_t(image->size().x())}};
          Corrade::Utility::copy(src, dst);

          return img;
        }
      }
    }
  }
}
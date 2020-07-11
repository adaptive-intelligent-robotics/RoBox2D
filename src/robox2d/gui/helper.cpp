#include "helper.hpp"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <Corrade/Containers/ArrayViewStl.h>
#include <Corrade/Containers/StridedArrayView.h>
#include <Corrade/Utility/Algorithms.h>

#include <Magnum/Math/Color.h>
#include <Magnum/Math/PackingBatch.h>


namespace robox2d {
  namespace gui {

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
      
      
    void save_png_image(const std::string& filename, const Image& rgb)
    {
      auto ends_with = [](const std::string& value, const std::string& ending) {
	if (ending.size() > value.size())
	  return false;
	return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
      };

      std::string png = ".png";
      if (ends_with(filename, png))
	png = "";

      stbi_write_png((filename + png).c_str(), rgb.width, rgb.height, rgb.channels, rgb.data.data(), rgb.width * rgb.channels);
    }

    void save_png_image(const std::string& filename, const GrayscaleImage& gray)
    {
      auto ends_with = [](const std::string& value, const std::string& ending) {
	if (ending.size() > value.size())
	  return false;
	return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
      };

      std::string png = ".png";
      if (ends_with(filename, png))
	png = "";

      stbi_write_png((filename + png).c_str(), gray.width, gray.height, 1, gray.data.data(), gray.width);
    }

    GrayscaleImage convert_rgb_to_grayscale(const Image& rgb)
    {
      assert(rgb.channels == 3);
      size_t width = rgb.width;
      size_t height = rgb.height;

      GrayscaleImage gray;
      gray.width = width;
      gray.height = height;
      gray.data.resize(width * height);

      for (size_t h = 0; h < height; h++) {
	for (size_t w = 0; w < width; w++) {
	  int id = w + h * width;
	  int id_rgb = w * rgb.channels + h * (width * rgb.channels);
	  uint8_t color = 0.3 * rgb.data[id_rgb + 0] + 0.59 * rgb.data[id_rgb + 1] + 0.11 * rgb.data[id_rgb + 2];
	  gray.data[id] = color;
	}
      }

      return gray;
    }
  } // namespace gui
} // namespace robox2d

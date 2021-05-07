#ifndef ROBOX2D_GUI_HELPER_HPP
#define ROBOX2D_GUI_HELPER_HPP

#include <string>
#include <vector>

namespace robox2d {
  namespace gui {
    struct Image {
      size_t width = 0, height = 0;
      size_t channels = 3;
      std::vector<uint8_t> data;
    };
    
    struct GrayscaleImage {
      size_t width = 0, height = 0;
      std::vector<uint8_t> data;
    };

    void save_png_image(const std::string& filename, const Image& rgb);
    void save_png_image(const std::string& filename, const GrayscaleImage& gray);
    
    GrayscaleImage convert_rgb_to_grayscale(const Image& rgb);
  } // namespace gui
} // namespace robox2d

#endif

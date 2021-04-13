#ifndef ROBOX2D_WINDOWLESSGRAPHICS_HPP
#define ROBOX2D_WINDOWLESSGRAPHICS_HPP

namespace robox2d {
  namespace gui {
    namespace magnum {
      class WindowlessGraphics : public BaseGraphics<WindowlessGLApplication>
      {
      public:
        WindowlessGraphics(const GraphicsConfiguration& configuration = default_configuration())
          : BaseGraphics<WindowlessGLApplication>(configuration)
        {}
        ~WindowlessGraphics() {}

        void set_simu(RobotDARTSimu* simu) override;

        static GraphicsConfiguration default_configuration();
      };
    } // namespace magnum
  }   // namespace gui
} // namespace robox2d

class WindowlessGraphics
{};

#endif // ROBOX2D_WINDOWLESSGRAPHICS_HPP

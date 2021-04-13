#ifndef ROBOX2D_GUI_MAGNUM_GLFW_APPLICATION_HPP
#define ROBOX2D_GUI_MAGNUM_GLFW_APPLICATION_HPP

#include <robox2d/gui/magnum/base_application.hpp>

// Workaround for X11lib defines
#undef Button1
#undef Button2
#undef Button3
#undef Button4
#undef Button5
#include <Magnum/Platform/GlfwApplication.h>

#include "robox2d/simu.hpp"

namespace robox2d {

    class Simu;

    namespace gui {
      class GlfwApplication : public BaseApplication, public Magnum::Platform::Application {
      public:
	explicit GlfwApplication(int argc, char** argv, robox2d::Simu* simu, const GraphicsConfiguration& configuration = GraphicsConfiguration());
	
	~GlfwApplication();
	
	void render() override;

	
	
      protected:
	Magnum::Float _speedMove, _speedStrafe;
	
	
	
	static constexpr Magnum::Float _speed = 0.05f;
	
	void viewportEvent(const Magnum::Vector2i& size);
	
	void drawEvent() override;
	
	virtual void keyReleaseEvent(KeyEvent& event) override;
	virtual void keyPressEvent(KeyEvent& event) override;
	
	virtual void mouseScrollEvent(MouseScrollEvent& event) override;
	virtual void mouseMoveEvent(MouseMoveEvent& event) override;
	
	void exitEvent(ExitEvent& event) override;
      };
    } // namespace gui
} // namespace robox2d

#endif

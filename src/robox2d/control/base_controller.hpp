#ifndef ROBOX2D_CONTROL_BASE_CONTROLLER
#define ROBOX2D_CONTROL_BASE_CONTROLLER

#include <memory>
#include <vector>
#include <Eigen/Core>
namespace robox2d {
  namespace control {
    
    class BaseController {
    public:
      BaseController(size_t nb_dofs):_nb_dofs(nb_dofs){}
      virtual ~BaseController() {}
      Eigen::VectorXd commands(double t)
      {
	return Eigen::VectorXd::Ones(_nb_dofs)*sin(2.0*M_PI*t) *M_PI;
      }
      
    protected:
      
      size_t _nb_dofs;
      
    };
  } // namespace control
} // namespace robox2d

#endif

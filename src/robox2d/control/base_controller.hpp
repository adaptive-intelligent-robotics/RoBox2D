#ifndef ROBOX2D_CONTROL_BASE_CONTROLLER
#define ROBOX2D_CONTROL_BASE_CONTROLLER

#include <memory>
#include <vector>
#include <Eigen/Core>

#include "../robot.hpp"

namespace robox2d {
  class Robot; //forward declaration
  
  namespace control {

    
    class BaseController {
    public:
      BaseController(size_t nb_dofs):_nb_dofs(nb_dofs){}
      virtual ~BaseController() {}

      virtual Eigen::VectorXd commands(double t,robox2d::Robot* robot)=0;
      //{
      //return Eigen::VectorXd::Ones(_nb_dofs)*sin(2.0*M_PI*t) *M_PI;
      //}
      
    protected:
      
      size_t _nb_dofs;
      
    };

    class ConstantPos: public BaseController {
    public:
      ConstantPos(Eigen::VectorXd cmd): BaseController(cmd.size()), _cmd(cmd){}
      
      Eigen::VectorXd commands(double t, robox2d::Robot* robot){ return _cmd;}
    private:
      Eigen::VectorXd _cmd;
    };
    
  } // namespace control
} // namespace robox2d

#endif

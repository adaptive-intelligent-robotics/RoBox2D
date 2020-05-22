#include "robot.hpp"

#include <unistd.h>
#include <iostream>

namespace robox2d {
  
  std::shared_ptr<Robot> Robot::clone() const
  {
    //return robot;
  }
  
  void Robot::control_update(double t)
  {
    Eigen::VectorXd commands = Eigen::VectorXd::Zero(nb_dofs());
    for (auto& ctrl : _controllers) {
      commands += ctrl->commands(t);
    }
    for(size_t i = 0; i<nb_dofs(); i++)
      _servos[i]->set_target_pos(commands[i]);
  }

  void Robot::physic_update()
  {
    for(auto s : _servos)
      s->update();
  }
  
} // namespace robox2d

#include "robot.hpp"
#include "control/base_controller.hpp"

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
      commands += ctrl->commands(t, this);
    }

    for(size_t i = 0; i<nb_dofs(); i++)
      _actuators[i]->set_input(commands[i]);
  }

  void Robot::physic_update()
  {
    for(auto s : _actuators)
      s->update();
  }

    void Robot::remove_controller(const std::shared_ptr<control::BaseController>& controller)
    {
        auto it = std::find(_controllers.begin(), _controllers.end(), controller);
        if (it != _controllers.end())
            _controllers.erase(it);
    }

    void Robot::remove_controller(size_t index)
    {
        assert((index < _controllers.size()) && "Controller index out of bounds");
        _controllers.erase(_controllers.begin() + index);
    }

  void Robot::clear_controllers() {
    _controllers.clear();
  }
  
} // namespace robox2d

#include "robot.hpp"

#include <unistd.h>


namespace robox2d {
    
    std::shared_ptr<Robot> Robot::clone() const
    {
      //return robot;
    }

    
    void Robot::update(double t)
    {
      /*Eigen::VectorXd commands = Eigen::VectorXd::Zero(_skeleton->getNumDofs());
        for (auto& ctrl : _controllers) {
            if (ctrl->active())
                commands += ctrl->weight() * ctrl->commands(t);
        }

        _skeleton->setCommands(commands);*/
    }
} // namespace robox2d

#ifndef ROBOX2D_ROBOT_HPP
#define ROBOX2D_ROBOT_HPP

#include<utility>
#include<memory>
#include<vector>

#include "common.hpp"
#include "control/base_controller.hpp"

namespace robox2d {

  
  class Robot : public std::enable_shared_from_this<Robot> {
  public:
    
    Robot(std::shared_ptr<b2World> world){
      
      auto base = common::createBody(world,{0.25f, 0.25f}, b2_staticBody,  {0.0f,0.0f,0.0f} );
      auto seg1 = common::createBody(world,{0.5f , 0.1f }, b2_dynamicBody, {0.5f,0.0f,0.0f} );
      auto seg2 = common::createBody(world,{0.5f , 0.1f }, b2_dynamicBody, {1.5f,0.0f,0.0f} );
      
      b2Vec2 anchor = base->GetWorldCenter();
      //auto j1 =
      _servos.push_back(std::make_shared<common::Servo>(world,base, seg1, anchor));
      //auto j2 =
      anchor = seg1->GetWorldCenter() + b2Vec2(0.5f , 0.0f);
      _servos.push_back(std::make_shared<common::Servo>(world,seg1, seg2,  anchor));
      
    }
    
    std::shared_ptr<Robot> clone() const;
        
    void physic_update();
    void control_update(double t);


    
    
    //void reinit_controllers();
    //size_t num_controllers() const;
    //std::vector<std::shared_ptr<control::RobotControl>> controllers() const;
    //std::vector<std::shared_ptr<control::RobotControl>> active_controllers() const;
    
    //std::shared_ptr<control::RobotControl> controller(size_t index) const;
    
    void add_controller(const std::shared_ptr<control::BaseController>& controller){_controllers.push_back(controller);};
    //void remove_controller(const std::shared_ptr<control::RobotControl>& controller);
    //void remove_controller(size_t index);
    //void clear_controllers();
    
    
     
    //size_t num_dofs() const;
    size_t nb_dofs() const {return _servos.size();};
    //size_t num_bodies() const;
    /*
    Eigen::Vector3d com() const;
    Eigen::Vector6d com_velocity() const;
    Eigen::Vector6d com_acceleration() const;
    
    Eigen::VectorXd positions() const;
    void set_positions(const Eigen::VectorXd& positions);
    
    Eigen::VectorXd velocities() const;
    void set_velocities(const Eigen::VectorXd& velocities);
    
    Eigen::VectorXd accelerations() const;
    void set_accelerations(const Eigen::VectorXd& accelerations);
    
    Eigen::VectorXd forces() const;
    void set_forces(const Eigen::VectorXd& forces);
    
    std::pair<Eigen::Vector6d, Eigen::Vector6d> force_torque(size_t joint_index) const;
    
    
    Eigen::Isometry3d body_pose(const std::string& body_name) const;
    Eigen::Isometry3d body_pose(size_t body_index) const;
    
    std::vector<std::string> body_names() const;
    std::string body_name(size_t body_index) const;
    void set_body_name(size_t body_index, const std::string& body_name);
    */
    
  protected:
    std::vector<std::shared_ptr<common::Servo>> _servos;
    std::vector<std::shared_ptr<control::BaseController>> _controllers;
  };
} // namespace robot_dart

#endif

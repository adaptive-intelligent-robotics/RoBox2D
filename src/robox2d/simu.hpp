#ifndef ROBOX2D_SIMU_HPP
#define ROBOX2D_SIMU_HPP


#include <algorithm> 
#include<memory>
#include<vector>


#include <box2d/box2d.h>

#include "common.hpp"
#include "robot.hpp"
#include "gui/base.hpp"

namespace robox2d {
  class Simu {
  public:
    using robot_t = std::shared_ptr<Robot>;
    
    Simu(size_t physic_freq=100, size_t control_freq=50, size_t graphic_freq=50);
    
    ~Simu();
    
    void run(double max_duration, std::array<Eigen::VectorXf, 2> &trajectories, int trajectory_length);
    
    std::shared_ptr<gui::Base> graphics() const;
    void set_graphics(const std::shared_ptr<gui::Base>& graphics);
    
    std::shared_ptr<b2World> world();



    
    
    /*template <typename Descriptor>
    void add_descriptor(size_t desc_dump = 1)
    {
      add_descriptor(std::make_shared<Descriptor>(Descriptor{*this, desc_dump}));
    }
    
        void add_descriptor(const std::shared_ptr<descriptor::BaseDescriptor>& desc);
        std::vector<std::shared_ptr<descriptor::BaseDescriptor>> descriptors() const;
        std::shared_ptr<descriptor::BaseDescriptor> descriptor(size_t index) const;

        void remove_descriptor(const std::shared_ptr<descriptor::BaseDescriptor>& desc);
        void remove_descriptor(size_t index);
        void clear_descriptors();
    */
    
    /*    void add_camera(const std::shared_ptr<gui::Base>& cam);
        std::vector<std::shared_ptr<gui::Base>> cameras() const;
        std::shared_ptr<gui::Base> camera(size_t index) const;

        void remove_camera(const std::shared_ptr<gui::Base>& desc);
        void remove_camera(size_t index);
        void clear_cameras();
    */
    
    
    size_t num_robots() const;
    std::vector<robot_t> robots() const;
    robot_t robot(size_t index) const;
    
    void add_robot(const robot_t& robot);
    void remove_robot(const robot_t& robot);
    void remove_robot(size_t index);
    void clear_robots();
    
    
    void add_floor();//double floor_width = 10.0, double floor_height = 0.1, const Eigen::Vector6d& pose = Eigen::Vector6d::Zero(), const std::string& floor_name = "floor");
    //  void add_checkerboard_floor(double floor_width = 10.0, double floor_height = 0.1, double size = 1., const Eigen::Vector6d& pose = Eigen::Vector6d::Zero(), const std::string& floor_name = "checkerboard_floor");

  protected:
    std::shared_ptr<b2World> _world;
    double _physic_period;
    double _control_period;
    double _graphic_period;
    double _time;
    double _time_step;
    
    int32 velocityIterations = 6;
    int32 positionIterations = 2;
     
    //std::vector<std::shared_ptr<descriptor::BaseDescriptor>> _descriptors;
    //std::vector<std::shared_ptr<gui::Base>> _cameras; // designed to include mainly graphcis::CameraOSR
    std::vector<robot_t> _robots;
    std::shared_ptr<gui::Base> _graphics;
  };
} // namespace robot_dart

#endif

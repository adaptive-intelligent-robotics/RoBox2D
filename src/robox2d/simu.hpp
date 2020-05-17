#ifndef ROBOX2D_SIMU_HPP
#define ROBOX2D_SIMU_HPP


#include <algorithm> 
#include<memory>
#include<vector>


#include <box2d/box2d.h>

#include "common.hpp"
#include "gui/graphics.hpp"

namespace robox2d {
  class Simu {
  public:
    
    Simu(double time_step = 0.015);
    
    ~Simu();
    
    void run(double max_duration = 5.0);
    
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

    double step() const;
    void set_step(double step);
    
    void stop_sim(bool disable = true);
    bool halted_sim() const;
    
    /*size_t num_robots() const;
    std::vector<robot_t> robots() const;
    robot_t robot(size_t index) const;
    
    void add_robot(const robot_t& robot);
    void remove_robot(const robot_t& robot);
    void remove_robot(size_t index);
    void clear_robots();
    */
    
    void add_floor();//double floor_width = 10.0, double floor_height = 0.1, const Eigen::Vector6d& pose = Eigen::Vector6d::Zero(), const std::string& floor_name = "floor");
    //  void add_checkerboard_floor(double floor_width = 10.0, double floor_height = 0.1, double size = 1., const Eigen::Vector6d& pose = Eigen::Vector6d::Zero(), const std::string& floor_name = "checkerboard_floor");

  protected:
    std::shared_ptr<b2World> _world;
    size_t _old_index;
    double _time_step;
    bool _break;
    //std::vector<std::shared_ptr<descriptor::BaseDescriptor>> _descriptors;
    //std::vector<std::shared_ptr<gui::Base>> _cameras; // designed to include mainly graphcis::CameraOSR
    //std::vector<robot_t> _robots;
    std::shared_ptr<gui::Base> _graphics;
  };
} // namespace robot_dart

#endif

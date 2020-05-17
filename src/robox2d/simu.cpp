#include "simu.hpp"
#include <iostream>

namespace robox2d {
  Simu::Simu(double time_step) : _world(new b2World(b2Vec2(0.0f, -10.0f))),
				 _old_index(0),
				 _time_step(time_step),
				 _break(false)
    {
      //_graphics = std::make_shared<gui::Base>(_world);
    }

    Simu::~Simu()
    {
      //_robots.clear();
        //_descriptors.clear();
        //_cameras.clear();
    }

    void Simu::run(double max_duration)
    {
        _break = false;
        size_t index = _old_index;
        double time = 0.0;
	int32 velocityIterations = 6;
	int32 positionIterations = 2;
	
        while ((time - max_duration) < -_time_step/2.0 && !_graphics->done()) {
	  //std::cout<<time<<std::endl;
	  time+=_time_step;

	  //for (auto& robot : _robots)
	  // robot->update(time);

	  _world->Step(_time_step, velocityIterations, positionIterations);

	  auto bb = _world->GetBodyList();//->GetNext();
	  b2Vec2 position = bb->GetPosition();
	  float angle = bb->GetAngle();
	  printf("%4.2f %4.2f %4.2f\n", position.x, position.y, angle);
	  
	  _graphics->refresh();

	  /*
	  // update descriptors
            for (auto& desc : _descriptors)
                if (index % desc->desc_dump() == 0)
                    desc->operator()();

            // update cameras
            for (auto& cam : _cameras)
                cam->refresh();
	  */
	  ++index;
	  
	  if (_break)
	    break;
        }
        _old_index = index;
    }

  std::shared_ptr<gui::Base> Simu::graphics() const
    {
        return _graphics;
    }

    void Simu::set_graphics(const std::shared_ptr<gui::Base>& graphics)
    {
        _graphics = graphics;
    }
  
  std::shared_ptr<b2World> Simu::world()
  {
    return _world;
  }
    
  /*
  void Simu::add_descriptor(const std::shared_ptr<descriptor::BaseDescriptor>& desc)
  {
    _descriptors.push_back(desc);
  }

  std::vector<std::shared_ptr<descriptor::BaseDescriptor>> Simu::descriptors() const
  {
    return _descriptors;
  }
  
  std::shared_ptr<descriptor::BaseDescriptor> Simu::descriptor(size_t index) const
    {
      //ROBOT_DART_ASSERT(index < _descriptors.size(), "Descriptor index out of bounds", nullptr);
      return _descriptors[index];
    }
  
  void Simu::add_camera(const std::shared_ptr<gui::Base>& cam)
  {
    _cameras.push_back(cam);
  }
  
  std::vector<std::shared_ptr<gui::Base>> Simu::cameras() const
  {
    return _cameras;
  }

  std::shared_ptr<gui::Base> Simu::camera(size_t index) const
  {
    //ROBOT_DART_ASSERT(index < _cameras.size(), "Camera index out of bounds", nullptr);
    return _cameras[index];
  }
  */
  double Simu::step() const
  {
    return _time_step;
  }

    void Simu::set_step(double step)
    {
      _time_step=step;
      //_graphics->set_render_period(step);
    }

    void Simu::stop_sim(bool disable)
    {
        _break = disable;
    }

    bool Simu::halted_sim() const
    {
        return _break;
    }

  /*size_t Simu::num_robots() const
    {
        return _robots.size();
    }

    std::vector<std::shared_ptr<Robot>> Simu::robots() const
    {
        return _robots;
    }

    std::shared_ptr<Robot> Simu::robot(size_t index) const
    {
      //ROBOT_DART_ASSERT(index < _robots.size(), "Robot index out of bounds", nullptr);
        return _robots[index];
    }

    void Simu::add_robot(const std::shared_ptr<Robot>& robot)
    {
      
      //_robots.push_back(robot);
      //_world->addSkeleton(robot->skeleton());
        
    }

    void Simu::remove_robot(const std::shared_ptr<Robot>& robot)
    {
        auto it = std::find(_robots.begin(), _robots.end(), robot);
        if (it != _robots.end()) {
	  //   _world->removeSkeleton(robot->skeleton());
            _robots.erase(it);
        }
    }

    void Simu::remove_robot(size_t index)
    {
      //ROBOT_DART_ASSERT(index < _robots.size(), "Robot index out of bounds", );
      //_world->removeSkeleton(_robots[index]->skeleton());
        _robots.erase(_robots.begin() + index);
    }

    void Simu::clear_robots()
    {
      //for (auto& robot : _robots) {
       //     _world->removeSkeleton(robot->skeleton());
	   // }
        _robots.clear();
    }
*/
  /*void Simu::remove_descriptor(const std::shared_ptr<descriptor::BaseDescriptor>& desc)
    {
        auto it = std::find(_descriptors.begin(), _descriptors.end(), desc);
        if (it != _descriptors.end()) {
            _descriptors.erase(it);
        }
    }

    void RobotDARTSimu::remove_descriptor(size_t index)
    {
        ROBOT_DART_ASSERT(index < _descriptors.size(), "Descriptor index out of bounds", );
        _descriptors.erase(_descriptors.begin() + index);
    }

    void RobotDARTSimu::clear_descriptors()
    {
        _descriptors.clear();
    }

    void RobotDARTSimu::remove_camera(const std::shared_ptr<gui::Base>& cam)
    {
        auto it = std::find(_cameras.begin(), _cameras.end(), cam);
        if (it != _cameras.end()) {
            _cameras.erase(it);
        }
    }

    void RobotDARTSimu::remove_camera(size_t index)
    {
        ROBOT_DART_ASSERT(index < _cameras.size(), "Cameras index out of bounds", );
        _cameras.erase(_cameras.begin() + index);
    }

    void RobotDARTSimu::clear_cameras()
    {
        _cameras.clear();
    }
  */
  void Simu::add_floor()//(double floor_width, double floor_height, const Eigen::Vector6d& pose, const std::string& floor_name)
  {
    common::createBody(_world, {50.0f, 0.50f}, b2_staticBody, {0.0f,-10.5f,0.0f} );
  }

    
} // namespace robot_dart

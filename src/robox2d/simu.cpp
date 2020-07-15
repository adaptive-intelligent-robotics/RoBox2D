#include "simu.hpp"
#include <iostream>
#include <boost/math/common_factor.hpp>

#include <fstream>
namespace robox2d
{

    Simu::Simu(size_t physic_freq, size_t control_freq, size_t graphic_freq) : _world(new b2World(b2Vec2(0.0f, 0.0f))),
                                                                               _time(0),
                                                                               _graphics(nullptr)
    {

        _time_step = 1.0f / double(boost::math::lcm(boost::math::lcm(physic_freq, control_freq), graphic_freq));
        _physic_period = 1.0f / (double)physic_freq;
        _control_period = 1.0f / (double)control_freq;
        _graphic_period = 1.0f / (double)graphic_freq;
    }

    Simu::~Simu()
    {
        _robots.clear();
        //_descriptors.clear();
        //_cameras.clear();
    }

    // retrieve ground truth position information of trajectories
    void Simu::run(double max_duration, std::array<Eigen::VectorXf, 2> &trajectories, Eigen::VectorXf &full_trajectory, int trajectory_length)
    {
        int record_freq{static_cast<int>(max_duration / _time_step / trajectory_length)};
        int trajectory_frame_counter{0};
        int loop_counter{1};

        while ((_time - max_duration) < -_time_step / 2.0 && (!_graphics || !_graphics->done()))
        {
            _time += _time_step;

            // control step
            if (std::abs(std::remainder(_time, _control_period)) < 1e-4)
            {
                for (auto &robot : _robots)
                    robot->control_update(_time);
            }

            // physic step
            if (std::abs(std::remainder(_time, _physic_period)) < 1e-4)
            {
                for (auto &robot : _robots)
                    robot->physic_update();

                _world->Step(_time_step, velocityIterations, positionIterations);
            }

            // graphic step
            if (_graphics && std::abs(std::remainder(_time, _graphic_period)) < 1e-4)
            {
                _graphics->refresh();
                usleep(_graphic_period * 1e6);
            }

            // first ball retrieved is actual ball
            bool is_first_ball{true};

            // two trajectories, one actual and one potential fake
            int trajectory_index{0};

            for (b2Body *body = _world->GetBodyList(); body; body = body->GetNext())
            {
                if (body->GetFixtureList()->GetShape()->GetType() == 0)
                {
                    if (is_first_ball)
                    {
                        b2Vec2 body_pos = body->GetWorldCenter();
                        full_trajectory[(loop_counter - 1) * 2] = body_pos.x;
                        full_trajectory[(loop_counter - 1) * 2 + 1] = body_pos.y;
                        is_first_ball = false;
                    }
                    if (loop_counter % record_freq == 0)
                    {
                        b2Vec2 body_pos = body->GetWorldCenter();
                        trajectories[trajectory_index][trajectory_frame_counter] = body_pos.x;
                        trajectories[trajectory_index][trajectory_frame_counter + 1] = body_pos.y;
                        ++trajectory_index;
                    }
                }
            }
            if (loop_counter % record_freq == 0)
                trajectory_frame_counter += 2;

            ++loop_counter;
        }
    }

    // retrieve image data
    void Simu::run(double max_duration, std::array<Eigen::VectorXf, 2> &trajectories, Eigen::VectorXf &full_trajectory, int trajectory_length, Eigen::VectorXf &avg_image)
    {
        int record_freq{static_cast<int>(max_duration / _time_step / trajectory_length)};
        int trajectory_frame_counter{0};
        int loop_counter{1};

        while ((_time - max_duration) < -_time_step / 2.0 && (!_graphics || !_graphics->done()))
        {
            _time += _time_step;

            // control step
            if (std::abs(std::remainder(_time, _control_period)) < 1e-4)
            {
                for (auto &robot : _robots)
                    robot->control_update(_time);
            }

            // physic step
            if (std::abs(std::remainder(_time, _physic_period)) < 1e-4)
            {
                for (auto &robot : _robots)
                    robot->physic_update();

                _world->Step(_time_step, velocityIterations, positionIterations);
            }

            // graphic step
            if (_graphics && std::abs(std::remainder(_time, _graphic_period)) < 1e-4)
            {
                _graphics->refresh();
                // usleep(_graphic_period * 1e6);
                std::vector<uint8_t> grayscale_img = robox2d::gui::convert_rgb_to_grayscale(_graphics->image()).data;
                for (size_t i{0}; i < grayscale_img.size(); ++i)
                    avg_image[i] += grayscale_img[i];
            }

            // first ball retrieved is actual ball
            bool is_first_ball{true};

            // two trajectories, one actual and one potential fake
            int trajectory_index{0};

            for (b2Body *body = _world->GetBodyList(); body; body = body->GetNext())
            {
                if (body->GetFixtureList()->GetShape()->GetType() == 0)
                {
                    if (is_first_ball)
                    {
                        b2Vec2 body_pos = body->GetWorldCenter();
                        full_trajectory[(loop_counter - 1) * 2] = body_pos.x;
                        full_trajectory[(loop_counter - 1) * 2 + 1] = body_pos.y;
                        is_first_ball = false;
                    }
                    if (loop_counter % record_freq == 0)
                    {
                        b2Vec2 body_pos = body->GetWorldCenter();
                        trajectories[trajectory_index][trajectory_frame_counter] = body_pos.x;
                        trajectories[trajectory_index][trajectory_frame_counter + 1] = body_pos.y;
                        ++trajectory_index;
                    }
                }
            }
            if (loop_counter % record_freq == 0)
                trajectory_frame_counter += 2;

            ++loop_counter;
        }
        avg_image /= (loop_counter - 1);
    }

    // retrieve video data
    void Simu::run(double max_duration, std::array<Eigen::VectorXf, 2> &trajectories, Eigen::VectorXf &full_trajectory, int trajectory_length, Eigen::MatrixXi &img_frames)
    {
        int record_freq{static_cast<int>(max_duration / _time_step / trajectory_length)};
        int trajectory_frame_counter{0};
        int loop_counter{1};

        while ((_time - max_duration) < -_time_step / 2.0 && (!_graphics || !_graphics->done()))
        {
            _time += _time_step;

            // control step
            if (std::abs(std::remainder(_time, _control_period)) < 1e-4)
            {
                for (auto &robot : _robots)
                    robot->control_update(_time);
            }

            // physic step
            if (std::abs(std::remainder(_time, _physic_period)) < 1e-4)
            {
                for (auto &robot : _robots)
                    robot->physic_update();

                _world->Step(_time_step, velocityIterations, positionIterations);
            }

            // graphic step
            if (_graphics && std::abs(std::remainder(_time, _graphic_period)) < 1e-4)
            {
                _graphics->refresh();
                usleep(_graphic_period * 1e6);
                auto gimage = _graphics->image();
                std::vector<uint8_t> grayscale_img = robox2d::gui::convert_rgb_to_grayscale(_graphics->image()).data;
                std::vector<int> int_grayscale_img(grayscale_img.begin(), grayscale_img.end());
                img_frames.row(loop_counter - 1) = Eigen::Map<Eigen::VectorXi> (int_grayscale_img.data(), int_grayscale_img.size());
            }

            // first ball retrieved is actual ball
            bool is_first_ball{true};

            // two trajectories, one actual and one potential fake
            int trajectory_index{0};

            for (b2Body *body = _world->GetBodyList(); body; body = body->GetNext())
            {
                if (body->GetFixtureList()->GetShape()->GetType() == 0)
                {
                    if (is_first_ball)
                    {
                        b2Vec2 body_pos = body->GetWorldCenter();
                        full_trajectory[(loop_counter - 1) * 2] = body_pos.x;
                        full_trajectory[(loop_counter - 1) * 2 + 1] = body_pos.y;
                        is_first_ball = false;
                    }
                    if (loop_counter % record_freq == 0)
                    {
                        b2Vec2 body_pos = body->GetWorldCenter();
                        trajectories[trajectory_index][trajectory_frame_counter] = body_pos.x;
                        trajectories[trajectory_index][trajectory_frame_counter + 1] = body_pos.y;
                        ++trajectory_index;
                    }
                }
            }
            if (loop_counter % record_freq == 0)
                trajectory_frame_counter += 2;

            ++loop_counter;
        }
        // max_duration / _time_step
        // std::ofstream ofs("testfile2");
        // for (size_t i{0}; i < avg_image.size(); ++i)
        // {ofs << avg_image[i] << ", ";}
        // ofs.close();
        // exit(0);
    }

    std::shared_ptr<gui::Base> Simu::graphics() const { return _graphics; }

    void Simu::set_graphics(const std::shared_ptr<gui::Base> &graphics) { _graphics = graphics; }

    std::shared_ptr<b2World> Simu::world() { return _world; }

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
  */

    size_t Simu::num_robots() const { return _robots.size(); }

    std::vector<std::shared_ptr<Robot>> Simu::robots() const { return _robots; }

    std::shared_ptr<Robot> Simu::robot(size_t index) const
    {
        //ROBOT_DART_ASSERT(index < _robots.size(), "Robot index out of bounds", nullptr);
        return _robots[index];
    }

    void Simu::add_robot(const std::shared_ptr<Robot> &robot)
    {

        _robots.push_back(robot);
        //_world->addSkeleton(robot->skeleton());
    }

    void Simu::remove_robot(const std::shared_ptr<Robot> &robot)
    {
        auto it = std::find(_robots.begin(), _robots.end(), robot);
        if (it != _robots.end())
        {
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

   */

    void Simu::add_floor() //(double floor_width, double floor_height, const Eigen::Vector6d& pose, const std::string& floor_name)
    {
        common::createBox(_world, {50.0f, 0.50f}, b2_staticBody, {0.0f, -10.5f, 0.0f});
    }

} // namespace robox2d

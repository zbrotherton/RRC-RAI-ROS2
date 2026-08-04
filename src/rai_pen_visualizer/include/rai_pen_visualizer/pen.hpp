#ifndef PEN_HPP
#define PEN_HPP

#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "tf2/exceptions.h"
#include "tf2_ros/transform_listener.h"
#include "tf2_ros/buffer.h"
#include "visualization_msgs/msg/marker.hpp"
#include "std_msgs/msg/bool.hpp"
#include "geometry_msgs/msg/transform_stamped.hpp"
#include "geometry_msgs/msg/point.hpp"

class Pen {
    public:
        Pen() : logger_(rclcpp::get_logger("")) {}
        Pen(std::string frame_id, std::string canvas_frame_id, rclcpp::Logger logger, rclcpp::Clock::SharedPtr clock);
        std::optional<visualization_msgs::msg::Marker> generate_marker();
        void set_state(bool state){state_ = state;}

    private:
        std::shared_ptr<tf2_ros::TransformListener> tf2_listener_;
        std::unique_ptr<tf2_ros::Buffer> tf2_buffer_;
        rclcpp::Time last_transform_stamp_;
        bool state_;
        bool active_marker_;
        int marker_id_;
        visualization_msgs::msg::Marker current_marker_;
        std::string frame_id_;
        std::string canvas_frame_id_;
        rclcpp::Logger logger_;
        rclcpp::Clock::SharedPtr clock_;
};

#endif  // PEN_HPP
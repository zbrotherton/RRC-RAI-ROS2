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

class Pen {
    public:
        Pen();
        std::optional<visualization_msgs::msg::Marker> generate_marker(unsigned int& marker_count);
        void update_transform();

    private:
        std::shared_ptr<tf2_ros::TransformListener> tf2_listener_;
        std::unique_ptr<tf2_ros::Buffer> tf2_buffer_;
        int last_transform_stamp_;
        std::string frame_id_;
        std::string canvas_frame_id;
        rclcpp::Logger logger;
};

#endif  // PEN_HPP
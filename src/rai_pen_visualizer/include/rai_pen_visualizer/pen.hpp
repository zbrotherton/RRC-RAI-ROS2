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
        Pen(){Pen("");}
        Pen(std::string frame_id);
        void set_state(bool state){state_ = state;}
        bool get_state(){return state_;}
        void initialize_marker(const std::string& canvas_frame);
        void update_stamp(rclcpp::Time time){current_marker_.header.stamp = time;}
        void add_point(geometry_msgs::msg::Point point){current_marker_.points.push_back(point);}
        visualization_msgs::msg::Marker& get_marker(){return current_marker_;}
        std::string get_frame_id(){return frame_id_;}
        bool get_marker_active(){return marker_active_;}
        void finish_marker(const rclcpp::Logger& logger, bool out_of_bounds);
        void update_log(const rclcpp::Logger& logger);

    private:
        bool state_;
        bool marker_active_;
        int marker_id_;
        visualization_msgs::msg::Marker current_marker_;
        std::string frame_id_;
};

#endif  // PEN_HPP
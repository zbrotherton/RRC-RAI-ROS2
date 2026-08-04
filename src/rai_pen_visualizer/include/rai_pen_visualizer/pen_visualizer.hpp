#ifndef PEN_VISUALIZER_HPP
#define PEN_VISUALIZER_HPP

#include <chrono>
#include <functional>
#include <memory>
#include <string>
#include <array>

#include "rclcpp/rclcpp.hpp"
#include "visualization_msgs/msg/marker.hpp"
#include "std_msgs/msg/bool.hpp"

#include "pen.hpp"

using namespace std::chrono_literals;

class PenVisualizer : public rclcpp::Node {
    public:
        PenVisualizer();

    private:
        std::array<Pen, 4> pen_array_;

        unsigned int marker_count_;
        double canvas_bound_;
        std::string canvas_frame_id_;
        std::shared_ptr<tf2_ros::TransformListener> tf2_listener_;
        std::unique_ptr<tf2_ros::Buffer> tf2_buffer_;

        void timer_callback();
        void state_callback(int pen, const std_msgs::msg::Bool& msg);
        void publish_marker(visualization_msgs::msg::Marker& marker) {marker_pub_->publish(marker);}
        
        rclcpp::TimerBase::SharedPtr timer_;
        rclcpp::Publisher<visualization_msgs::msg::Marker>::SharedPtr marker_pub_;
        std::array<rclcpp::Subscription<std_msgs::msg::Bool>::SharedPtr, 4> state_sub_array_;
};

#endif  // PEN_VISUALIZER_HPP
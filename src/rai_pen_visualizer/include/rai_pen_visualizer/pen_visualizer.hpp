#ifndef PEN_VISUALIZER_HPP
#define PEN_VISUALIZER_HPP

#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "visualization_msgs/msg/marker.hpp"
#include "std_msgs/msg/bool.hpp"

class PenVisualizer : public rclcpp::Node {
    public:
        PenVisualizer();

    private:
        void publish_marker();
        void state_callback(const std_msgs::msg::Bool::SharedPtr msg);
        
    rclcpp::Publisher<visualization_msgs::msg::Marker>::SharedPtr marker_pub_;
    rclcpp::Subscription<std_msgs::msg::Bool>::SharedPtr state_sub_;
};

#endif  // PEN_VISUALIZER_HPP
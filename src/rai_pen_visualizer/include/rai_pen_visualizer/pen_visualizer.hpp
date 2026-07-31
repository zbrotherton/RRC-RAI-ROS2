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

class PenVisualizer : public rclcpp::Node {
    public:
        PenVisualizer();

    private:
        std::array<Pen, 4> pen_array_;
        unsigned int marker_count_;
        void timer_callback();
        void state_callback(
            std::function<std::optional<visualization_msgs::msg::Marker>(unsigned int)>& generator,
            const std_msgs::msg::Bool::SharedPtr msg
        );
        void publish_marker();
        
        rclcpp::TimerBase::SharedPtr timer_;
        rclcpp::Publisher<visualization_msgs::msg::Marker>::SharedPtr marker_pub_;
        std::array<rclcpp::Subscription<std_msgs::msg::Bool>::SharedPtr, 4> state_sub_array_;
};

#endif  // PEN_VISUALIZER_HPP
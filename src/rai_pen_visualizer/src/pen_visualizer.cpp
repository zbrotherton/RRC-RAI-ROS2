#include "rai_pen_visualizer/pen_visualizer.hpp"

PenVisualizer::PenVisualizer() : Node("pen_visualizer"), marker_count_(0) {
  this->declare_parameter("canvas_frame_id", "canvas");
  this->declare_parameter("pen_frame_prefix", "pen");
  this->declare_parameter("pen_state_topic_prefix", "pen_state"); 
  
  for(int i = 0; i < 3; i++){
    std::string id = std::to_string(i + 1);
    
    pen_array_.at(i) = Pen(
      this->get_parameter("canvas_frame_id").as_string(),
      this->get_parameter("pen_name_prefix").as_string() + "_" + id,
      this->get_logger(),
      this->get_clock()
    );
    
    state_sub_array_.at(i) = this->create_subscription<std_msgs::msg::Bool>(
      this->get_parameter("pen_state_topic_prefix").as_string() + "_" + id,
      1,
      std::bind(&PenVisualizer::state_callback, this, i, std::placeholders::_1));
  }

  marker_pub_ = this->create_publisher<visualization_msgs::msg::Marker>("pen_marker", 1);
}

void PenVisualizer::state_callback(int pen, const std_msgs::msg::Bool& msg) {
  if(msg.data){
    std::optional<visualization_msgs::msg::Marker> marker = pen_array_.at(pen).generate_marker(marker_count_);
    if(marker.has_value()){
      RCLCPP_INFO(this->get_logger(), "Publishing Marker %u", marker_count_);
      marker_count_++;
      publish_marker(marker.value());
    }
  }
}
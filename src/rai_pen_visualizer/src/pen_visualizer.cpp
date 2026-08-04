#include "rai_pen_visualizer/pen_visualizer.hpp"

PenVisualizer::PenVisualizer() : Node("pen_visualizer"), marker_count_(0) {
  this->declare_parameter("canvas_frame_id", "canvas");
  this->declare_parameter("pen_frame_prefix", "pen");
  this->declare_parameter("canvas_size_in", 20.0);
  this->declare_parameter("pen_state_topic_prefix", "pen_state");
  this->declare_parameter("update_frequency", 20);

  for(int i = 0; i < 4; i++){
    std::string id = std::to_string(i + 1);
    
    pen_array_.at(i) = Pen(
      this->get_parameter("pen_frame_prefix").as_string() + "_" + id,
      this->get_parameter("canvas_frame_id").as_string(),
      this->get_parameter("canvas_size_in").as_double() ,
      this->get_logger(),
      this->get_clock()
    );
    
    state_sub_array_.at(i) = this->create_subscription<std_msgs::msg::Bool>(
      this->get_parameter("pen_state_topic_prefix").as_string() + "_" + id,
      1,
      [this, i](const std_msgs::msg::Bool& msg) {this->state_callback(i, msg);}
    );
  }

  timer_ = this->create_wall_timer(1.0s/this->get_parameter("update_frequency").as_int(), std::bind(&PenVisualizer::timer_callback, this));

  marker_pub_ = this->create_publisher<visualization_msgs::msg::Marker>("pen_marker", 1);
}

void PenVisualizer::timer_callback(){
  for(Pen& pen : pen_array_){
    std::optional<visualization_msgs::msg::Marker> marker = pen.generate_marker();
    if(marker.has_value()){
      publish_marker(marker.value());
    }
  }
}

void PenVisualizer::state_callback(int pen, const std_msgs::msg::Bool& msg) {
  pen_array_.at(pen).set_state(msg.data);
}
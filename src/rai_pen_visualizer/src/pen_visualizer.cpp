#include "rai_pen_visualizer/pen_visualizer.hpp"

PenVisualizer::PenVisualizer() : Node("pen_visualizer"), marker_count_(0) {
  this->declare_parameter("canvas_frame_id", "canvas");
  this->declare_parameter("pen_frame_prefix", "pen");
  this->declare_parameter("pen_state_topic_prefix", "pen_state"); 
  this->declare_parameter("listener_frequency", 50);

  timer_ = this->create_wall_timer(1s/this->get_parameter("listener_frequency").as_int(), 
    std::bind(&PenVisualizer::timer_callback, this));
  
  for(int i = 0; i < 3; i++){
    std::string id = std::to_string(i + 1);
    
    pen_array_.at(i) = Pen(
      this->get_parameter("canvas_frame_id").as_string(),
      this->get_parameter("pen_name_prefix").as_string() + "_" + id,
      this->get_logger()
    );

    auto generator_function = [this, i](unsigned int x) {pen_array_.at(i).generate_marker(x);};
    
    state_sub_array_.at(i) = this->create_subscription<std_msgs::msg::Bool>(
      this->get_parameter("pen_state_topic_prefix").as_string() + "_" + id,
      1,
      [this, i, generator_function](const std_msgs::msg::Bool::SharedPtr msg){
        this->state_callback(generator_function, msg);
      }
    );
  }

  marker_pub_ = this->create_publisher<visualization_msgs::msg::Marker>("pen_marker", 1);
}

void PenVisualizer::timer_callback() {
  for(Pen& p : pen_array_){
    p.update_transform();
  }
}

void PenVisualizer::state_callback(
  std::function<std::optional<visualization_msgs::msg::Marker>(unsigned int)>& generate_marker,
  const std_msgs::msg::Bool::SharedPtr msg) {
  if(msg->data){
    std::optional<visualization_msgs::msg::Marker> marker = generate_marker(marker_count_);
    RCLCPP_INFO(this->get_logger(), "Publishing Point");
    publish_marker();
  }
}

void PenVisualizer::publish_marker() {
  visualization_msgs::msg::Marker marker;

  marker.header.frame_id = "/my_frame";
  marker.header.stamp = rclcpp::Clock().now();

  marker.ns = "pen_visualizer";
  marker.id = marker_count_++;

  marker.type = visualization_msgs::msg::Marker::POINTS;

  marker.action = visualization_msgs::msg::Marker::ADD;

  marker.pose.position.x = 0;
  marker.pose.position.y = 0;
  marker.pose.position.z = 0;
  marker.pose.orientation.x = 0.0;
  marker.pose.orientation.y = 0.0;
  marker.pose.orientation.z = 0.0;
  marker.pose.orientation.w = 1.0;

  marker.scale.x = 1.0;
  marker.scale.y = 1.0;
  marker.scale.z = 1.0;

  marker.color.r = 0.0f;
  marker.color.g = 1.0f;
  marker.color.b = 0.0f;
  marker.color.a = 1.0; 

  marker.lifetime = rclcpp::Duration::from_nanoseconds(0);

  marker_pub_->publish(marker);
}
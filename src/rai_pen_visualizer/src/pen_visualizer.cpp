#include "rai_pen_visualizer/pen_visualizer.hpp"

PenVisualizer::PenVisualizer() : Node("pen_visualizer") {
  marker_pub_ = this->create_publisher<visualization_msgs::msg::Marker>("pen_marker", 1);
  state_sub_ = this->create_subscription<std_msgs::msg::Bool>("pen_state", 1, 
    std::bind(&PenVisualizer::state_callback, this, std::placeholders::_1));
}

void PenVisualizer::publish_marker() {
  visualization_msgs::msg::Marker marker;

  marker.header.frame_id = "/my_frame";
  marker.header.stamp = rclcpp::Clock().now();

  marker.ns = "basic_shapes";
  marker.id = 0;

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

void PenVisualizer::state_callback(const std_msgs::msg::Bool::SharedPtr msg) {
  if(msg->data){
    RCLCPP_INFO(this->get_logger(), "Publishing Point");
    publish_marker();
  }
}

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<PenVisualizer>());
  rclcpp::shutdown();
  return 0;
}
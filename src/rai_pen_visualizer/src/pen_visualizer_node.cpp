#include "rai_pen_visualizer/pen_visualizer.hpp"

int main(int argc, char * argv[]) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<PenVisualizer>());
  rclcpp::shutdown();
  return 0;
}
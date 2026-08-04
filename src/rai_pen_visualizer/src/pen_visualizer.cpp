#include "rai_pen_visualizer/pen_visualizer.hpp"

PenVisualizer::PenVisualizer() : Node("pen_visualizer"), marker_count_(0) {
  this->declare_parameter("canvas_frame_id", "canvas");
  this->declare_parameter("pen_frame_prefix", "pen");
  this->declare_parameter("canvas_size_in", 20.0);
  this->declare_parameter("pen_state_topic_prefix", "pen_state");
  this->declare_parameter("update_frequency", 20);

  canvas_bound_ = this->get_parameter("canvas_size_in").as_double()/2 * 0.0254;
  canvas_frame_id_ = this->get_parameter("canvas_frame_id").as_string();

  tf2_buffer_ = std::make_unique<tf2_ros::Buffer>(this->get_clock());
  tf2_listener_ = std::make_shared<tf2_ros::TransformListener>(*tf2_buffer_);

  for(int i = 0; i < 4; i++){
    std::string id = std::to_string(i + 1);
    
    pen_array_.at(i) = Pen(
      this->get_parameter("pen_frame_prefix").as_string() + "_" + id
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
    if(!pen.get_state()){
        if(!pen.get_marker_active()){
            return;
        }
        pen.finish_marker(this->get_logger(), false);
        publish_marker(pen.get_marker());
    }

    geometry_msgs::msg::TransformStamped t;
    try {
        t = tf2_buffer_->lookupTransform(canvas_frame_id_, pen.get_frame_id(), tf2::TimePointZero);
    }
    catch(const tf2::TransformException & ex) {
        RCLCPP_WARN(this->get_logger(), "Could not transform %s to %s: %s", 
            canvas_frame_id_.c_str(), 
            pen.get_frame_id().c_str(),
            ex.what()
        );
        return;
    }

    if(t.header.stamp == last_transform_stamp_){
        RCLCPP_INFO(this->get_logger(), "Duplicate transform, skipping");
        return;
    }
    last_transform_stamp_ = t.header.stamp;

    if(std::abs(t.transform.translation.x) > canvas_bound_ || 
        std::abs(t.transform.translation.y) > canvas_bound_){
        if(!pen.get_marker_active()){
            return;
        }
        pen.finish_marker(this->get_logger(), true);
        publish_marker(pen.get_marker());
    }

    if(!pen.get_marker_active()){
      pen.initialize_marker(canvas_frame_id_);
    }

    pen.update_stamp(this->get_clock()->now());
    
    geometry_msgs::msg::Point point;

    point.x = t.transform.translation.x;
    point.y = t.transform.translation.y;
    point.z = -0.01;

    pen.add_point(point);

    pen.update_log(this->get_logger());

    publish_marker(pen.get_marker());
  }
}

void PenVisualizer::state_callback(int pen, const std_msgs::msg::Bool& msg) {
  pen_array_.at(pen).set_state(msg.data);
}
#include "rai_pen_visualizer/pen.hpp"

Pen::Pen(std::string frame_id, std::string canvas_frame_id, rclcpp::Logger logger, rclcpp::Clock::SharedPtr clock) : 
    frame_id_(frame_id), canvas_frame_id_(canvas_frame_id), logger_(logger) {
        tf2_buffer_ = std::make_unique<tf2_ros::Buffer>(clock);
        tf2_listener_ = std::make_shared<tf2_ros::TransformListener>(*tf2_buffer_);
        last_transform_stamp_ = rclcpp::Time();
}

std::optional<visualization_msgs::msg::Marker> Pen::generate_marker(unsigned int marker_count) {
    geometry_msgs::msg::TransformStamped t;
    try {
        t = tf2_buffer_->lookupTransform(canvas_frame_id_, frame_id_, tf2::TimePointZero);
    }
    catch(const tf2::TransformException & ex) {
        RCLCPP_WARN(logger_, "Could not transform %s to %s: %s", 
            canvas_frame_id_.c_str(), 
            frame_id_.c_str(),
            ex.what()
        );
        return std::nullopt;
    }
    if(t.header.stamp == last_transform_stamp_){
        RCLCPP_INFO(logger_, "Duplicate transform, skipping");
        return std::nullopt;
    }
    
    last_transform_stamp_ = t.header.stamp;

    visualization_msgs::msg::Marker marker;

    marker.header.frame_id = canvas_frame_id_;
    marker.header.stamp = rclcpp::Clock().now();

    marker.ns = "pen_visualizer";
    marker.id = marker_count;

    marker.type = visualization_msgs::msg::Marker::POINTS;

    marker.action = visualization_msgs::msg::Marker::ADD;

    marker.pose.position.x = t.transform.translation.x;
    marker.pose.position.y = t.transform.translation.y;
    marker.pose.position.z = t.transform.translation.z;
    marker.pose.orientation.x = t.transform.rotation.x;
    marker.pose.orientation.y = t.transform.rotation.y;
    marker.pose.orientation.z = t.transform.rotation.z;
    marker.pose.orientation.w = t.transform.rotation.w;

    marker.scale.x = 1.0;
    marker.scale.y = 1.0;
    marker.scale.z = 1.0;

    marker.color.r = 0.0f;
    marker.color.g = 1.0f;
    marker.color.b = 0.0f;
    marker.color.a = 1.0; 

    marker.lifetime = rclcpp::Duration::from_nanoseconds(0);

    return std::make_optional(marker);
}
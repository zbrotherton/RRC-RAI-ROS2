#include "rai_pen_visualizer/pen.hpp"

Pen::Pen(std::string frame_id, std::string canvas_frame_id, rclcpp::Logger logger, rclcpp::Clock::SharedPtr clock) : 
    frame_id_(frame_id), canvas_frame_id_(canvas_frame_id), logger_(logger), clock_(clock){
        tf2_buffer_ = std::make_unique<tf2_ros::Buffer>(clock);
        tf2_listener_ = std::make_shared<tf2_ros::TransformListener>(*tf2_buffer_);
        last_transform_stamp_ = rclcpp::Time();
        state_ = false;
        active_marker_ = false;
        marker_id_ = 0;
}

std::optional<visualization_msgs::msg::Marker> Pen::generate_marker() {
    if(!state_){
        if(!active_marker_){
            return std::nullopt;
        }
        marker_id_++;
        active_marker_ = false;
        RCLCPP_INFO(logger_, "Finished marker %i for %s", 
            marker_id_, 
            frame_id_.c_str()
        );
        return std::make_optional(current_marker_);
    }

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

    if(!active_marker_){
        current_marker_ = visualization_msgs::msg::Marker();

        current_marker_.header.frame_id = canvas_frame_id_;

        current_marker_.ns = frame_id_;
        current_marker_.id = marker_id_;

        current_marker_.type = visualization_msgs::msg::Marker::LINE_STRIP;

        current_marker_.action = visualization_msgs::msg::Marker::ADD;

        current_marker_.pose.position.x = 0;
        current_marker_.pose.position.y = 0;
        current_marker_.pose.position.z = 0;
        current_marker_.pose.orientation.x = 0;
        current_marker_.pose.orientation.y = 0;
        current_marker_.pose.orientation.z = 0;
        current_marker_.pose.orientation.w = 1;

        current_marker_.scale.x = 0.001;
        current_marker_.scale.y = 0.001;
        current_marker_.scale.z = 0.001;

        current_marker_.color.r = 0.0f;
        current_marker_.color.g = 1.0f;
        current_marker_.color.b = 0.0f;
        current_marker_.color.a = 1.0; 

        current_marker_.lifetime = rclcpp::Duration::from_nanoseconds(0);

        current_marker_.frame_locked = true;

        active_marker_ = true;
    }

    current_marker_.header.stamp = clock_->now();
    
    geometry_msgs::msg::Point point;

    point.x = t.transform.translation.x;
    point.y = t.transform.translation.y;
    point.z = -0.01;

    current_marker_.points.push_back(point);

    RCLCPP_INFO(logger_, "Updating marker %i for %s", 
        marker_id_, 
        frame_id_.c_str()
    );

    return std::make_optional(current_marker_);
}
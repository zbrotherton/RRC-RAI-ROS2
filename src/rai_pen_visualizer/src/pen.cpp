#include "rai_pen_visualizer/pen.hpp"

Pen::Pen(std::string frame_id) : frame_id_(frame_id) {
    state_ = false;
    marker_active_ = false;
    marker_id_ = 0;
}

void Pen::initialize_marker(const std::string& canvas_frame){
    current_marker_ = visualization_msgs::msg::Marker();

    current_marker_.header.frame_id = canvas_frame;

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
    current_marker_.color.g = 0.0f;
    current_marker_.color.b = 0.0f;
    current_marker_.color.a = 1.0; 

    current_marker_.lifetime = rclcpp::Duration::from_nanoseconds(0);

    current_marker_.frame_locked = true;

    marker_active_ = true;
}

void Pen::finish_marker(const rclcpp::Logger& logger, bool out_of_bounds){
    marker_active_ = false;

    if(out_of_bounds){
        RCLCPP_INFO(logger, "Out of bounds, Finished marker %i for %s", 
            marker_id_, 
            frame_id_.c_str()
        );
    } else{
        RCLCPP_INFO(logger, "Finished marker %i for %s", 
            marker_id_,
            frame_id_.c_str()
        );
    }
    
    marker_id_++;
}

void Pen::update_log(const rclcpp::Logger& logger){
    RCLCPP_DEBUG(logger, "Updating marker %i for %s", 
        marker_id_, 
        frame_id_.c_str()
    );
}
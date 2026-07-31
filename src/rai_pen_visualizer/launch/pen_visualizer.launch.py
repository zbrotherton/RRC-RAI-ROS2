from launch import LaunchDescription
from launch.substitutions import PathJoinSubstitution
from launch_ros.substitutions import FindPackageShare
from launch_ros.actions import Node

from launch.substitutions import PathJoinSubstitution

def generate_launch_description(): 
    visualizer_params = PathJoinSubstitution(
        [
            FindPackageShare("rai_pen_visualizer"),
            "config",
            "pen_visualizer.yaml",
        ]
    )

    pen_visualizer_node = Node(
        package="rai_pen_visualizer",
        executable="pen_visualizer_node",
        parameters=[
            visualizer_params
        ],
        output="both",
    )

    nodes = [
        pen_visualizer_node
    ]

    return LaunchDescription(nodes)
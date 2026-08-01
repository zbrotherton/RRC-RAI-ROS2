from launch import LaunchDescription
from launch.substitutions import PathJoinSubstitution, LaunchConfiguration
from launch_ros.substitutions import FindPackageShare
from launch_ros.actions import Node
from launch.actions import DeclareLaunchArgument

def generate_launch_description(): 
    
    declared_arguments = []
    
    declared_arguments.append(
        DeclareLaunchArgument(
            name="use_sim_time",
            default_value="false",
            description="Use simulation time"
        )
    )
    
    use_sim_time = LaunchConfiguration("use_sim_time")
    
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
            visualizer_params,
            {'use_sim_time': use_sim_time}
        ],
        output="both",
    )

    nodes = [
        pen_visualizer_node
    ]

    return LaunchDescription(nodes)
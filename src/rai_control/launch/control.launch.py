from launch import LaunchDescription
from launch.substitutions import PathJoinSubstitution
from launch_ros.substitutions import FindPackageShare
from launch_ros.actions import Node

from launch.substitutions import PathJoinSubstitution, LaunchConfiguration
from launch.event_handlers import OnProcessExit
from launch.actions import DeclareLaunchArgument, RegisterEventHandler

from launch.conditions import UnlessCondition

def generate_launch_description():
    # Declare args
    declared_arguments = []
    
    declared_arguments.append(
        DeclareLaunchArgument(
            name="use_sim",
            default_value="false",
            description="Run in simulation"
        )
    )
    
    use_sim = LaunchConfiguration("use_sim")
    
    # Get nodes
    robot_controllers = PathJoinSubstitution(
        [
            FindPackageShare("rai_control"),
            "config",
            "ros2_controllers.yaml",
        ]
    )

    control_node = Node(
        package="controller_manager",
        executable="ros2_control_node",
        parameters=[
            robot_controllers
        ],
        output="both",
        condition = UnlessCondition(use_sim)
    )
    
    joint_state_broadcaster_spawner = Node(
        package="controller_manager",
        executable="spawner",
        arguments=["joint_state_broadcaster", "--controller-manager", "/controller_manager", "--switch-timeout", "20.0"],
    )

    joint_trajectory_controller_spawner = Node(
        package="controller_manager",
        executable="spawner",
        arguments=["joint_trajectory_controller", "--controller-manager", "/controller_manager", "--switch-timeout", "20.0"],
    )
    
    velocity_controller_spawner = Node(
        package="controller_manager",
        executable="spawner",
        arguments=["velocity_controller", "--controller-manager", "/controller_manager", "--switch-timeout", "20.0"],
    )

    nodes = [
        control_node,
        joint_state_broadcaster_spawner,
        joint_trajectory_controller_spawner,
        velocity_controller_spawner
    ]

    return LaunchDescription(declared_arguments + nodes)
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, IncludeLaunchDescription
from launch.conditions import IfCondition, UnlessCondition
from launch.substitutions import LaunchConfiguration
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch_ros.substitutions import FindPackageShare


def generate_launch_description():
    use_sim = LaunchConfiguration('use_sim')
    use_mock_hardware = LaunchConfiguration('use_mock_hardware')

    return LaunchDescription([
        # Launch Arguments
        DeclareLaunchArgument(
            'use_sim',
            default_value='false',
            description='Run in Simulation'
        ),
        DeclareLaunchArgument(
            'use_mock_hardware',
            default_value=use_sim,
            description='Mock hardware'
        ),

        # Publishers & URDF
        IncludeLaunchDescription(
            PythonLaunchDescriptionSource(
                [FindPackageShare('rai_description'),
                 '/launch',
                 '/publisher.launch.py']
            ),
            launch_arguments={
                'use_mock_hardware': use_mock_hardware
                }.items()
        ),

        IncludeLaunchDescription(
            PythonLaunchDescriptionSource(
                [FindPackageShare('igvc_gazebo'),
                 '/launch',
                 '/empty_world.launch.py'
                ]
            ),
            condition=IfCondition(use_sim),
        ),
    ])
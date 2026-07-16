cd ~/2026RobotCode

apt-get update --fix-missing
apt-get upgrade -y

. /opt/ros/humble/setup.sh

echo '. /opt/ros/humble/setup.sh' >> ~/.bashrc

echo ' if [ -d '~/2026RobotCode/install' ]; then 
    . ~/2026RobotCode/install/setup.bash
fi ' >> ~/.bashrc


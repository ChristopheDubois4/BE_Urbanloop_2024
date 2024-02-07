FROM dustynv/ros:humble-desktop-l4t-r35.4.1

# Mise à jour des packages
RUN apt-get update && apt-get install -y python3-pip

# Installation de paho-mqtt avec pip
RUN pip3 install paho-mqtt

RUN mkdir -p ${ROS_ROOT}/src
RUN source ${ROS_ROOT}/install/setup.bash && cd ${ROS_ROOT}/src && ros2 pkg create urban --build-type ament_python --dependencies rclpy --license Apache-2.0

WORKDIR ${ROS_ROOT}/src/urban

ADD urdf ./
COPY ./speed_publisher.py ./
COPY ./speed_cmd_publisher.py ./
COPY ./urban.launch.sh ./
COPY ./setup.py ./
ADD launch ./

WORKDIR ${ROS_ROOT}

# RUN source ${ROS_ROOT}/install/setup.bash && colcon build --symlink-install --packages-select src/urban
RUN source ${ROS_ROOT}/install/setup.bash && colcon build --merge-install --base-paths src/urban


ENTRYPOINT ["/ros_entrypoint.sh"]
CMD [ "bash" ]

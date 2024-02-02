# ROS 2

La documentation de ros2 est disponible sur [ros.ord](https://docs.ros.org/en/humble/Tutorials.html)

## Topics

- Liste des topics

``` bash
ros2 topic list
```


``` bash
/clicked_point
/current_pose
/diagnostics
/goal_pose
/imu
/initial_pose
/initialpose
/joint_states
/map
/map_array
/modified_map
/modified_map_array
/modified_path
/move_base_simple/goal
/oak/imu/data # unité de mesure inertielle au niveau de la JETSON
/oak/rgb/camera_info
/oak/rgb/image_raw
/oak/rgb/image_raw/compressed # flux vidéeo de la caméra (compréssée)
/oak/rgb/image_raw/compressedDepth
/oak/stereo/camera_info
/oak/stereo/image_raw
/oak/stereo/image_raw/compressed
/oak/stereo/image_raw/compressedDepth
/parameter_events
/path
/robot_description
/rosout
/shuttle_1/ouster/imu # unité de mesure inertielle du LIDAR
/shuttle_1/ouster/metadata
/shuttle_1/ouster/nearir_image
/shuttle_1/ouster/os_driver/transition_event
/shuttle_1/ouster/points # Points du LIDAR
/shuttle_1/ouster/range_image
/shuttle_1/ouster/reflec_image
/shuttle_1/ouster/signal_image
/tf
/tf_static
```


- Avoir des informations sur un topic

``` bash
ros2 topic info "nom_du_topic"
```

Exemple : récupération des informations sur le topic utilisé pour échanger les points du LIDAR

``` bash
ros2 topic info /shuttle_1/ouster/points 
```

``` bash
Type: sensor_msgs/msg/PointCloud2
```

- Détail d'un message ROS

Il est possible de trouver la structure d'un message ROS en ligne, cf [docs.ros.org](http://docs.ros.org/en/noetic/api/sensor_msgs/html/index-msg.html)

``` bash
ros2 interface show "type_du_topic"
```
Exemple : récuparation de la structure de données d'un message ROS de type "Imu" 

En ligne à [Imu.html](http://docs.ros.org/en/noetic/api/sensor_msgs/html/msg/Imu.html) ou en ligne de commande

``` bash
ros2 interface show sensor_msgs/msg/Imu
```

``` bash
# This is a message to hold data from an IMU (Inertial Measurement Unit)
#
# Accelerations should be in m/s^2 (not in g's), and rotational velocity should be in rad/sec
#
# If the covariance of the measurement is known, it should be filled in (if all you know is the
# variance of each measurement, e.g. from the datasheet, just put those along the diagonal)
# A covariance matrix of all zeros will be interpreted as "covariance unknown", and to use the
# data a covariance will have to be assumed or gotten from some other source
#
# If you have no estimate for one of the data elements (e.g. your IMU doesn't produce an
# orientation estimate), please set element 0 of the associated covariance matrix to -1
# If you are interpreting this message, please check for a value of -1 in the first element of each
# covariance matrix, and disregard the associated estimate.

std_msgs/Header header
	builtin_interfaces/Time stamp
		int32 sec
		uint32 nanosec
	string frame_id

geometry_msgs/Quaternion orientation
	float64 x 0
	float64 y 0
	float64 z 0
	float64 w 1
float64[9] orientation_covariance # Row major about x, y, z axes

geometry_msgs/Vector3 angular_velocity
	float64 x
	float64 y
	float64 z
float64[9] angular_velocity_covariance # Row major about x, y, z axes

geometry_msgs/Vector3 linear_acceleration
	float64 x
	float64 y
	float64 z
float64[9] linear_acceleration_covariance # Row major x, y z
```

## ROSBAG

Pour plus d'informations, voir [Recording-And-Playing-Back-Data.html](https://docs.ros.org/en/foxy/Tutorials/Beginner-CLI-Tools/Recording-And-Playing-Back-Data/Recording-And-Playing-Back-Data.html)

- Créer un rosbag

``` bash
ros2 bag info nom_du_rosbag
```

- Lancer le rosbag

Il est possible d'ouvrir directement le rosbag dans foxglove *(ouvrir un fichier local)* ou de relancer les messages directement dur le réseau

``` bash
ros2 bag play nom_du_rosbag
```

**RPOBLEMES**

Pour l'instant le rosbag lag, même quand il est directement enregistré sur le plateau de capteur ce qui rend son exploitation impossible.

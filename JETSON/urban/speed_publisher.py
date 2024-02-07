import rclpy
from rclpy.node import Node
from sensor_msgs.msg import Imu
from std_msgs.msg import Float32
import paho.mqtt.client as paho
from paho import mqtt

# MQTT broker details
cluster_url = "cf9b375e8064414397c16aacf9b8e9cb.s2.eu.hivemq.cloud"
port = 8883
# MQTT credentials
username = "urban"
password = "Urbanl00p2023"

# Sending rate of speed data
publish_rate = 0.01

class ImuToSpeedConverter(Node):


    def __init__(self):
        super().__init__('imu_to_speed_converter')

        # Initialize ROS 2 subscriber to Imu topic
        self.subscription = self.create_subscription(
            Imu,
            '/oak/mu/data',
            self.imu_callback,
            10
        )
        
        # Initialize ROS 2 publisher for custom speed topic
        self.publisher_speed = self.create_publisher(
            Float32,
            '/custom/speed_x',
            10
        )
        
        # Initialize MQTT client
        self.mqtt_client = paho.Client(client_id="", userdata=None, protocol=paho.MQTTv5)
        self.mqtt_client.on_connect = self.on_mqtt_connect
        # enable TLS for secure connection
        self.mqtt_client.tls_set(tls_version=mqtt.client.ssl.PROTOCOL_TLS)
        # set username and password 
        self.mqtt_client.username_pw_set(username, password)
        # connect to HiveMQ Cloud on port 8883
        self.mqtt_client.connect(cluster_url, port)
        
         # Initialize previous callback time
        self.previous_callback_time = None
        # Initialize previous speed
        self.previous_speed_x = 0.0
        
         # Initialize timer for publishing speed
        self.timer = self.create_timer(publish_rate, self.publish_speed)
        
        
    def on_mqtt_connect(self, client, userdata, flags, rc):
        if rc == 0:
            self.get_logger().info('Connected to MQTT broker')
        else:
            self.get_logger().error(f'Failed to connect to MQTT broker with error code {rc}')


    def imu_callback(self, msg):
        # Extract acceleration values from Imu message
        acceleration_x = msg.linear_acceleration.x
        
        # Calculate dt (time elapsed since the previous callback)
        current_callback_time = self.get_clock().now()
        if self.previous_callback_time is None:
            dt = 0.0 
        else:
            dt = (current_callback_time - self.previous_callback_time).nanoseconds / 1e9 
        # Update previous callback time
        self.previous_callback_time = current_callback_time
        
        # Integrate acceleration to get velocity
        speed_x = self.previous_speed_x + acceleration_x * dt
        self.previous_speed_x = speed_x 
        
        
    def publish_speed(self):
        # Publish velocity data to MQTT topic
        self.mqtt_client.publish('/speed', str(self.previous_speed_x))
        
        # Publish velocity data to ROS 2 custom speed topic
        speed_msg = Float32()
        speed_msg.data = self.previous_speed_x
        self.publisher_speed.publish(speed_msg)


def main(args=None):
    rclpy.init(args=args)

    imu_to_speed_converter = ImuToSpeedConverter()
	    
    rclpy.spin(imu_to_speed_converter)
    
    imu_to_speed_converter.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()


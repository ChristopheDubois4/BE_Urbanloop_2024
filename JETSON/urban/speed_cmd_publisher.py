import rclpy
from rclpy.node import Node
from std_msgs.msg import Float32, Bool
import time

class SpeedController(Node):
    def __init__(self):
        super().__init__('speed_controller')

        # Initialize ROS 2 subscriber for current speed
        self.subscription_speed = self.create_subscription(
            Float32,
            '/custom/speed_x',
            self.speed_callback,
            10
        )

        # Initialize ROS 2 subscriber for speed order received
        self.subscription_speed_order = self.create_subscription(
            Float32,
            '/custom/speed_order',
            self.speed_order_callback,
            10
        )

        # Initialize ROS 2 subscriber for obstacle detection
        self.subscription_obstacle = self.create_subscription(
            Bool,
            '/custom/obstacle_detected',
            self.obstacle_callback,
            10
        )

        # Initialize ROS 2 publisher for speed command
        self.publisher_speed_cmd = self.create_publisher(
            Float32,
            '/custom/speed_cmd',
            10
        )

        # Initialize variables for PI controller
        self.Kp = 0.8  # Proportional gain
        self.Ki = 0.1  # Integral gain
        self.last_time = None
        self.err_sum = 0.0

        # Initialize flag for obstacle detection
        self.obstacle_detected = False

    def pi_controller(self, error): 
        # Get current time
        now = time.time()

        # Check if it is the first call of the function
        if self.last_time is None:
            self.last_time = time.time()
            self.err_sum = 0
        else:
            # Calculate the time elapsed since the last calculation (integral)
            time_change = now - self.last_time
            self.err_sum += (error * time_change)

        # Calculate the output of the PI controller
        output = self.Kp * error + self.Ki * self.err_sum 
        output = max(output, 0)
        # Remember the time for the next time
        self.last_time = now
        # return the output value
        return output

    def speed_callback(self, msg):
        
        current_speed = msg.data
        control_msg = Float32()
        
        if not self.obstacle_detected:
            # Compute error (difference between desired and current speed)
            error = self.desired_speed - current_speed
            # Compute control command using PI controller
            control_command = self.pi_controller(error)
            # Publish control command
            control_msg.data = control_command
            self.publisher_speed_cmd.publish(control_msg)
        else :
            # If obstacle detected, set control command to 0
            control_msg.data = 0.0
            self.publisher_speed_cmd.publish(control_msg)
            # Shutdown the node
            rclpy.shutdown()
            

    def speed_order_callback(self, msg):
        self.desired_speed = msg.data

    def obstacle_callback(self, msg):
        self.obstacle_detected = msg.data
        
        
def main(args=None):
    rclpy.init(args=args)
    speed_controller = SpeedController()
    rclpy.spin(speed_controller)
    speed_controller.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()

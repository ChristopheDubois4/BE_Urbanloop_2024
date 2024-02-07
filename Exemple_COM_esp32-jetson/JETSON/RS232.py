import serial
import threading
from time import sleep


class SerialCommunication:
    def __init__(self, port, baudrate, timeout):
        """
        Initialize the SerialCommunication object

        Args:
            - port : The port name or identifier for the serial communication
            - baudrate : The baud rate of the serial communication
            - timeout : The timeout value for reading from the serial port
        """
        self.__serial = serial.Serial(port, baudrate=baudrate, timeout=timeout)

    def receive_data(self):
        """
        Receive data from the serial port

        Return :
            - topic : The topic of the message
            - content : The content of the message
        """
        while True:
            sleep(0.001)
            try:
                message = self.__serial.readline().decode().strip()
                if message:
                    print("Message reçu :", message) # -- Debug
                    # Extract topic and message content
                    topic, content = message.split(' ', 1)
                    return (topic, content)
            except:
                pass

    def send_data(self, topic, content):
        """
        Send data over the serial port

        Args:
            - topic : The topic of the message
            - content : The content of the message
        """
        message = f"{topic} {content}\n"
        self.__serial.write(message.encode())


class Process:
    def __init__(self, port="/dev/ttyTHS1", baudrate=9600, timeout=1):
        """
        Initialize the Process (main program)

        Args:
            - port : The port name or identifier for the serial communication
            - baudrate : The baud rate of the serial communication
            - timeout : The timeout value for reading from the serial port
        """
        # Create an instance of the SerialCommunication class
        self.__serial_com = SerialCommunication(port, baudrate, timeout)
        self.__thread = None
        self.__running = False

    def start_listening(self):
        """
        Start listening for incoming data in a separate thread
        """
        self.__running = True
        self.__thread = threading.Thread(target=self.__process_message)
        self.__thread.start()

    def stop_listening(self):
        """
        Stop listening for incoming data
        """
        self.__running = False
        self.__thread.join()

    def __process_message(self):
        """
        process the receive data from the serial port (example)
        """
        while self.__running:
            # Get the message
            (topic, content) = self.__serial_com.receive_data()
            # Carry out a different treatment depending on the topic
            if topic == "data_1":
                data_1 = int(content)
                # Do something with data_1...
                print("data_1 :", data_1) # -- Debug
            elif topic == "data_2":
                data_2 = int(content)
                # Do something with data_2...
                print("data_2 :", data_2) # -- Debug
            elif topic == "data_3":
                data_3 = int(content)
                # Do something with data_3...
                print("data_3 :", data_3) # -- Debug
            print("")       

    def send_example(self):
        """
        Send data over the serial port (example)

        Args:
            - topic : The topic of the message
            - content : The content of the message
        """
        # Example of use: send data to the ESP32

        v = 150
        d = 110
        c = 130

        while True:

            self.__serial_com.send_data("speed", v)
            self.__serial_com.send_data("distance", d)
            self.__serial_com.send_data("instruction", c)
            v += 1
            d += 1
            c += 1
            sleep(1)


process = Process()
process.start_listening()
process.send_example()



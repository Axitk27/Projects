# =============================================================================
# Created By    : Edmund Jochim
# License       : GNU General Public License v3 (GPL-3)
# Creation Date : 2024-04-10
# =============================================================================
"""The Module Has Been Build for interacting with the Mecanum Wheel Car built in the CSPM Module at THM in summer semester 2024
   The file is inspired and partly based on the Python Wrapper for the Ryze Tello Drone which can be found here: https://github.com/damiafuentes/DJITelloPy
   The Tello Library was published under the MIT License and was created by DAMIÀ FUENTES ESCOTÉ in 2018."""

# =============================================================================
# Imports
# coding=utf-8
import logging
import socket
import time
from collections import deque
from threading import Thread, Lock
from typing import Optional, Union, Type, Dict

#from .enforce_types import enforce_types

# =============================================================================
# Global Variables
threads_initialized = False
cars: Optional[dict] = {}
client_socket: socket.socket

# =============================================================================

class MecanumCarException(Exception):
    pass

#@enforce_types
class MecanumCar:
    """Python wrapper to interact with the Mecanum Wheel Car"""

    RESPONSE_TIMEOUT = 10
    RETRY_COUNT = 3

    TIME_BTW_COMMANDS = 0.1
    TIME_BTW_RC_CONTROL_COMMANDS = 0.001

    CAR_IP = '192.168.10.1'
    CONTROL_UDP_PORT = 4444
    STATE_UDP_PORT = 4445

    # Set up logger
    HANDLER = logging.StreamHandler()
    FORMATTER = logging.Formatter('[%(levelname)s] %(filename)s - %(lineno)d - %(message)s')
    HANDLER.setFormatter(FORMATTER)

    LOGGER = logging.getLogger('mecanumcarpy')
    LOGGER.addHandler(HANDLER)
    LOGGER.setLevel(logging.INFO)
    # Use MecanumCar.LOGGER.setLevel(logging.<LEVEL>) in YOUR CODE
    # to only receive logs of the desired level and higher

    # Conversion functions for state protocol fields
    INT_STATE_FIELDS = (
        'batP', 'mA', 'mB', 'mC', 'mD'
    )
    FLOAT_STATE_FIELDS = ('batV')

    state_field_converters: Dict[str, Union[Type[int], Type[float]]]
    state_field_converters = {key : int for key in INT_STATE_FIELDS}
    state_field_converters.update({key : float for key in FLOAT_STATE_FIELDS})

    def __init__(self, host: str=CAR_IP, retry_count: int=RETRY_COUNT):
        """
        Initialize the MecanumCar instance.

        Args:
            host (str): The IP address of the Mecanum Wheel Car. Default is '0.0.0.0'.
            retry_count (int): The number of times to retry a command if it fails. Default is 3.
        """
        global threads_initialized, client_socket, cars
        self.speed = 63

        self.address = (host, MecanumCar.CONTROL_UDP_PORT)
        self.retry_count = retry_count
        self.last_received_command_timestamp = time.time()
        self.last_rc_control_timestamp = time.time()

        if not threads_initialized:
            # Run MecanumCar command responses UDP receiver on background
            client_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
            client_socket.bind(("", MecanumCar.CONTROL_UDP_PORT))
            response_receiver_thread = Thread(target=MecanumCar.udp_response_receiver)
            response_receiver_thread.daemon = True
            response_receiver_thread.start()

            # Run state UDP receiver on background
            state_receiver_thread = Thread(target=MecanumCar.udp_state_receiver)
            state_receiver_thread.daemon = True
            state_receiver_thread.start()

            threads_initialized = True
        
        cars[host] = {'responses': [], 'state': {}}
        
        self.LOGGER.info("MecanumCar instance was initialized. Host: '{}'. Port: '{}'.".format(host, MecanumCar.CONTROL_UDP_PORT))

    def get_own_udp_object(self):
        global cars

        host = self.address[0]
        return cars[host]

    @staticmethod
    def udp_response_receiver():
        """
        Receive UDP responses from the Mecanum Wheel Car.

        This method runs on a separate thread and continuously receives UDP responses from the Mecanum Wheel Car.
        The responses are stored in the `cars` dictionary for later retrieval.
        """
        while True:
            try:
                data, address = client_socket.recvfrom(1024)

                address = address[0]
                MecanumCar.LOGGER.debug('Data received from {} at client_socket'.format(address))

                if address not in cars:
                    continue

                cars[address]['responses'].append(data)

            except Exception as e:
                MecanumCar.LOGGER.error(e)
                break

    @staticmethod
    def udp_state_receiver():
        """
        Receive state updates from the Mecanum Wheel Car.

        This method runs on a separate thread and continuously receives state updates from the Mecanum Wheel Car.
        The state updates are stored in the `cars` dictionary for later retrieval.
        """
        state_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        state_socket.bind(("", MecanumCar.STATE_UDP_PORT))

        while True:
            try:
                data, address = state_socket.recvfrom(1024)
                address = address[0]
                MecanumCar.LOGGER.debug('Data received from {} at state_socket'.format(address))

                if address not in cars:
                    continue

                data = data.decode('utf-8').strip()
                cars[address]['state'] = MecanumCar.parse_state(data)

            except Exception as e:
                MecanumCar.LOGGER.error(e)
                break

    @staticmethod
    def parse_state(state: str) -> Dict[str, Union[int, float, str]]:
        MecanumCar.LOGGER.debug('Raw state data: {}'.format(state))

        state_dict = {}
        for field in state.split(','):
            if ':' in field:  # Check if the field contains a colon
                key, value = field.split(':')
                if key in MecanumCar.state_field_converters:
                    num_type = MecanumCar.state_field_converters[key]
                    try:
                        value = num_type(value)
                    except ValueError as e:
                        MecanumCar.LOGGER.debug('Error parsing state value for {}: {} to {}'
                                                .format(key, value, num_type))
                        MecanumCar.LOGGER.error(e)
                        continue
                state_dict[key] = value
            else:
                MecanumCar.LOGGER.warning('Field without colon found: {}'.format(field))

        return state_dict


    def get_current_state(self) -> dict:
        """
        Get the current state of the Mecanum Wheel Car.

        Returns:
            dict: A dictionary containing the current state values.
        """
        return self.get_own_udp_object()['state']

    def get_state_field(self, key: str):
        """
        Get a specific field from the current state of the Mecanum Wheel Car.

        Args:
            key (str): The key of the state field to retrieve.

        Returns:
            Any: The value of the state field.

        Raises:
            MecanumCarException: If the specified state field does not exist.
        """
        state = self.get_current_state()

        if key in state:
            return state[key]
        else:
            raise MecanumCarException('Could not get state property: {}'.format(key))

    def send_command_with_return(self, command: str, timeout: int = RESPONSE_TIMEOUT) -> str:
        """
        Send a command to the Mecanum Wheel Car and wait for a response.

        Args:
            command (str): The command to send to the Mecanum Wheel Car.
            timeout (int): The maximum time to wait for a response, in seconds. Default is 10 seconds.

        Returns:
            str: The response received from the Mecanum Wheel Car.

        Raises:
            MecanumCarException: If a response is not received within the specified timeout.
        """
        diff = time.time() - self.last_received_command_timestamp
        if diff < self.TIME_BTW_COMMANDS:
            self.LOGGER.debug('Waiting {} seconds to execute command: {}...'.format(diff, command))
            time.sleep(diff)

        self.LOGGER.info("Send command: '{}'".format(command))
        timestamp = time.time()

        client_socket.sendto(command.encode('utf-8'), self.address)

        responses = self.get_own_udp_object()['responses']

        while not responses:
            if time.time() - timestamp > timeout:
                message = "Aborting command '{}'. Did not receive a response after {} seconds".format(command, timeout)
                self.LOGGER.warning(message)
                return message
            time.sleep(0.1)  # Sleep during send command

        self.last_received_command_timestamp = time.time()

        first_response = responses.pop(0)  # first datum from socket
        try:
            response = first_response.decode("utf-8")
        except UnicodeDecodeError as e:
            self.LOGGER.error(e)
            return "response decode error"
        response = response.rstrip("\r\n")

        self.LOGGER.info("Response {}: '{}'".format(command, response))
        return response

    def send_command_without_return(self, command: str):
        """
        Send a command to the Mecanum Wheel Car without waiting for a response.

        Args:
            command (str): The command to send to the Mecanum Wheel Car.
        """
        self.LOGGER.info("Send command (no response expected): '{}'".format(command))
        client_socket.sendto(command.encode('utf-8'), self.address)

    def send_control_command(self, command: str, timeout: int = RESPONSE_TIMEOUT) -> bool:
        """
        Send a control command to the Mecanum Wheel Car and wait for a response.

        Args:
            command (str): The control command to send to the Mecanum Wheel Car.
            timeout (int): The maximum time to wait for a response, in seconds. Default is 10 seconds.

        Returns:
            bool: True if the command was successful, False otherwise.

        Raises:
            MecanumCarException: If the command fails after the specified number of retries.
        """
        response = "max retries exceeded"
        for i in range(0, self.retry_count):
            response = self.send_command_with_return(command, timeout=timeout)

            if 'ok' in response.lower():
                return True

            self.LOGGER.debug("Command attempt #{} failed for command: '{}'".format(i, command))

        self.raise_result_error(command, response)
        return False # never reached

    def send_read_command(self, command: str) -> str:
        """
        Send a read command to the Mecanum Wheel Car and wait for a response.

        Args:
            command (str): The read command to send to the Mecanum Wheel Car.

        Returns:
            str: The response received from the Mecanum Wheel Car.

        Raises:
            MecanumCarException: If the command fails and returns an error.
        """
        response = self.send_command_with_return(command)

        try:
            response = str(response)
        except TypeError as e:
            self.LOGGER.error(e)

        if any(word in response for word in ('error', 'ERROR', 'False')):
            self.raise_result_error(command, response)
            return "Error: this code should never be reached"

        return response

    def send_read_command_int(self, command: str) -> int:
        """
        Send a read command to the Mecanum Wheel Car and wait for an integer response.

        Args:
            command (str): The read command to send to the Mecanum Wheel Car.

        Returns:
            int: The integer response received from the Mecanum Wheel Car.

        Raises:
            MecanumCarException: If the command fails and returns an error.
        """
        response = self.send_read_command(command)
        return int(response)

    def send_read_command_float(self, command: str) -> float:
        """
        Send a read command to the Mecanum Wheel Car and wait for a float response.

        Args:
            command (str): The read command to send to the Mecanum Wheel Car.

        Returns:
            float: The float response received from the Mecanum Wheel Car.

        Raises:
            MecanumCarException: If the command fails and returns an error.
        """
        response = self.send_read_command(command)
        return float(response)
    
    def raise_result_error(self, command: str, response: str) -> bool:
        """
        Raise an error after an unsuccessful command.

        This method is used internally to raise an error when a command fails after the specified number of retries.

        Args:
            command (str): The command that failed.
            response (str): The response received from the Mecanum Wheel Car.

        Raises:
            MecanumCarException: Always raises this exception.
        """
        tries = 1 + self.retry_count
        raise MecanumCarException("Command '{}' was unsuccessful for {} tries. Latest response:\t'{}'"
                             .format(command, tries, response))
    
    def get_batteryVoltage(self) -> float:
        """Get voltage of battery in V.
        Returns:
            float: Battery voltage in V
        """
        return self.get_state_field('batV')
    
    def get_batteryPercent(self) -> int:
        """Get battery charge linearized estimation in percent
        Returns:
            int: battery charge in percent
        """
        return self.get_state_field('batP')
    
    def get_motorA(self) -> int:
        """Get motor A duty cycle in percent
        Returns:
            int: motor A duty cycle in percent
        """
        return self.get_state_field('mA')
    
    def get_motorB(self) -> int:
        """Get motor B duty cycle in percent
        Returns:
            int: motor B duty cycle in percent
        """
        return self.get_state_field('mB')
    
    def get_motorC(self) -> int:
        """Get motor C duty cycle in percent
        Returns:
            int: motor C duty cycle in percent
        """
        return self.get_state_field('mC')
    
    def get_motorD(self) -> int:
        """Get motor D duty cycle in percent
        Returns:
            int: motor D duty cycle in percent
        """
        return self.get_state_field('mD')

    def send_keepalive(self)-> None:
        """
        Sends a keepalive control command.

        This method is responsible for sending a keepalive control command to the car.
        The keepalive command is used to maintain the connection and prevent it from timing out.

        Parameters:
        - None

        Returns:
        - None
        """
        self.send_control_command("keepalive")

    def move(self, direction: str) -> None:
        """
        Drive the MecanumCar left, right, forward, or back.

        Parameters:
            direction (str): The direction to move the car. Can be 'left', 'right', 'forward', or 'back'.

        Raises:
            ValueError: If an unexpected error occurs.

        Returns:
            None
        """
        if direction not in ['left', 'right', 'forward', 'back']:
            raise ValueError("Invalid direction: {}".format(direction))

        if direction == 'left':
            self.send_rc_control(0, -self.speed, 0)
        elif direction == 'right':
            self.send_rc_control(0, self.speed, 0)
        elif direction == 'forward':
            self.send_rc_control(self.speed, 0, 0)
        elif direction == 'back':
            self.send_rc_control(-self.speed, 0, 0)
        
    def move_diagonally(self, direction: str) -> None:
        """
        Moves the car diagonally in one of four directions.

        Parameters:
            direction (str): The direction in which to move the car diagonally. Can be 'rf' for right-forward, 'rb' for right-backward,
            'lb' for left-backward, or 'lf' for left-forward.

        Raises:
            ValueError: If an unexpected error occurs.

        Returns:
            None
        """
        if direction not in ['rf', 'rb', 'lb', 'lf']:
            raise ValueError("Invalid direction: {}".format(direction))
        
        if direction == 'rf':
            self.send_rc_control(int(self.speed/2), int(self.speed/2), 0)
        elif direction == 'rb':
            self.send_rc_control(-int(self.speed/2), int(self.speed/2), 0)
        elif direction == 'lb':
            self.send_rc_control(-int(self.speed/2), -int(self.speed/2), 0)
        elif direction == 'lf':
            self.send_rc_control(int(self.speed/2), -int(self.speed/2), 0)

    def move_forward(self) -> None:
        """
        Moves the car forward either for a specified amount of time or a specified distance.

        Returns:
            None
        """
        self.move("forward")

    def move_back(self) -> None:
        """
        Move the car backwards either for a specified amount of time or a specified distance.

        Returns:
        - None
        """
        self.move("back")

    def rotate_clockwise(self) -> None:
        """
        Rotates the car clockwise either by a specified time or angle.

        Returns:
        - None
        """
        self.send_rc_control(0, 0, self.speed)

    def rotate_counter_clockwise(self) -> None:
        """
        Rotates the car in a counter-clockwise direction.

        Returns:
        - None
        """
        self.send_rc_control(0, 0, -self.speed)
        

    def send_rc_control(self, forward_backward_velocity: int, left_right_velocity: int,
                        angular_velocity: int) -> None:
        """Send RC control via four channels. Command is sent every self.TIME_BTW_RC_CONTROL_COMMANDS seconds.
        
        Args:
            left_right_velocity (int): The velocity for left/right movement. Range: -127 to 127.
            forward_backward_velocity (int): The velocity for forward/backward movement. Range: -127 to 127.
            angular_velocity (int): The angular velocity for yaw movement. Range: -127 to 127.
        """
        def clamp127(x: int) -> int:
            return max(-127, min(127, x))

        if time.time() - self.last_rc_control_timestamp > self.TIME_BTW_RC_CONTROL_COMMANDS:
            self.last_rc_control_timestamp = time.time()
            cmd = 'rc {} {} {}'.format(
                clamp127(forward_backward_velocity),
                clamp127(left_right_velocity),
                clamp127(angular_velocity)
            )
            self.send_command_without_return(cmd)

    def update_speed(self, speed: int) -> None:
        """Update the speed of the car.
        
        Args:
            speed (int): The new speed of the car. Range: 0 to 127.
        """
        self.speed = speed

    def stop(self) -> None:
        """Stop the car."""
        self.send_control_command("stop")

def main():
    car = MecanumCar()


if __name__ == "__main__":
    main()
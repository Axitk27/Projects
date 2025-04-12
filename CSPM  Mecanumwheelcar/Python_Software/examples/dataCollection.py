#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
import os
import time
import numpy as np
import pandas as pd
from datetime import datetime
from threading import Thread

sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))
from mecanumcarpy import MecanumCar

battery_voltages = []
battery_percents = []
motor_as = []
motor_bs = []
motor_cs = []
motor_ds = []
timestamps = []

def control_car(car):
    def wait_time(t: int):
        time.sleep(t / 2)
        car.send_keepalive()
        time.sleep(t / 2)

    """Control the movement of the car."""
    car.update_speed(100)
    car.move_forward()
    wait_time(10)

    car.update_speed(110)
    car.move_back()
    wait_time(10)

    car.update_speed(80)
    car.move_diagonally(direction='rb')
    wait_time(10)

    car.stop()
    wait_time(10)

    car.update_speed(70)
    car.move_diagonally(direction='lf')
    wait_time(10)

    car.update_speed(127)
    car.move(direction="left")
    wait_time(10)

    car.stop()

def get_state(car, duration, interval=1):
    """Collect state information about the car every second."""
    global battery_voltages, battery_percents, motor_as, motor_bs, motor_cs, motor_ds, timestamps

    time.sleep(5)

    start_time = time.time()
    while time.time() - start_time < duration-5:
        battery_voltages.append(car.get_batteryVoltage())
        battery_percents.append(car.get_batteryPercent())
        motor_as.append(car.get_motorA())
        motor_bs.append(car.get_motorB())
        motor_cs.append(car.get_motorC())
        motor_ds.append(car.get_motorC())
        timestamps.append(datetime.now())

        time.sleep(interval)

def main():
    car = MecanumCar()
    car.send_keepalive()

    # Define duration for control and state collection
    duration = 60  # 60 seconds

    # Start state collection in a separate thread
    state_thread = Thread(target=get_state, args=(car, duration))
    
    # Start car control in a separate thread
    control_thread = Thread(target=control_car, args=(car,))

    # Start both threads
    state_thread.start()
    control_thread.start()

    # Wait for both threads to complete
    control_thread.join()
    state_thread.join()

    # Convert the state data into a pandas DataFrame
    state_data = {
        'timestamp': timestamps,
        'battery_voltage': battery_voltages,
        'battery_percent': battery_percents,
        'motor_a': motor_as,
        'motor_b': motor_bs,
        'motor_c': motor_cs,
        'motor_d': motor_ds,
    }
    df = pd.DataFrame(state_data)

    # Save the collected state information to a CSV file
    df = pd.DataFrame(state_data)

    # Generate file name with current date and time
    file_name = datetime.now().strftime("car_state_%Y%m%d_%H%M%S.csv")
    df.to_csv(file_name, index=False)
    print(f"State data saved to {file_name}")

if __name__ == "__main__":
    main()

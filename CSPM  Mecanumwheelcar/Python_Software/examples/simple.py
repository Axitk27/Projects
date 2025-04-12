#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
import os
import time

sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))
from mecanumcarpy import MecanumCar

def main():
    car = MecanumCar()
    
    time.sleep(3)
    car.update_speed(100)
    car.move_forward()
    time.sleep(2)
    car.stop()
    
    car.move_diagonally(direction='rf')

    print(car.get_batteryPercent())
    print(car.get_batteryVoltage())
    print(car.get_motorA())
    print(car.get_motorB())
    print(car.get_motorC())
    print(car.get_motorD())
    time.sleep(3)

    car.stop()

    

if __name__ == "__main__":
    main()
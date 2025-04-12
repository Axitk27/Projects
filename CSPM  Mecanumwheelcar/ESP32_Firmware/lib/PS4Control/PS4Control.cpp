/*    
    Programm intended to be used in the CCCE 2024 Mecanum Wheels Car Project.
    Copyright (C) 2024 Edmund Jochim

    This file uses the PS4 Controller Library by https://github.com/aed3 to
    control the car using a PS4 controller. The class is responsible for setting up the
    PS4 controller and handling the input from the controller.

    This code is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License V3 as published by
    the Free Software Foundation.

    This code is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "PS4Control.h"
#include <Arduino.h>

PS4_Control::PS4_Control() {
    // Constructor
}

bool PS4_Control::PS4_Setup(const char* macAddress) {
    PS4.begin(macAddress);
    return true;
}

void PS4_Control::handleInput(int8_t move_dir[3]) {
    if (PS4.isConnected()) {
    if (PS4.LStickX()) {
      #ifdef SERIAL_DEBUG
        Serial.printf("Left Stick x at %d\n", PS4.LStickX());
      #endif
      if (PS4.LStickX() > ANALOG_STICK_DEADZONE || PS4.LStickX() < -ANALOG_STICK_DEADZONE){
        move_dir[2] = PS4.LStickX();
      }
      else {
        move_dir[2] = 0;
      }
    }
    if (PS4.LStickY()) {
      #ifdef SERIAL_DEBUG
        Serial.printf("Left Stick y at %d\n", PS4.LStickY());
      #endif
      if (PS4.LStickY() > ANALOG_STICK_DEADZONE || PS4.LStickY() < -ANALOG_STICK_DEADZONE){
        move_dir[2] = PS4.LStickY();
      }
      else {
        move_dir[2] = 0;
      }
    }
    /*
    if (PS4.RStickX()) {
      #ifdef SERIAL_DEBUG
        Serial.printf("Right Stick x at %d\n", PS4.RStickX());
      #endif
    }
    */
    if (PS4.RStickY()) {
      #ifdef SERIAL_DEBUG
        Serial.printf("Right Stick y at %d\n", PS4.RStickY());
      #endif
      if (PS4.RStickY() > ANALOG_STICK_DEADZONE || PS4.RStickY() < -ANALOG_STICK_DEADZONE){
        move_dir[0] = PS4.RStickY();
      }
      else {
        move_dir[0] = 0;
      }
    }
    
     if (PS4.L2()) {
      #ifdef SERIAL_DEBUG
        Serial.printf("L2 button at %d\n", PS4.L2Value());
      #endif
      if (PS4.L2Value() > TRIGGER_DEADZONE) {
        move_dir[1] = static_cast<int8_t>((PS4.L2Value()) * -0.5);
      }
      else {
        move_dir[1] = 0;
      }
    }
    if (PS4.R2()) {
      #ifdef SERIAL_DEBUG
        Serial.printf("R2 button at %d\n", PS4.R2Value());
      #endif
      if (PS4.R2Value() > TRIGGER_DEADZONE) {
        move_dir[1] = static_cast<int8_t>((PS4.R2Value()) * 0.5);
      }
      else {
        move_dir[1] = 0;
      }
    }
  }
}

void PS4_Control::batteryToLED(int batteryPercent) {
    if (batteryPercent >= 80) {
        PS4.setLed(0, 255, 0);
    }
    else if (batteryPercent >= 50 && batteryPercent < 80) {
        PS4.setLed(200, 255, 0);
    }
    else if (batteryPercent >= 20 && batteryPercent < 50) {
        PS4.setLed(255, 140, 0);
    }
    else if (batteryPercent < 20) {
        PS4.setLed(255, 0, 0);
    }
    else
        PS4.setLed(0, 0, 255);
}



/*
void PS4_Control::printMacAddress() {
    PS4.begin();
    const uint8_t* address = esp_bt_dev_get_address();
    char str[100];
    sprintf(str, "ESP32's Bluetooth MAC address is - %02x:%02x:%02x:%02x:%02x:%02x", address[0],address[1],address[2],address[3],address[4],address[5]);
    Serial.println(str);
}
*/
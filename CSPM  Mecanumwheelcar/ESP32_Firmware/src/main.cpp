/*  
    Copyright (C) 2024 Axit Kakadiya, Devraj Solanki and Edmund Jochim

    Programm intended to be used for the CCCE 2024 Mecanum Wheels Car Project.
    The main.cpp file includes the necessary libraries and defines the control mode and other configuration options.
    It also initializes the necessary objects and variables, sets up the motor control, and handles the control input.
    Additionally, it handles battery level monitoring and displays it on an LED.

    The control mode can be selected by commenting out one of the following defines: UDP_CONTROL_MODE or PS4_CONTROL_MODE.
    Uncomment SERIAL_DEBUG to enable serial debugging.

    The main functions in this file are:
    - setup(): Initializes the car, sets up the control mode, and sets up the motor control.
    - loop(): Handles the control input, calculates the motor PWM values, and outputs them to the motors.
    - batteryEmpty(): Handles the case when the battery level is critically low.

    This code is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License V3 as published by
    the Free Software Foundation.

    This code is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    Note: This file assumes the presence of other source files and libraries that are not included in this code snippet.

    For more information, refer to the project documentation and comments within the code.
*/
#include <Arduino.h>
#include <MotorPWM.h>
#include <UDPcontrol.h>
#include <PS4Control.h>
#include <BatteryStatus.h>
#include <StatusLED.h>
#include "Sensor.h"
#include <Adafruit_LSM9DS1.h>

/*########################################################################*/
// Define control mode (comment out one of the following)                //#
#define UDP_CONTROL_MODE                                                 //#
//#define PS4_CONTROL_MODE                                               //#
/*########################################################################*/

// Uncomment to print the MAC address of ESP32 to the serial monitor
//#define PRINT_MAC_ADDRESS

// Define to enable serial debugging
//#define SERIAL_DEBUG

#ifdef SERIAL_DEBUG
  // Used to store the last time a message was printed to the serial monitor
  unsigned long previousMillis = 0;
#endif

// Variable used to check battery level and display it on the LED
unsigned long StatusUpdateMillis = 0;
float batteryVoltage;
uint8_t batteryPercent;

#ifdef UDP_CONTROL_MODE
  // Variable used to store the last time a status message was sent
  unsigned long lastStatusMessageMillis = 0;

  // Definition of network credentials and static IP configuration
  const char* ssid = "MecanumWheelCar";
  const char* password = "car12345";
  unsigned int commandPort = 4444;
  unsigned int statusPort = 4445;
  IPAddress localIP(192, 168, 10, 1);
  IPAddress gateway(192, 168, 10, 1);
  IPAddress subnet(255, 255, 255, 0);
#endif

#ifdef PS4_CONTROL_MODE
  unsigned long ledupdateMillis = 0;
#endif

// Arrays to store the move direction and motor speeds
int8_t moveDir[3]; // Array for dirX, dirY, and angle
int16_t motorSpeed[4]; // Array for motor_A, motor_B, motor_C, and motor_D

// Object for PWM motor control (frequency = 20 kHz, resolution = 8 bits)
MotorPWM MotorControl(20000,8);

// Create object for LSM9DS1 sensor
//Adafruit_LSM9DS1 lsm = Adafruit_LSM9DS1();
//read_data acc(&lsm);

#ifdef UDP_CONTROL_MODE
  UDPControl udpControl(ssid, password, commandPort, statusPort, localIP, gateway, subnet);
#elif defined(PS4_CONTROL_MODE)
  PS4_Control ps4Control;
#endif

// Function to handle the case when the battery level is critically low
void batteryEmpty();

// Function to initialize the car and set up the control mode
void setup(){
  initStatusLED();
  playStartupAnimation();

  initBatteryStatus();
  
  #if defined(PRINT_MAC_ADDRESS) || defined(SERIAL_DEBUG)
    Serial.begin(115200);
  #endif

  #ifdef PRINT_MAC_ADDRESS
    // Print the MAC address of the ESP32 to the serial monitor
    // Need to uncomment Libraries in PS4Control.h
    PS4_Control ps4BT;
    ps4BT.printMacAddress();
  #endif

  #ifdef UDP_CONTROL_MODE
    udpControl.setupAP();
    udpControl.begin();
  #elif defined(PS4_CONTROL_MODE)
    // Enter the Bluetooth MAC address of the ESP32
    ps4Control.PS4_Setup("a0:a3:b3:27:71:0a");
  #endif

  //acc.millisOld = millis();
  //acc.sensor_setup(lsm);

  // Set up the motor control
  MotorControl.pwmSetup();

  // Initialize the move direction
  moveDir[0] = 0;
  moveDir[1] = 0;
  moveDir[2] = 0;

  batteryVoltage = getBatteryVoltage();
  batteryPercent = calculateBatteryPercent(batteryVoltage);
}

// Main loop function to handle control input, calculate motor PWM values, and output them to the motors
void loop(){
  if (millis() - StatusUpdateMillis >= 1000){
      StatusUpdateMillis = millis();
      
      batteryVoltage = getBatteryVoltage();
      batteryPercent = calculateBatteryPercent(batteryVoltage);
      showBatteryLevel(batteryPercent);
    }

  /* Battey level check bugs sometimes out because voltage can drop below 10% for a short time
  if (batteryPercent <= 10){
    moveDir[0] = 0;
    moveDir[1] = 0;
    moveDir[2] = 0;
    MotorControl.calculatePWM(moveDir,motorSpeed);
    MotorControl.output(motorSpeed);
    batteryEmpty();
  }
  */
  
  #ifdef UDP_CONTROL_MODE
    udpControl.handlePackets(moveDir);

    if (udpControl.lastCommandReceived && (millis() - lastStatusMessageMillis >= 1000)) {
        lastStatusMessageMillis = millis();
        
        udpControl.sendStatusMessage(batteryVoltage, batteryPercent, motorSpeed);
    }

  #elif defined(PS4_CONTROL_MODE)
    ps4Control.handleInput(moveDir);

    if (millis() - ledupdateMillis >= 1000){
      ledupdateMillis = millis();
      ps4Control.batteryToLED(batteryPercent);
    }
  #endif

  MotorControl.calculatePWM(moveDir,motorSpeed);
  MotorControl.output(motorSpeed);

  // collect sensor data from gyroscope and accelerometer
  //acc.data_read(lsm, acc);

  #ifdef SERIAL_DEBUG
  if (millis() - previousMillis >= 1000)
  {
    previousMillis = millis();

    //acc.data_display(acc);
    
    float batteryVoltage = getBatteryVoltage();
    Serial.print("Battery ADC: ");
    Serial.println(getADCValue());
  
    Serial.print("Battery voltage: ");
    Serial.print(batteryVoltage);
    Serial.print(" V, ");
    Serial.print(calculateBatteryPercent(batteryVoltage));
    Serial.println(" %");

    Serial.println("Motor speeds: ");
    Serial.print("mA: ");
    Serial.print(motorSpeed[0]);
    Serial.print(" ");
    Serial.print("mB: ");
    Serial.println(motorSpeed[1]);
    Serial.print("mC: ");
    Serial.print(motorSpeed[2]);
    Serial.print(" ");
    Serial.print("mD: ");
    Serial.println(motorSpeed[3]);
    
  }
  #endif
}

// Function to handle the case when the battery level is critically low
void batteryEmpty(){
  while (true){
    setGreenLED(false);
    setYellowLED(false);
    setRedLED(true);
    delay(500);
    setRedLED(false);
    delay(500);
  }
}

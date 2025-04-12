# CSPM_MecanumWheelCar

## Description
A mecanum wheel is an omnidirectional wheel design for a land-based vehicle to move in any
direction. [Source](https://en.wikipedia.org/wiki/Mecanum_wheel). A toy car with four mecanum
wheels has been build up from building blocks (similar to LEGO bricks) and will be provided by
supervisor. Each motor can be independantly driven by a seperate motor.
The existing control is only a very simple on-off-control the motors. In the project, it should be
replaced by an advanced control with PWM-based speed adjustment for each motor.

## Tasks
1) Hardware
 - Selection of hardware components, preferably based on available boards: 2S / 7.4V LiPo Battery and charger („BMS“), 5V-Output Step-Down-Converter for μC supply, μC-Board (Preferably ESP32), H-Bridge drivers for the motors
 - Wiring of the components (using jumper-wires, wago cage clamp connectors or similar)
 - 3D-printing of housing; this must be mountable on the car’s chassis
 - Selection of Gamepad / HID for user interaction

2) Software (Mostly on ESP32)
 - Reception of steering commands via WiFi / UDP or bluetooth
 - Development of a suitable command list (e.g. 3x integer for: fw/rev, turn left/right, move left/right)
 - Transformation of steering commands to motor control PWM duty cycle
 - Optional: Use of 6-DOF or 9-DOF sensor for acceleration measurement and advanced control design
 - If WiFi / UDP is used: PC-Software (Python) to convert gamepad / user commands into UDP Datagrams being transfered to the car
 
 ## Schematic
 ![Schematic](Schematic/Schematic_MecanumCar_bb.png?raw=true "Schematic of MecanumWheelCar")

## Command list
| Command   	| Description                                                                                                                              	| Possible Response 	|
|-----------	|------------------------------------------------------------------------------------------------------------------------------------------	|-------------------	|
| rc x y z  	| Apply speed to  x y or z direction. x (backward - forward) = -127 - 127 y (left - right) = -127 - 127 z (rotation ccw - cw) = -127 - 127 	| -/error           	|
| keepalive 	| Prevents timeout from occuring.                                                                                                          	| ok/error          	|
| stop      	| Set all speed to 0.                                                                                                                      	| ok/error          	|

## Android App
 ![Schematic](MecanumCarApp/App_Screenshot.jpg?raw=true "Screenshot of Mecanum Car App")

 An Android App is available to control the Mecanum Car via Wifi on an Android Smartphone. You need to be in Mecanum Car's Wifi network. Then you can hold the buttons to control the direction. The slider at the bottom can be used to change the speed of the Mecanum Car. On the top right is the battery level of the Mecanum Car displayed.
 The apk for the app can be downloaded [here](https://git.thm.de/ejhm83//uploads/09837b6083e2bd7ce43f53d85e7c19a5/app-release.apk).

 Note the logo of THM belongs to Technische Hochschule Mittelhessen and is not part of the license of this project.

## Licence
Project is licensed under GNU GENERAL PUBLIC LICENSE Version 3 for the software components and
CC BY-SA 4.0 for the hardware components.

Project developed as part of the module Case Study with Project Managing in the Control, Computer and Communications Engineering (CCCE) study programm in the IEM department at THM.

[Technische Hochschule Mittelhessen | IEM](https://www.thm.de/iem/)

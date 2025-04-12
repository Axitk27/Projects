/*    
    Programm intended to be used in the CCCE 2024 Mecanum Wheels Car Project.
    Copyright (C) 2024 Edmund Jochim

    This file is the header file for the PS4 control class.
    It uses the PS4 Controller Library by https://github.com/aed3
    which can be found here: https://github.com/aed3/PS4-esp32

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

#ifndef PS4_CONTROL_H
#define PS4_CONTROL_H

#include <PS4Controller.h>
//#include <esp_bt_main.h>
//#include <esp_bt_device.h>

#define ANALOG_STICK_DEADZONE 10
#define TRIGGER_DEADZONE 5

class PS4_Control {
    public:
    PS4_Control();
    bool PS4_Setup(const char* macAddress);
    void handleInput(int8_t move_dir[3]);
    void batteryToLED(int batteryPercent);
    //void printMacAddress();
};


#endif
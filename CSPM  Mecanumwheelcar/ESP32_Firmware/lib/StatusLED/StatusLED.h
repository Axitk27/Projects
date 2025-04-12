/*    
    Programm intended to be used for the CCCE 2024 Mecanum Wheels Car Project.
    Copyright (C) 2024 Edmund Jochim

    This file is the header file for the status LED functions.

    This code is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License V3 as published by
    the Free Software Foundation.

    This code is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef STATUSLED_H
#define STATUSLED_H

#define LED_RED_PIN 12
#define LED_YELLOW_PIN 14
#define LED_GREEN_PIN 27

void initStatusLED();
void setRedLED(bool state);
void setYellowLED(bool state);
void setGreenLED(bool state);

void playStartupAnimation();
void showBatteryLevel(int batteryPercentage);

#endif
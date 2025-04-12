/*    
    Programm intended to be used for the CCCE 2024 Mecanum Wheels Car Project.
    Copyright (C) 2024 Axit Kakadiya

    This file is the header file for the battery status functions.

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

#ifndef BATTERYSTATUS_H
#define BATTERYSTATUS_H

// Pin to measure battery voltage
#define BATTERY_PIN 34

// Maximum and minimum voltage levels
#define MAX_VOLTAGE 8.4
#define MIN_VOLTAGE 6.4

#define R1 51065.0
#define R2 32780.0

void initBatteryStatus();

// Function to get the battery voltage
float getBatteryVoltage();

int getADCValue();

// Function to calculate the battery level
int calculateBatteryPercent(float voltage);

#endif // BATTERY_STATUS_H
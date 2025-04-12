/*    
    Programm intended to be used for the CCCE 2024 Mecanum Wheels Car Project.
    Copyright (C) 2024 Axit Kakadiya

    This file is used to watch the battery status of the car. The functions in this
    file are used to get the battery voltage and calculate the battery level.

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

#include "BatteryStatus.h"
#include<Arduino.h>

void initBatteryStatus() {
    pinMode(BATTERY_PIN, INPUT);
}

float getBatteryVoltage() {
    int adc_value = analogRead(BATTERY_PIN);
    return static_cast<float>((adc_value / 4095.0) * 3.3)*((R1+R2)/R2); // ESP32 ADC resolution is 12-bit (0-4095)
}

int getADCValue() {
    return analogRead(BATTERY_PIN);
}

int calculateBatteryPercent(float voltage) {
    /*
    int percentage = (voltage - MIN_VOLTAGE) / (MAX_VOLTAGE - MIN_VOLTAGE) * 100;
    if (percentage > 100) {
        percentage = 100;
    }
    */
   float v_normalized = (voltage - MIN_VOLTAGE) / (MAX_VOLTAGE - MIN_VOLTAGE);
   // state of charge is calculated using fitted polynomial curve
   int SOC = static_cast<int>(112.0 - (112.0 / (1.0 + std::pow(1.9 * v_normalized, 2.9))));
    if (SOC > 100) {
         SOC = 100;
    }
    if (SOC < 0) {
        SOC = 0;
    }
    return SOC;
}
/*    
    Programm intended to be used for the CCCE 2024 Mecanum Wheels Car Project.
    Copyright (C) 2024 Edmund Jochim

    This file is used to display the status of the car using LEDs. All LEDs can be
    controlled separately. Alternatively, Animations can be defined. There is also
    a function to display the battery level using the LEDs.

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

#include "StatusLED.h"
#include <Arduino.h>

void initStatusLED(){
    pinMode(LED_RED_PIN, OUTPUT);
    pinMode(LED_YELLOW_PIN, OUTPUT);
    pinMode(LED_GREEN_PIN, OUTPUT);
}

void setRedLED(bool state){
    digitalWrite(LED_RED_PIN, state);
}

void setYellowLED(bool state){
    digitalWrite(LED_YELLOW_PIN, state);
}

void setGreenLED(bool state){
    digitalWrite(LED_GREEN_PIN, state);
}

void playStartupAnimation(){
    for (int i = 0; i < 3; i++){
        setRedLED(true);
        delay(500);
        setRedLED(false);
        setYellowLED(true);
        delay(500);
        setYellowLED(false);
        setGreenLED(true);
        delay(500);
        setGreenLED(false);
    }
}
void showBatteryLevel(int batteryPercentage){
    if (batteryPercentage < 20){
        setRedLED(true);
        setYellowLED(false);
        setGreenLED(false);
    }
    else if (batteryPercentage < 50){
        setRedLED(true);
        setYellowLED(true);
        setGreenLED(false);
    }
    else{
        setRedLED(true);
        setYellowLED(true);
        setGreenLED(true);
    }
}
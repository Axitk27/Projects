/*    
    Programm intended to be used in the CCCE 2024 Mecanum Wheels Car Project.
    Copyright (C) 2024 Axit Kakadiya

    This file is the header file for the MotorPWM class.

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

#ifndef Motorpwm_h
#define Motorpwm_h
#include<Arduino.h>
#include <driver/ledc.h>

#define A_FRONT 23
#define A_BACK 22

#define B_FRONT 21
#define B_BACK 19

#define D_FRONT 17
#define D_BACK 16

#define C_FRONT 18
#define C_BACK 5

#define AF_Channel 0
#define AB_Channel 1
#define BF_Channel 2
#define BB_Channel 3
#define CF_Channel 4
#define CB_Channel 5
#define DF_Channel 6
#define DB_Channel 7

#define MIN_PWM 153  // 60 % of MAX_DUTY_CYCLE
#define THRES_PWM 25 // 10 % of MAX_DUTY_CYCLE


class MotorPWM{

    private:

    u_int16_t PWM_FREQ;
    u_int8_t PWM_RESOLUTION ;
    int16_t MAX_DUTY_CYCLE;
    int maxSpeed;
    
    public:

    MotorPWM(int PWM_Frequency, int PWM_Resolution);
    void pwmSetup();
    void calculatePWM(int8_t* movedir,int16_t* motorspeed);
    void output(int16_t* motorspeed);

};
#endif
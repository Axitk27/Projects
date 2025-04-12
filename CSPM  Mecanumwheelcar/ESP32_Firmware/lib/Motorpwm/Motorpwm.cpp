/*    
    Programm intended to be used in the CCCE 2024 Mecanum Wheels Car Project.
    Copyright (C) 2024 Axit Kakadiya

    This file is used to control the motors of the car using PWM signals. The class is responsible for setting up the
    PWM signals and controlling the speed of the motors.

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

#include "MotorPWM.h"

MotorPWM :: MotorPWM(int PWM_Frequency, int PWM_Resolution){
        this->PWM_FREQ = PWM_Frequency;
        this->PWM_RESOLUTION = PWM_Resolution;
        this->MAX_DUTY_CYCLE = (1 << PWM_RESOLUTION) - 1;
        this->maxSpeed = 127;
}

void MotorPWM::pwmSetup(){
        ledcSetup(AF_Channel, PWM_FREQ, PWM_RESOLUTION);
        ledcSetup(AB_Channel, PWM_FREQ, PWM_RESOLUTION);
        ledcSetup(BF_Channel, PWM_FREQ, PWM_RESOLUTION);
        ledcSetup(BB_Channel, PWM_FREQ, PWM_RESOLUTION);
        ledcSetup(CF_Channel, PWM_FREQ, PWM_RESOLUTION);
        ledcSetup(CB_Channel, PWM_FREQ, PWM_RESOLUTION);
        ledcSetup(DF_Channel, PWM_FREQ, PWM_RESOLUTION);
        ledcSetup(DB_Channel, PWM_FREQ, PWM_RESOLUTION);

        ledcAttachPin(A_FRONT, AF_Channel);
        ledcAttachPin(A_BACK, AB_Channel);
        ledcAttachPin(B_FRONT, BF_Channel);
        ledcAttachPin(B_BACK, BB_Channel);
        ledcAttachPin(C_FRONT, CF_Channel);
        ledcAttachPin(C_BACK, CB_Channel);
        ledcAttachPin(D_FRONT, DF_Channel);
        ledcAttachPin(D_BACK, DB_Channel);
}



void MotorPWM::calculatePWM(int8_t* movedir,int16_t* motorspeed)
{
        // Calculate the speeds of the four wheels
        motorspeed[0] = movedir[0] + movedir[1] + movedir[2];
        motorspeed[1] = movedir[0] - movedir[1] - movedir[2];
        motorspeed[2] = movedir[0] - movedir[1] + movedir[2];
        motorspeed[3] = movedir[0] + movedir[1] - movedir[2];

        // Normalize the speeds
        int16_t maximum = abs(motorspeed[0]);

        for (int i = 1; i < 4; i++){
                if (abs(motorspeed[i]) > maxSpeed){
                        maxSpeed = abs(motorspeed[i]);
                }
        }

        if(maximum < this->maxSpeed){
                maximum = this->maxSpeed;
        }

    maximum = this->maxSpeed;

        for (int i = 0; i < 4; i++) {
        motorspeed[i] = static_cast<int16_t>((static_cast<int32_t>(motorspeed[i]) * MAX_DUTY_CYCLE) / maximum);
        
        if (motorspeed[i] > THRES_PWM && motorspeed[i] < MIN_PWM) {
                motorspeed[i] = MIN_PWM;
        }
        else if (motorspeed[i] > -MIN_PWM && motorspeed[i] < -THRES_PWM) {
                motorspeed[i] = MIN_PWM;
        }
        
        }
}

void MotorPWM::output(int16_t* motorspeed) 
{
// Set the motor speeds
if (motorspeed[0] >= 0) {
        ledcWrite(AF_Channel, abs(motorspeed[0]));
        ledcWrite(AB_Channel, 0);
}

else if (motorspeed[0] < 0) {
        ledcWrite(AF_Channel, 0);
        ledcWrite(AB_Channel, abs(motorspeed[0]));

}

if (motorspeed[1] >= 0) {
        ledcWrite(BF_Channel, abs(motorspeed[1]));
        ledcWrite(BB_Channel, 0);
}

else if (motorspeed[1] < 0) {
        ledcWrite(BF_Channel, 0);
        ledcWrite(BB_Channel, abs(motorspeed[1]));
}

if (motorspeed[2] >= 0) {
        ledcWrite(CF_Channel, abs(motorspeed[2]));
        ledcWrite(CB_Channel, 0);
}

else if (motorspeed[2] < 0) {
        ledcWrite(CF_Channel, 0);
        ledcWrite(CB_Channel, abs(motorspeed[2]));        
} 


if (motorspeed[3] >= 0) {
        ledcWrite(DF_Channel, abs(motorspeed[3]));
        ledcWrite(DB_Channel, 0);
} 

else if (motorspeed[3] < 0) {
        ledcWrite(DF_Channel, 0);
        ledcWrite(DB_Channel, abs(motorspeed[3]));
}

}
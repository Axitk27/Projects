/*    
    Programm intended to be used in the CCCE 2024 Mecanum Wheels Car Project.
    Copyright (C) 2024 Devraj Solanki and Edmund Jochim

    This file is used for the UDP control class. This class is used to
    control the car using UDP packets. The class is responsible for setting up the
    UDP server, handling incoming packets, and sending responses.

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

#include "UDPControl.h"

UDPControl::UDPControl(const char* ssid, const char* password, unsigned int commandPort, unsigned int statusPort, IPAddress local_ip, IPAddress gateway, IPAddress subnet) {
    this->ssid = ssid;
    this->password = password;
    this->udpCommandPort = commandPort;
    this->udpStatusPort = statusPort;
    this->localIP = local_ip;
    this->gateway = gateway;
    this->subnet = subnet;
    this->lastKeepaliveTime = millis();
    this->lastCommandReceived = false;
}

bool UDPControl::setupAP() {
    WiFi.softAPConfig(localIP, gateway, subnet);
    WiFi.softAP(ssid, password);
    #ifdef SERIAL_DEBUG
        Serial.println("Access Point Started");
        Serial.print("AP IP address: ");
        Serial.println(localIP);
    #endif
    return true;
}

void UDPControl::begin() {
    udpCommand.begin(udpCommandPort);
    udpStatus.begin(udpStatusPort);
    #ifdef SERIAL_DEBUG
        Serial.print("UDP server started on port: ");
        Serial.println(udpPort);
        Serial.print("UDP status server started on port: ");
        Serial.println(udpStatusPort);
    #endif
}

void UDPControl::handlePackets(int8_t move_dir[3]) {
    int packetSize = udpCommand.parsePacket();
    if (packetSize) {
        char incomingPacket[255];
        int len = udpCommand.read(incomingPacket, 255);
        if (len > 0) {
            incomingPacket[len] = 0;
        }
        #ifdef SERIAL_DEBUG
                Serial.print("Received UDP packet: ");
                Serial.println(incomingPacket);
        #endif

        if (decodeAndExecuteCommand(String(incomingPacket), move_dir)) {
            lastKeepaliveTime = millis();
            lastCommandIP = udpCommand.remoteIP();
            lastCommandReceived = true;
        } else {
            sendResponse("error");
        }
    }

    if (millis() - lastKeepaliveTime >= 10000) {
        move_dir[0] = 0;
        move_dir[1] = 0;
        move_dir[2] = 0;
#ifdef SERIAL_DEBUG
        Serial.println("Timeout: Stopping the car due to no keepalive signal.");
#endif
    }
}

bool UDPControl::decodeAndExecuteCommand(const String& command, int8_t move_dir[3]) {
    String cmd = command;
    cmd.trim();

#ifdef SERIAL_DEBUG
    Serial.print("Decoding command: ");
    Serial.println(cmd);
#endif

    int firstSpace = cmd.indexOf(' ');
    String action = firstSpace == -1 ? cmd : cmd.substring(0, firstSpace);
    String rest = firstSpace == -1 ? "" : cmd.substring(firstSpace + 1);

    if (action.equalsIgnoreCase("rc")) {
        int x, y, z;
        int num = sscanf(rest.c_str(), "%d %d %d", &x, &y, &z);
        if (num == 3 && x >= -127 && x <= 127 && y >= -127 && y <= 127 && z >= -127 && z <= 127) {
            move_dir[0] = x;
            move_dir[1] = y;
            move_dir[2] = z;
            return true;
        }
    } else if (action.equalsIgnoreCase("stop")) {
        move_dir[0] = 0;
        move_dir[1] = 0;
        move_dir[2] = 0;
        sendResponse("ok");
        return true;
    } else if (action.equalsIgnoreCase("keepalive")) {
        lastKeepaliveTime = millis();
        sendResponse("ok");
        return true;
    }

    return false;
}

void UDPControl::sendResponse(const char* response) {
    udpCommand.beginPacket(udpCommand.remoteIP(), udpCommandPort);
    udpCommand.write((uint8_t*)response, strlen(response));
    udpCommand.endPacket();
}

void UDPControl::sendStatusMessage(float batteryVolt, uint8_t batteryPercent, int16_t motorSpeed[4]) {
    if (lastCommandReceived) {
        // Format the battery voltage with a precision of 2 decimal places
        char response[64];
        snprintf(response, sizeof(response), "batV:%.2f,batP:%d,mA:%d,mB:%d,mC:%d,mD:%d,:\n",
                 batteryVolt, batteryPercent, static_cast<int>(abs(motorSpeed[0])/255.0*100.0), static_cast<int>(abs(motorSpeed[1])/255.0*100.0), static_cast<int>(abs(motorSpeed[2])/255.0*100.0), static_cast<int>(motorSpeed[3]/255.0*100.0));
        
        udpStatus.beginPacket(lastCommandIP, udpStatusPort);
        udpStatus.write((uint8_t*)response, strlen(response));
        udpStatus.endPacket();
    }
}


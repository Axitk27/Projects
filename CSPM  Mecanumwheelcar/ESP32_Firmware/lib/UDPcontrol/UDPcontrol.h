/*    
    Programm intended to be used in the CCCE 2024 Mecanum Wheels Car Project.
    Copyright (C) 2024 Devraj Solanki and Edmund Jochim

    This file is the header file for the UDP control class.

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

#ifndef UDP_CONTROL_H
#define UDP_CONTROL_H

#include <WiFi.h>
#include <WiFiUdp.h>

class UDPControl {
public:
    UDPControl(const char* ssid, const char* password, unsigned int commandPort, unsigned int statusPort, IPAddress local_ip, IPAddress gateway, IPAddress subnet);

    bool setupAP();
    void begin();
    void handlePackets(int8_t move_dir[3]);
    void sendResponse(const char* response);
    void sendStatusMessage(float batteryVolt, uint8_t batteryPercent, int16_t motorSpeed[4]);

    bool lastCommandReceived;

private:
    bool decodeAndExecuteCommand(const String& command, int8_t move_dir[3]);

    const char* ssid;
    const char* password;
    unsigned int udpCommandPort;
    unsigned int udpStatusPort;
    IPAddress localIP;
    IPAddress gateway;
    IPAddress subnet;

    WiFiUDP udpCommand;
    WiFiUDP udpStatus;

    unsigned long lastKeepaliveTime;
    IPAddress lastCommandIP;
};

#endif

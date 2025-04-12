%% Clear all data
clc
clear all
close all

%% Creat a connection with MQTT Broker

clientid = "MatlabClient";  % Set your desired client ID
username = "admin";              % Your MQTT broker username
password = "iotlab2023team4";              % Your MQTT broker password
%%rootcert = "C:\Users\axitk\OneDrive\Desktop\THM Document\IOT Project\Graphical part\Certificate\_.thingspeak.com";

brokerAddress = "tcp://mt-labor.iem.thm.de";
port =20005;

mqClient = mqttclient(brokerAddress, Port=port, ClientID=clientid, ...
                      Username=username, Password=password);
mqClient.Connected;  % Connect to the MQTT broker
    
if (mqClient.Connected == true)
    disp("MQTT brocker Connection is done.......!!!!!!!")
else
    disp("MQTT brocker Connection failed")
end

%% Check the MQTT Connection

if (mqClient.Connected == true)
    disp("MQTT brocker Connection is Live")
else
    disp("MQTT brocker Connection failed")
end
%% Flush MQTT Client
 flush(mqClient);
 disp("Flushing is done")
%% to subscribe to a topic and read a data
mqttTopics = "IoT/test/random";
subscribe(mqClient,mqttTopics);
disp("Mqtt Subscription is done")

%% Read the any topic
MQTTData = read(mqClient,"Topic",mqttTopics)
data = jsondecode(MQTTData.Data(1))
%% Data reading for the angle 
%data = timetable();
%MQTTData = timetable();
timehistory = [];
V1data = [];
V2data = [];
V3data = [];
hold on;

% Plot each line separately
graph1 = plot(MQTTData.Time(1), data.phasevoltage_v1, 'LineWidth', 2);
graph2 = plot(MQTTData.Time(1), data.phasevoltage_v2, 'LineWidth', 2);
graph3 = plot(MQTTData.Time(1), data.phasevoltage_v3, 'LineWidth', 2);

xlabel('Time(HH:MM:SS)');
ylabel('Voltage(V)');
title('Three Phase Voltage');
grid on;
legend("V1", "V2","V3");

while true
    pause(1)
    try
        MQTTData = read(mqClient,"Topic",mqttTopics);
        time = MQTTData.Time(1);
        timehistory = [timehistory;time]
        data = jsondecode(MQTTData.Data(1));
        V1data = [V1data;data.phasevoltage_v1];
        V2data = [V2data;data.phasevoltage_v2];
        V3data = [V3data;data.phasevoltage_v3];
        set(graph1, 'XData',timehistory,'YData',V1data);
        set(graph2, 'XData',timehistory,'YData',V2data);
        set(graph3, 'XData',timehistory,'YData',V3data);
        %data = [data;MQTTData]
    catch exception
        disp(exception.message)
    end
end
%% Current Live data

timehistory = [];
I1data = [];
I2data = [];
I3data = [];
hold on;

% Plot each line separately
graph1 = plot(MQTTData.Time(1), data.current_l1, 'LineWidth', 2);
graph2 = plot(MQTTData.Time(1), data.current_l2, 'LineWidth', 2);
graph3 = plot(MQTTData.Time(1), data.current_l3, 'LineWidth', 2);

xlabel('Time(HH:MM:SS)');
ylabel('Current(Amp)');
title('Three Phase Current');
grid on;
legend("I1", "I2","I3");

while true
    pause(1)
    try
        MQTTData = read(mqClient,"Topic",mqttTopics);
        time = MQTTData.Time(1);
        timehistory = [timehistory;time];
        data = jsondecode(MQTTData.Data(1));
        I1data = [I1data;data.current_l1];
        I2data = [I2data;data.current_l2];
        I3data = [I3data;data.current_l3];
        set(graph1, 'XData',timehistory,'YData',I1data);
        set(graph2, 'XData',timehistory,'YData',I2data);
        set(graph3, 'XData',timehistory,'YData',I3data);
    catch exception
        disp(exception.message)
    end
end

%% Polar Plot Voltage
A1=0;
while true
    pause(1)
    try
        MQTTData = read(mqClient,"Topic",mqttTopics);
        data = jsondecode(MQTTData.Data(1));
        polarplot([0,A1], [0,data.phasevoltage_v1], '-xb', ...
                  [0,deg2rad(data.angle_v2v1)], [0,data.phasevoltage_v2], '-xr', ...
                  [0,deg2rad(data.angle_v3v1)], [0,data.phasevoltage_v3], '-xg');
        title('Polar Plot of Voltage(V)');
        grid on;
        legend("V1", "V2","V3");
    catch exception
        disp(exception.message)
    end
end

%% Polar Plot Current

while true
    pause(1)
    try
        MQTTData = read(mqClient,"Topic",mqttTopics);
        data = jsondecode(MQTTData.Data(1));
        polarplot([0,deg2rad(data.angle_l1v1)], [0,data.current_l1], '-xb', ...
                  [0,deg2rad(data.angle_l2v1)], [0,data.current_l2], '-xr', ...
                  [0,deg2rad(data.angle_l3v1)], [0,data.current_l3], '-xg');
        title('Polar Plot of Current(Amp)');
        grid on;
        legend("I1", "I2","I3");
    catch exception
        disp(exception.message)
    end
end

%% Unsubscribe to the topic

unsubscribe(mqClient)
disp("unsubscribe to the all topics")


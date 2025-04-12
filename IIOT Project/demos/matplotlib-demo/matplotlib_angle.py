import paho.mqtt.client as mqtt
import json
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

global phasevoltage_avg  # Access the global variable

# Set EMQX broker parameters
broker_address = "mt-labor.iem.thm.de"
port = 20005
topic = "IoT/test/random"

# Set client ID, username, and password
client_id = "admin"
username = "admin"
password = "iotlab2023team4"

# Create the trigonometric circle graph outside the callback function
fig, ax = plt.subplots(figsize=(6, 6))
circle = plt.Circle((0, 0), 1, color='gray', fill=False, linestyle='dashed')
ax.add_patch(circle)
line, = ax.plot([], [], label='', marker='o')  # Initialize the graph line

# Add labels and legend
ax.set_title('Trigonometric Circle with Angle')
ax.set_xlabel('Real')
ax.set_ylabel('Imaginary')
ax.legend()

# Display the graph in a non-blocking way
plt.grid(True)
plt.axhline(0, color='black', linewidth=0.5)
plt.axvline(0, color='black', linewidth=0.5)
plt.axis('equal')  # Ensure axes have the same scale
plt.ion()  # Enable interactive mode
plt.show(block=False)

# Callback function when a connection is established
def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Successfully connected to the EMQX broker")
        # Subscribe to the topic after successful connection
        client.subscribe(topic)
    else:
        print(f"Failed to connect to the EMQX broker, return code: {rc}")

def on_message(client, userdata, message):
    if message.topic == "IoT/test/random":
        payload = message.payload.decode("utf-8")
        print(f"Message received on topic {message.topic}: {payload}")
        data = json.loads(payload)

        # Retrieve values of phasevoltage_avg, phasevoltage_v1, phasevoltage_v2, phasevoltage_v3
        phasevoltage_avg = data.get("phasevoltage_avg", None)
        phasevoltage_v1 = data.get("phasevoltage_v1", None)
        phasevoltage_v2 = data.get("phasevoltage_v2", None)
        phasevoltage_v3 = data.get("phasevoltage_v3", None)

        # Define angles in degrees
        angle_degrees_avg = phasevoltage_avg
        angle_degrees_v1 = phasevoltage_v1
        angle_degrees_v2 = phasevoltage_v2
        angle_degrees_v3 = phasevoltage_v3

        # Convert angles to radians
        angle_radians_avg = np.radians(angle_degrees_avg)
        angle_radians_v1 = np.radians(angle_degrees_v1)
        angle_radians_v2 = np.radians(angle_degrees_v2)
        angle_radians_v3 = np.radians(angle_degrees_v3)

        # Coordinates of points to represent angles in the trigonometric circle
        x_avg, y_avg = np.cos(angle_radians_avg), np.sin(angle_radians_avg)
        x_v1, y_v1 = np.cos(angle_radians_v1), np.sin(angle_radians_v1)
        x_v2, y_v2 = np.cos(angle_radians_v2), np.sin(angle_radians_v2)
        x_v3, y_v3 = np.cos(angle_radians_v3), np.sin(angle_radians_v3)

        # Update axis data for each angle
        line_avg.set_data([0, x_avg], [0, y_avg])
        line_avg.set_label(f'Angle_avg = {angle_degrees_avg}°')

        line_v1.set_data([0, x_v1], [0, y_v1])
        line_v1.set_label(f'Angle_v1 = {angle_degrees_v1}°')

        line_v2.set_data([0, x_v2], [0, y_v2])
        line_v2.set_label(f'Angle_v2 = {angle_degrees_v2}°')

        line_v3.set_data([0, x_v3], [0, y_v3])
        line_v3.set_label(f'Angle_v3 = {angle_degrees_v3}°')

        # Update the graph
        ax.legend()
        plt.draw()  # Force the figure to redraw
        plt.pause(0.001)  # Short pause to allow the display to update

# Create additional lines for each phase angle
line_avg, = ax.plot([], [], label='Angle_avg')
line_v1, = ax.plot([], [], label='Angle_v1')
line_v2, = ax.plot([], [], label='Angle_v2')
line_v3, = ax.plot([], [], label='Angle_v3')

# Configure the MQTT client with client ID, username, and password
client = mqtt.Client(client_id)
client.username_pw_set(username, password)
client.on_connect = on_connect
client.on_message = on_message

# Connect to the EMQX broker
client.connect(broker_address, port, 60)

# Maintain the connection and process messages
client.loop_forever()

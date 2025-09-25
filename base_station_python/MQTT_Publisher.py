# PHASE 3 PROJECT: Listen sound, detect it and send signal to esp through MQTT
# live_audio_publisher.py

import tensorflow as tf
import tensorflow_hub as hub
import numpy as np
import csv
import sounddevice as sd
import time
import paho.mqtt.client as mqtt

# --- Model Loading (Same as Phase 1) ---
model = hub.load('https://tfhub.dev/google/yamnet/1')
class_map_path = model.class_map_path().numpy()
class_names = []
with open(class_map_path) as csv_file:
    reader = csv.reader(csv_file)
    next(reader)
    for row in reader:
        class_names.append(row[2])
# ------------------------------------

# --- MQTT Configuration ---
MQTT_BROKER = "test.mosquitto.org"
MQTT_PORT = 1883
MQTT_COMMAND_TOPIC = "soundalert/commands" # This MUST match the ESP32 sketch

# --- MQTT Callback Functions ---
def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Connected to MQTT Broker!")
    else:
        print(f"Failed to connect, return code {rc}\n")

# --- Setup MQTT Client ---
client = mqtt.Client()
client.on_connect = on_connect
client.connect(MQTT_BROKER, MQTT_PORT)
client.loop_start() # Start a background thread to handle MQTT communication

# --- Target Sounds ---
# Using a dictionary now for easier message lookup
TARGET_SOUNDS = {
    "Fire alarm": "FIRE",
    "Siren": "SIREN",
    "Smoke detector, smoke alarm": "FIRE", # Map smoke alarm to same FIRE alert
    "Dog": "DOG",
    "Animal": "ANIMAL",
    "Speech": "SPEECH",
    "Domestic animals, pets": "ANIMAL2", 
    "Rodents, rats, mice": "ANIMAL3" , 
    "Vehicle": "VEHICLE", 
}

CONFIDENCE_THRESHOLD = 0.2 # Tune this value (0.0 to 1.0)

# --- Audio Recording Parameters ---
SAMPLE_RATE = 16000
DURATION_SECS = 1

print("Starting live audio publisher...")
print(f"Listening for: {', '.join(TARGET_SOUNDS.keys())}")

try:
    while True:
        # Record and process audio (same as Phase 1)
        audio_chunk = sd.rec(int(DURATION_SECS * SAMPLE_RATE), samplerate=SAMPLE_RATE, channels=1, dtype='float32')
        sd.wait()
        scores, _, _ = model(audio_chunk.flatten())
        scores = scores.numpy().mean(axis=0)
        top_class_index = scores.argmax()
        top_class_name = class_names[top_class_index]
        top_score = scores[top_class_index]

        print(f"Heard: '{top_class_name}' (Score: {top_score:.2f})")

        # --- THIS IS THE NEW PART ---
        # Check if the detected sound is a target and meets the confidence threshold
        if top_class_name in TARGET_SOUNDS and top_score > CONFIDENCE_THRESHOLD:
            alert_message = TARGET_SOUNDS[top_class_name]
            print(f"--- DETECTED TARGET! Publishing '{alert_message}' to MQTT ---")
            client.publish(MQTT_COMMAND_TOPIC, alert_message)
            time.sleep(5) # Add a delay after sending an alert to avoid spamming

except KeyboardInterrupt:
    print("\nStopping the publisher.")
    client.loop_stop()
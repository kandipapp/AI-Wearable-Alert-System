#PHASE 1 PROJECT: listen sound and output the type of sound

# live_audio_classifier.py
import tensorflow as tf
import tensorflow_hub as hub
import numpy as np
import csv
import sounddevice as sd
import time

# --- Model Loading (same as before) ---
model = hub.load('https://tfhub.dev/google/yamnet/1')
class_map_path = model.class_map_path().numpy()
class_names = []
with open(class_map_path) as csv_file:
    reader = csv.reader(csv_file)
    next(reader)
    for row in reader:
        class_names.append(row[2])
# ------------------------------------

# --- Target Sounds ---
# These are the sounds we want to detect. Find their names in the yamnet_class_map.csv
TARGET_SOUNDS = ["Fire alarm", "Siren", "Smoke detector, smoke alarm", "Dog", "Shout", "Speech", "Cat"] 

# --- Audio Recording Parameters ---
SAMPLE_RATE = 16000
DURATION_SECS = 0.975 # Analyze audio in 1-second chunks

print("Starting live audio classification...")
print(f"Listening for: {', '.join(TARGET_SOUNDS)}")

try:
    while True:
        # Record audio for the specified duration
        print("\nListening...")
        audio_chunk = sd.rec(int(DURATION_SECS * SAMPLE_RATE), samplerate=SAMPLE_RATE, channels=1, dtype='float32')
        sd.wait() # Wait for recording to complete

        # Run inference
        scores, _, _ = model(audio_chunk.flatten())
        scores = scores.numpy().mean(axis=0) # Average predictions over the chunk

        # Find the top prediction
        top_class_index = scores.argmax()
        top_class_name = class_names[top_class_index]
        top_score = scores[top_class_index]
        
        print(f"Most likely sound: '{top_class_name}' with score {top_score:.2f}")

        # Check if the top prediction is one of our targets and has a decent score
        if top_class_name in TARGET_SOUNDS and top_score > 0.3: # 0.3 is a confidence threshold, you can tune this
            print("-----------------------------------------")
            print(f"!!! ALERT: Detected '{top_class_name}' !!!")
            print("-----------------------------------------")
        
        time.sleep(0.5) # Small delay to avoid flooding the console

except KeyboardInterrupt:
    print("\nStopping the classifier.")
# AI-Wearable-Alert-System (AWAS)

![photo_2025-09-25_18-31-46](https://github.com/user-attachments/assets/128b63a2-1df8-4cc7-ace3-70b7f5ba8b22)

An AI-powered wearable system, the **AI Wearable Alert System (AWAS)**, designed to enhance situational awareness for the deaf and hard of hearing. This project detects critical environmental sounds using a machine learning model and wirelessly transmits alerts to a custom 3D-printed gauntlet, which provides haptic and visual feedback to the user.

![photo_2025-09-25_18-31-36](https://github.com/user-attachments/assets/b1002c74-6efe-4403-9209-3c57712fa3a7)

![photo_2025-09-25_18-31-42](https://github.com/user-attachments/assets/1f46c1e0-35da-448c-8c2b-20737ff9d18c)

---

## 🚀 Key Features

- **Real-Time Sound Recognition:** Utilizes Google's YAMNet model via TensorFlow and TensorFlow Hub to classify over 500 different audio events.
- **Wireless Communication:** Employs the lightweight and efficient MQTT protocol for near-instant alert transmission over Wi-Fi.
- **Custom Wearable Gauntlet:** Features a unique, ergonomic gauntlet designed in Autodesk Inventor and fabricated using a 3D printer.
- **Differentiated Haptic & Visual Alerts:** Provides distinct feedback patterns using multiple LEDs and a vibration motor to help the user distinguish between different alerts (e.g., fire alarm vs. siren).

---

## 🛠️ System Architecture

The system is composed of two main components:

1.  **The Base Station (Listener):** A Python script running on a host computer that captures ambient audio, uses the AI model to classify sounds, and publishes alerts.
2.  **The Wearable Gauntlet (Notifier):** An ESP32-based device that subscribes to the alert channel and activates physical feedback mechanisms.

---

## ⚙️ Hardware & Components

### Base Station
- Laptop (Windows/macOS/Linux)
- Microphone (built-in or USB)

### Wearable Gauntlet
- **Microcontroller:** ESP32 Development Board
- **Visual Feedback:** 4x 5mm LEDs (Red, Blue, Yellow, Green) with 220 Ohm resistors
- **Haptic Feedback:** Coin Vibrating Motor Disc
- **Enclosure:** Custom Gauntlet (designed in Autodesk Inventor, 3D Printed in PLA filament)

---

## 💾 Software & Setup

### 1. Base Station (Python Listener)
This script listens for sounds and publishes MQTT messages.

- **Prerequisites:** Python 3.9+
- **Key Libraries:** `tensorflow`, `tensorflow_hub`, `sounddevice`, `paho-mqtt`

**Setup:**
```bash
# 1. Clone the repository
git clone https://github.com/kandipapp/AI-Wearable-Alert-System.git
cd AI-Wearable-Alert-System/base_station_python

# 2. Create and activate a virtual environment
python -m venv venv_AWAS
source venv_AWAS/bin/activate  # On Windows: venv\Scripts\activate

# 3. Install dependencies
pip install -r requirements.txt 

# 4. Run the script
python MQTT_Publisher.py
```

### 2. Wearable (Arduino/ESP32)
This sketch runs on the ESP32 to receive MQTT messages and trigger alerts.

- **Prerequisites:** Arduino IDE, ESP32 Board Support Package
- **Key Libraries:** `WiFi.h`, `PubSubClient.h` (by Nick O'Leary)

**Setup:**
1.  Open `wearable_arduino/WearableAlert.ino` in the Arduino IDE.
2.  Install the `PubSubClient` library via the Library Manager.
3.  Modify the sketch to include your Wi-Fi credentials (`ssid` and `password`).
4.  Upload the sketch to your ESP32 board.

---

## 🔮 Future Work & Improvements

- [ ] **Migrate to Raspberry Pi:** Move the Python script and replace the ESP32 to a Raspberry Pi 4 using the more efficient TensorFlow Lite model to create a standalone Wearable Device without Base Station. 
- [ ] **Integrate Battery Power:** Add a LiPo battery and charging circuit to the gauntlet to make it fully portable.
- [ ] **User Customization:** Develop a simple web interface to allow users to customize alert patterns.
- [ ] **Expand Sound Library:** Explore training a custom model to recognize user-specific sounds (e.g., a specific doorbell).

---

## 📜 License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.


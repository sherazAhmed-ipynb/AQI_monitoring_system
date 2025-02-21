# Air Quality Index (AQI) Monitoring System

This project is an **Air Quality Index (AQI) Monitoring System** that uses multiple sensors to measure air quality parameters and sends the data to an ESP32 for further processing or cloud integration.

## Overview
The system utilizes the following sensors:
- **MQ-5** (LPG, natural gas, and CO detection)
- **MQ-7** (CO detection)
- **MQ-135** (Air quality detection, CO2, NH3, Benzene, etc.)
- **DHT11** (Temperature and Humidity measurement)
- **Dust Sensor** (Particulate matter detection)

The collected data is processed to calculate an **AQI value**, which is displayed on an LCD and transmitted to an ESP32 for further use. The ESP32 then sends the data to **Firebase** for remote monitoring and analysis.

## Features
- Reads air quality parameters using **MQ series gas sensors**.
- Measures **temperature and humidity** using a **DHT11** sensor.
- Detects **dust concentration** using a dust sensor.
- Displays real-time data on a **16x2 LCD**.
- Calculates **AQI** based on sensor readings.
- Sends collected data to an **ESP32** via Serial communication.
- **ESP32** connects to **Firebase** to store and monitor sensor data remotely.

## Hardware Requirements
- **Arduino Board** (Uno, Mega, etc.)
- **ESP32** (for IoT integration)
- **MQ-5 Gas Sensor**
- **MQ-7 Gas Sensor**
- **MQ-135 Gas Sensor**
- **DHT11 Temperature & Humidity Sensor**
- **Dust Sensor**
- **16x2 LCD Display**
- **WiFi Connection** (for Firebase integration)
- **Jumper Wires & Breadboard**

## Installation & Usage
1. **Clone this repository**
   ```sh
   git clone https://github.com/yourusername/AQI-Monitoring.git
   ```
2. **Install dependencies**
   - Install the required Arduino libraries:
     - `DHT.h` (for DHT11 sensor)
     - `LiquidCrystal.h` (for LCD display)
     - `Firebase_ESP_Client.h` (for Firebase connection on ESP32)
   
3. **Upload the Code**
   - Open the `.ino` file in the Arduino IDE.
   - Select the correct board and COM port.
   - Upload the **Arduino code** to read sensor data.
   - Upload the **ESP32 code** to connect with Firebase.

4. **View Sensor Readings**
   - Check the LCD display for live sensor values.
   - Open the Serial Monitor (9600 baud) to view transmitted data.
   - Visit Firebase to monitor data remotely.

## AQI Calculation
The system calculates AQI based on weighted values from different sensors:
```cpp
float aqi = (0.2 * mq5Norm) + (0.2 * mq7Norm) + (0.2 * mq135Norm) + (0.2 * humidityNorm) + (0.2 * dustNorm);
```
This value is then mapped to an AQI scale:
- **0 - 100** → Good
- **101 - 200** → Moderate
- **201 - 300** → Poor
- **301 - 400** → Unhealthy
- **401+** → Very Unhealthy

## Firebase Integration
The ESP32 connects to **Firebase Realtime Database** to upload sensor data:
- **WiFi Credentials**: Modify the `WIFI_SSID` and `WIFI_PASSWORD` in the ESP32 code.
- **Firebase API Key**: Set your `DATABASE_URL` and `API_KEY` in the ESP32 code.
- **Data Format**:
  ```json
  {
    "mq5": value,
    "mq7": value,
    "mq135": value,
    "humidity": value,
    "temperature": value,
    "dust": value
  }
  ```
- The ESP32 reads the sensor data from the Arduino via **Serial2** communication, processes it, and uploads it to Firebase.

## Future Enhancements
- Integration with **IoT platforms** like **Thingspeak** or **Blynk**.
- Implementing **cloud storage** for long-term air quality monitoring.
- Adding **mobile app support** for remote monitoring.
- Enhancing **data visualization** with Firebase analytics.

## License
This project is open-source and available under the **MIT License**.

## Contributing
Feel free to **fork** this repo, submit **pull requests**, or suggest improvements!

---
🚀 **Stay Safe, Breathe Clean!**


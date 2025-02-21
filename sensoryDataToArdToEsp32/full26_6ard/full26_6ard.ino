#include <DHT.h>
#include <LiquidCrystal.h>

#define MQ5_PIN A0
#define MQ7_PIN A1
#define MQ135_PIN A2
#define DHT_PIN 8
#define DHT_TYPE DHT11
#define DUST_SENSOR_LED_PIN 9
#define DUST_SENSOR_VOUT_PIN A3

DHT dht(DHT_PIN, DHT_TYPE);
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

void setup() {
  Serial.begin(9600); // Initialize Serial communication
  dht.begin(); // Initialize DHT sensor
  pinMode(DUST_SENSOR_LED_PIN, OUTPUT); // Set the LED pin as output
  lcd.begin(16, 2); // Initialize the LCD with 16 columns and 2 rows
  lcd.print("Initializing...");
  delay(2000);
}

float normalize(float value, float min, float max) {
  return (value - min) / (max - min) * 100.0;
}

float calculateAQI(float mq5, float mq7, float mq135, float humidity, float dust) {
  float mq5Norm = normalize(mq5, 0, 1000); // Assume 0-1000 ppm range for MQ5
  float mq7Norm = normalize(mq7, 0, 1000); // Assume 0-1000 ppm range for MQ7
  float mq135Norm = normalize(mq135, 0, 1000); // Assume 0-1000 ppm range for MQ135
  float dustNorm = normalize(dust, 0, 500); // Assume 0-500 µg/m³ range for dust
  float humidityNorm = humidity; // Use humidity as is, since it is already in 0-100% range

  // Weight the normalized values (arbitrary weights)
  float aqi = (0.2 * mq5Norm) + (0.2 * mq7Norm) + (0.2 * mq135Norm) + (0.2 * humidityNorm) + (0.2 * dustNorm);
  
  // Map the combined value to a 0-500 AQI scale
  return (aqi / 100.0) * 500.0;
}

String getAQILevel(float aqi) {
  if (aqi <= 100) {
    return "Good";
  } else if (aqi <= 200) {
    return "Moderate";
  } else if (aqi <= 300) {
    return "Poor";
  } else if (aqi <= 400) {
    return "Unhealthy";
  } else {
    return "Very Unhealthy";
  }
}

void loop() {
  // Read gas sensor values
  int sensorValueMQ5 = analogRead(MQ5_PIN);  // Read the analog input for MQ-5
  float voltageMQ5 = sensorValueMQ5 * (5.0 / 1023.0);  // Convert the analog reading (0 - 1023) to a voltage (0 - 5V)
  float ppmMQ5 = voltageMQ5 * 100;  // Convert voltage to ppm (approximation)

  int sensorValueMQ7 = analogRead(MQ7_PIN);  // Read the analog input for MQ-7
  float voltageMQ7 = sensorValueMQ7 * (5.0 / 1023.0);  // Convert the analog reading (0 - 1023) to a voltage (0 - 5V)
  float ppmMQ7 = voltageMQ7 * 100;  // Convert voltage to ppm (approximation)

  int sensorValueMQ135 = analogRead(MQ135_PIN);  // Read the analog input for MQ-135
  float voltageMQ135 = sensorValueMQ135 * (5.0 / 1023.0);  // Convert the analog reading (0 - 1023) to a voltage (0 - 5V)
  float ppmMQ135 = voltageMQ135 * 100;  // Convert voltage to ppm (approximation)

  // Read DHT11 sensor values
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // Read dust sensor value
  digitalWrite(DUST_SENSOR_LED_PIN, LOW); // Power on the LED
  delayMicroseconds(280); // Wait 280us
  int dustValue = analogRead(DUST_SENSOR_VOUT_PIN); // Read the dust sensor value
  digitalWrite(DUST_SENSOR_LED_PIN, HIGH); // Power off the LED
  delayMicroseconds(9680); // Wait 9680us

  // Calculate AQI
  float aqi = calculateAQI(ppmMQ5, ppmMQ7, ppmMQ135, humidity, dustValue);
  String aqiLevel = getAQILevel(aqi);

  // Display sensor values on LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("MQ5: "); lcd.print(ppmMQ5); lcd.print(" ppm");
  lcd.setCursor(0, 1);
  lcd.print("MQ7: "); lcd.print(ppmMQ7); lcd.print(" ppm");
  delay(2000); // Wait for 2 seconds

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: "); lcd.print(temperature); lcd.print("C");
  lcd.setCursor(0, 1);
  lcd.print("Humid: "); lcd.print(humidity); lcd.print("%");
  delay(2000); // Wait for 2 seconds

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("MQ135:"); lcd.print(ppmMQ135); lcd.print(" ppm");
  lcd.setCursor(0, 1);
  lcd.print("Dust: "); lcd.print(dustValue); lcd.print(" ppm");
  delay(2000); // Wait for 2 seconds

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("AQI: "); lcd.print(aqi);
  lcd.setCursor(0, 1);
  lcd.print(aqiLevel);
  delay(2000); // Wait for 2 seconds

  // Send values to ESP32
  Serial.print(ppmMQ5); Serial.print(",");
  Serial.print(ppmMQ7); Serial.print(",");
  Serial.print(ppmMQ135); Serial.print(",");
  Serial.print(humidity); Serial.print(",");
  Serial.print(temperature); Serial.print(",");
  Serial.print(dustValue); Serial.print(",");
  Serial.println(aqi);

  delay(2000); // Wait for 2 seconds before the next reading
}

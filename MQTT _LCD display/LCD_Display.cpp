#include <WiFi.h>
#include <Wire.h>
#include <PubSubClient.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>
#include "FS.h"

void reconnect();
void setup_wifi();

#define IR_SENSOR_PIN 34

#define wifi_ssid "WE_ED0340"
#define wifi_password "alwaalaa12257*"
#define mqtt_server "192.168.1.3" // Define the MQTT server IP address

#define ir_sensor_topic "sensor/IR/distance" // Define the MQTT topic for IR sensor data
#define EEPROM_SIZE 512                      // Define the EEPROM size in bytes

WiFiClient espClient;
PubSubClient client(espClient);

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup()
{
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();

  pinMode(IR_SENSOR_PIN, INPUT);

  EEPROM.begin(EEPROM_SIZE); // Initialize EEPROM with the defined size

  setup_wifi();
  client.setServer(mqtt_server, 1883); // Set up the MQTT server and port
}

void setup_wifi()
{
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);

  WiFi.begin(wifi_ssid, wifi_password);

  // Wait until the connection is established
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect()
{
  // Loop until the client is connected to the MQTT server
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    // Try to connect with a unique client ID
    if (client.connect("ESP32Client"))
    {
      Serial.println("connected");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void store_local_data(int value)
{
  EEPROM.put(0, value); // Store the integer value at address 0 in EEPROM
  EEPROM.commit();      // Commit the changes to EEPROM to make sure data is saved
}

int read_local_data()
{
  int value;
  EEPROM.get(0, value); // Read the integer value from address 0 in EEPROM
  return value;
}

void loop()
{
  if (!client.connected())
  { // Check if the MQTT client is connected
    reconnect();
  }
  client.loop();

  // Read the IR sensor value
  int rawSensorValue = analogRead(IR_SENSOR_PIN);

  // Map the raw sensor value (0-4095) to a range of 1-100
  int mappedSensorValue = map(rawSensorValue, 0, 4095, 1, 100);

  // Display the mapped sensor value on the LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("IR Sensor:");
  lcd.setCursor(0, 1);
  lcd.print(mappedSensorValue);

  // Publish the sensor value to the MQTT topic
  Serial.print("IR Sensor Reading: ");
  Serial.println(mappedSensorValue);

  if (WiFi.status() == WL_CONNECTED)
  {                                                                           // Check if WiFi is connected
    client.publish(ir_sensor_topic, String(mappedSensorValue).c_str(), true); // Publish sensor value to MQTT
    store_local_data(mappedSensorValue);                                      // Store the sensor value in EEPROM
  }
  else
  {
    Serial.println("WiFi not connected, storing data locally.");
    store_local_data(mappedSensorValue); // Store the sensor value in EEPROM if WiFi is not connected
  }

  delay(2000);
}

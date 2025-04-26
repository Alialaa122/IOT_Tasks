#include <Arduino.h>
#include <ESP32Servo.h>

// Create an object from class Servo
Servo myservo;

const int irSensorPin = 4; // Pin for the IR sensor
const int servoPin = 5;    // Pin for the servo motor

void setup() {
  pinMode(irSensorPin, INPUT); // Initialize pin for IR sensor
  myservo.attach(servoPin);    // Attach pin for controlling servo motor
  Serial.begin(115200);        // Start serial communication
}

void loop() {
  // Read analog value from IR sensor
  int irread = analogRead(irSensorPin);                 // Read the analog value from the IR sensor
  int map_read = map(irread, 0, 4095, 1, 100);          // Map the value to a range of 1-100
  Serial.println(map_read);                             // Print the mapped value to the serial monitor

  // Check if the mapped value is less than or equal to 30
  if (map_read <= 30) {  
    // Open the door by moving the servo to 0 degrees
    myservo.write(0);
    delay(2000);  // Wait for 2 seconds
    // Close the door by moving the servo to 180 degrees
    myservo.write(180);
  } 
  // Check if the mapped value is greater than or equal to 90
  else if (map_read >= 90) {  
    // Make sure that the door is closed
    myservo.write(180);
  }

  delay(500); // Short delay before the next loop iteration
}
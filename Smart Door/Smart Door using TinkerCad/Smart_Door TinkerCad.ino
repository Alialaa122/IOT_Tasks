#include <Servo.h>  // Use the standard Servo library

Servo myservo;     // Create an object from class Servo

const int irSensorPin = A0; // Analog pin for the IR sensor
const int servoPin = 9;     // Pin for the servo motor

void setup() {
  pinMode(irSensorPin, INPUT); // Initialize pin for IR sensor
  myservo.attach(servoPin);    // Attach pin for controlling servo motor
  Serial.begin(9600);          // Start serial communication at 9600 bps
}

void loop() {
  int irread = analogRead(irSensorPin);           // Read the analog value from the IR sensor
  int map_read = map(irread, 0, 1023, 1, 100);    // Map the value to a range of 1-100
  Serial.println(map_read);                       // Print the mapped value to the serial monitor

  if (map_read <= 30) {  
    myservo.write(0);    // Open the door by moving the servo to 0 degrees
    delay(2000);         // Wait for 2 seconds
    myservo.write(180);  // Close the door by moving the servo to 180 degrees
  } else if (map_read >= 90) {  
    myservo.write(180);  // Make sure that the door is closed
  }

  delay(500); // Short delay before the next loop iteration
}

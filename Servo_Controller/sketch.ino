#include <Arduino.h>
#include <ESP32Servo.h>
#include <Keypad.h>

// create an object from Servo Class
Servo myservo;
//Define number of Rows and Columns
const byte ROWS = 4;
const byte COLS = 4;
//Define Keys on the Keypad
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

//Define pins number of rows and columns of the keypad
byte rowPins[ROWS] = {13, 12, 14, 27};
byte colPins[COLS] = {26, 25, 33, 32};

// Create a keypad object
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

String inputString = ""; // Buffer for custom angle input
bool inputMode = false;  // Flag to track custom angle input mode

void setup() {

  myservo.attach(5);// attach pin 5 to a servo
  Serial.begin(115200);
}

void loop() {
  char customKey = customKeypad.getKey(); // get the key pressed on the keypad
  
  if (customKey) { // If a key is pressed
    if (inputMode) { // If in custom angle input mode
      if (customKey >= '0' && customKey <= '9') { // If the key is a digit
        // Collect digits for custom angle
        inputString += customKey;
        Serial.print("Current input: ");
        Serial.println(inputString);
      } else if (customKey == '#') { // if a key is # 
        setCustomAngle();//then Confirm and set custom angle
        inputMode = false; // Exit custom input mode
        inputString = "";  // Clear input buffer
      } else if (customKey == '*') {
        // Cancel custom angle input
        inputMode = false;
        inputString = "";  // Clear input buffer
        Serial.println("Custom angle input cancelled.");
      }
    } else { // If not in custom angle input mode
      switch (customKey) {
        case '1':
          myservo.write(30);
          break;
        case '2':
          myservo.write(45);
          break;
        case '3':
          myservo.write(60);
          break;
        case '4':
          myservo.write(90);
          break;
        case '5':
          myservo.write(120);
          break;
        case '6':
          myservo.write(135);
          break;
        case '7':
          myservo.write(180);
          break;
        case '*':
          myservo.detach(); // Detach the Servo
          break;
        case '#': 
          if (!myservo.attached()) {// if servo is not attached 
            myservo.attach(5);//then attach it ti the pin 5 
          }
          myservo.write(0); // set the servo to the angle 0
          break;
        case 'A':
          inputMode = true; // Enter custom angle input mode
          Serial.println("Enter custom angle (0-180): ");
          break;
        default:
          break;
      }
    }
  }
}

//BONUS

void setCustomAngle() {
  int angle = inputString.toInt();// covert string to integer
  if (angle < 0) angle = 0; // if it is lower than 0 Set it to 0
  if (angle > 180) angle = 180; // if it is greater than 180 Set it to 180
  myservo.write(angle); // Set servo to the custom angle
  Serial.print("Setting custom angle: ");
  Serial.println(angle);
}


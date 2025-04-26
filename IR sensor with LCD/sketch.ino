#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

// Define the number of columns and rows for the LCD
int lcdColumns = 16;
int lcdRows = 2;

// Initialize the LCD with I2C address 0x27 and specified columns and rows
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);

void setup() {

pinMode(4, INPUT); // intialize pin 4 as an input for ir sensor
Serial.begin(115200);
  lcd.init();  // Initialize the LCD
  lcd.backlight(); // Turn on the backlight


  lcd.setCursor(0, 0);  // Set cursor to the first row and column
  lcd.print("Initializing...");
  delay(2000); // Wait for 2 seconds
  lcd.clear(); // Clear the LCD screen

}

void loop() {
  int irread=analogRead(4); // Read the analog value from pin 4 (IR sensor)
  if(irread<20){  // If the IR sensor value is less than 20
    lcd.setCursor(0, 0); // Set cursor to the first row and column
    lcd.print("No input detected");
    lcd.setCursor(0, 1); // Set cursor to the second row and column
    lcd.print("Please connect");
  }
  else{

    // Map the IR sensor value to a distance range (1.0 to 25.0 mm)
    float distance = map(irread, 0, 4095, 1.0, 25.0);

    lcd.setCursor(0, 0); // Set cursor to the first row and column
    lcd.print("IR Value: ");
    lcd.print(irread); // Print the IR sensor value

    
    lcd.setCursor(0, 1); // Set cursor to the second row and column
    lcd.print("Dist: ");
    lcd.print(distance, 1); // Display distance with one decimal place
    lcd.print(" mm");
  }
  delay(500);   // Wait for 500 milliseconds before repeating the loop

}

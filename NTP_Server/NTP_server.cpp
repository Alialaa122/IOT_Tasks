#include <WiFi.h>                 
#include "time.h"                 
#include "sntp.h"                 // Library for SNTP (Simple Network Time Protocol) functions
#include <LiquidCrystal_I2C.h>    
#include "FS.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);   

// Wi-Fi credentials
const char* ssid       = "WE_ED0340";            
const char* password   = "alwaalaa12257*";       

// NTP server addresses and time configuration
const char* ntpServer1 = "pool.ntp.org";         
const char* ntpServer2 = "time.nist.gov";        
const long  gmtOffset_sec = 7200;                // Offset from GMT/UTC in seconds (2 hours)
const int   daylightOffset_sec = 3600;           // Offset for daylight saving time in seconds (1 hour)

// Function to print the local time to the LCD
void printLocalTime()
{
    struct tm timeinfo;                         // Structure to hold time information
    if (!getLocalTime(&timeinfo)) {             // Try to get the local time
        Serial.println("No time available (yet)");  // Print message if time is not available
        return;                                 // Exit the function if time is not available
    }

    // Display Date & Time on the LCD
    lcd.clear();                                
    lcd.print(&timeinfo, "%B %d %Y");           // Print the date in the format "Month Day Year"
                                                // Example: "November 22 2022"
    lcd.setCursor(0, 1);                        
    lcd.print(&timeinfo, "%a, %H:%M:%S");       // Print the time in the format "Day, Hour:Minute:Second"
                                                // Example: "Tuesday, 10:30:05"
}

// Callback function that is called when the time is adjusted via NTP
void timeavailable(struct timeval *t)
{
    Serial.println("Got time adjustment from NTP!");  // Print a message when time is synchronized
    printLocalTime();                                 // Update the time display on the LCD
}

// Setup function that runs once when the ESP32 starts
void setup()
{
    Serial.begin(115200);                          // Start the serial communication at 115200 baud rate

    // Setup and initialize the LCD
    lcd.init();                                    // Initialize the LCD
    lcd.backlight();                               // Turn on the LCD backlight

    // Set the notification callback function for time synchronization
    sntp_set_time_sync_notification_cb(timeavailable);

    // Enable NTP server address acquisition via DHCP
    sntp_servermode_dhcp(1);

    // Configure NTP servers and timezone/daylight offset
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer1, ntpServer2);

    // Connect to Wi-Fi network
    Serial.printf("Connecting to %s ", ssid);     // Print connecting message to Serial Monitor
    lcd.clear();                                  // Clear the LCD screen
    lcd.print("Connecting to ");                  // Print "Connecting to " on the LCD
    lcd.setCursor(0, 1);                          // Move the cursor to the first column of the second row
    lcd.print(ssid);                              // Print the SSID on the LCD

    WiFi.begin(ssid, password);                   // Start connecting to Wi-Fi network
    while (WiFi.status() != WL_CONNECTED) {       // Wait until the ESP32 is connected to Wi-Fi
        delay(500);                               // Wait 500ms before checking again
        Serial.print(".");                        // Print a dot to indicate the connection process
    }
    Serial.println(" CONNECTED");                 // Print a connected message to Serial Monitor
    lcd.clear();                                  // Clear the LCD screen
    lcd.print("CONNECTED");                       // Display "CONNECTED" on the LCD
    delay(2000);                                  // Wait for 2 seconds
}

// Loop function that runs repeatedly after setup()
void loop()
{
    delay(1000);                                  // Wait 1 second
    printLocalTime();                             // Print the local time to the LCD
}





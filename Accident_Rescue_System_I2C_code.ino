#include <Wire.h>
#include <MPU6050_tockn.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>

// Initialize MPU6050
MPU6050 mpu6050(Wire);

// Define constants
const char* phoneNumber = "+919482892959";  // Predefined number
const char* message = "Accident detected. Location: https://maps.google.com/?q=";

// Create objects
TinyGPSPlus gps;
SoftwareSerial gpsSerial(10, 11);  // GPS module (TX=10, RX=11)
SoftwareSerial gsmSerial(3, 2);    // GSM module (TX=3, RX=2)
LiquidCrystal_I2C lcd(0x27, 16, 2);  // I2C LCD at address 0x27

bool accidentDetected = false;

void setup() {
  Serial.begin(9600);
  gpsSerial.begin(9600);  // Initialize GPS serial communication
  gsmSerial.begin(9600);  // Initialize GSM serial communication
  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);  // Calibrate gyro

  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();

  Serial.println("System Initialized");
}

void loop() {
  mpu6050.update();  // Update MPU6050 sensor readings

  float roll = mpu6050.getAngleX();  // Get the roll angle (angleX)

  Serial.print("angleX: ");
  Serial.print(roll);

  // Define conditions based on roll angle
  if (roll >= 1 && roll <= 100) {
    Serial.println("\tNormal Condition.");
  } 
  else if (roll >= -150 && roll <= -4) {
    Serial.println("\tAccident Occurred!");

    if (!accidentDetected) {
      accidentDetected = true;

      // Display "Accident Occurred" on LCD
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Accident Occurred!");

      sendSMS(phoneNumber, message);

      // Display "SMS Sent Successfully" on LCD
      lcd.setCursor(0, 1);
      lcd.print("SMS Sent");

      makeContinuousCalls(phoneNumber);
    }
  }

  delay(500);  // Small delay for readability
}

void sendSMS(const char* number, const String& msg) {
  gsmSerial.print("AT+CMGF=1\r");  // Set SMS to text mode
  delay(100);
  gsmSerial.print("AT+CMGS=\"");
  gsmSerial.print(number);
  gsmSerial.print("\"\r");
  delay(100);
  gsmSerial.print(msg);
  delay(100);
  gsmSerial.write(26);  // ASCII code for CTRL+Z to send SMS
}

void makeContinuousCalls(const char* number) {
  int retryCount = 3;  // Limit to 3 call attempts

  while (retryCount > 0) {
    gsmSerial.print("ATD");
    gsmSerial.print(number);
    gsmSerial.print(";\r");
    delay(20000);  // Call duration

    gsmSerial.print("ATH\r");  // Hang up
    delay(1000);  // Delay between calls

    retryCount--;
  }
}

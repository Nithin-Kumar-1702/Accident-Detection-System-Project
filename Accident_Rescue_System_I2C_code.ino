#include <Wire.h>
#include <MPU6050_tockn.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>

MPU6050 mpu6050(Wire);
TinyGPSPlus gps;

SoftwareSerial gpsSerial(10, 11);
SoftwareSerial gsmSerial(3, 2);

LiquidCrystal_I2C lcd(0x27, 16, 2);

const char phoneNumber[] = "+919482892959";
bool accidentDetected = false;

void sendSMSWithLocation(const char *number);
void makeCalls(const char *number);
void readGPS();

void setup()
{
    Serial.begin(9600);
    gpsSerial.begin(9600);
    gsmSerial.begin(9600);

    Wire.begin();
    mpu6050.begin();
    mpu6050.calcGyroOffsets(true);

    lcd.init();
    lcd.backlight();
    lcd.clear();
    lcd.print("System Ready");
}

void loop()
{
    readGPS();
    mpu6050.update();

    float roll = mpu6050.getAngleX();

    Serial.print("Roll: ");
    Serial.println(roll);

    if (roll >= -150 && roll <= -4)
    {
        if (!accidentDetected)
        {
            accidentDetected = true;

            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Accident!");

            sendSMSWithLocation(phoneNumber);

            lcd.setCursor(0, 1);
            lcd.print("SMS Sent");

            makeCalls(phoneNumber);
        }
    }

    delay(500);
}

void readGPS()
{
    while (gpsSerial.available() > 0)
    {
        gps.encode(gpsSerial.read());
    }
}

void sendSMSWithLocation(const char *number)
{
    char message[160];

    if (gps.location.isValid())
    {
        float latitude = gps.location.lat();
        float longitude = gps.location.lng();

        sprintf(message,"Accident detected! Location:\nhttps://maps.google.com/?q=%.6f,%.6f", latitude, longitude);
    }
    else
    {
        sprintf(message, "Accident detected! GPS location not available.");
    }

    gsmSerial.println("AT+CMGF=1");
    delay(1000);

    gsmSerial.print("AT+CMGS=\"");
    gsmSerial.print(number);
    gsmSerial.println("\"");
    delay(1000);

    gsmSerial.print(message);
    delay(500);

    gsmSerial.write(26);
    delay(3000);
}

void makeCalls(const char *number)
{
    int retry = 3;

    while (retry > 0)
    {
        gsmSerial.print("ATD");
        gsmSerial.print(number);
        gsmSerial.println(";");
        delay(20000);

        gsmSerial.println("ATH");
        delay(2000);

        retry--;
    }
}


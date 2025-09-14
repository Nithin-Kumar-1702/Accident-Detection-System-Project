# Accident-Detection-System-Project

This project is an Accident Detection and Emergency Alert System designed for vehicles, using Arduino hardware and multiple sensors. When a significant tilt or roll is detected—indicating a possible accident—the system automatically sends an SMS with a live Google Maps location and initiates emergency phone calls to a predefined number. A connected LCD provides real-time status updates for users or responders.

Features
Accident Detection: Monitors vehicle roll angle using the MPU6050 accelerometer/gyroscope sensor to identify abnormal motion patterns.

Emergency SMS Alert: Upon accident detection, sends a custom SMS containing a Google Maps link with current GPS coordinates to a configured phone number, enabling quick location tracking.

Automated Phone Calls: Makes continuous call attempts to an emergency contact number to ensure notification is received.

Location Tracking: Uses a GPS module to acquire accurate latitude and longitude information.

User Interface: LCD display shows system status, detection alerts, and SMS notification feedback.

Hardware Used
Arduino Board

MPU6050 Sensor (accelerometer & gyroscope)

GPS Module

GSM Module (SIM800 or similar)

I2C LCD Display

How it Works
Sensor Monitoring: The system constantly checks the roll angle via the MPU6050. Normal motion is distinguished from accident conditions using defined angle thresholds.

Accident Detected: If an accident is suspected, an SMS alert—with a Google Maps link to the vehicle’s live location—is sent to emergency services or family, and phone calls are initiated.

Feedback: An LCD screen provides immediate feedback, showing accident alerts and confirmation of message delivery.

Getting Started
Connect all hardware components as described in the code.

Upload the provided Arduino sketch.

Insert a SIM card into the GSM module and ensure GPS access in your area.

Code Highlights
Easy-to-configure phone numbers and message text, with customization for location reporting.

Includes functions for sending SMS, making calls, and interfacing with sensors and LCD display.

This project demonstrates practical use of IoT in vehicular and personal safety, combining real-time sensor data, GPS tracking, and mobile communication for rapid emergency notification.

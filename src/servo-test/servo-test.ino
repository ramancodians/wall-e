// // Define the onboard LED pin for ESP32
// const int ledPin = 2; // On most ESP32 boards, the onboard LED is on GPIO 2

// void setup() {
//   // Initialize the LED pin as an output
//   pinMode(ledPin, OUTPUT);
// }

// void loop() {
//   digitalWrite(ledPin, HIGH); // Turn the LED on
//   delay(1000);                // Wait for 1 second
//   digitalWrite(ledPin, LOW);  // Turn the LED off
//   delay(1000);                // Wait for 1 second
// }

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// Initialize the PCA9685
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

#define NUM_SERVOS  1
#define SERVOMIN  100 // Minimum pulse length for 0 degree
#define SERVOMAX  600 // Maximum pulse length for 180 degree
#define SERVO_DELAY = 1000

// Servo mapping
#define LEFT_EYE_SERVO 0
#define RIGHT_EYE_SERVO 1
#define HEAD_SERVO 2
#define NECK_SERVO 3


void setup() {
  Serial.begin(115200);
  pwm.begin();
  pwm.setPWMFreq(50); // Analog servos run at 50 Hz
}

void loop() {
 while 
}

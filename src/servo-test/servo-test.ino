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
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// Initialize the PCA9685
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

// Wi-Fi credentials
const char* ssid = "OnePlus 12";
const char* password = "0632725300";
// WebServer instance
ESP8266WebServer server(80);


#define NUM_SERVOS 1
#define SERVOMIN 100  // Minimum pulse length for 0 degree
#define SERVOMAX 600  // Maximum pulse length for 180 degree
#define SERVO_DELAY = 1000

// Display
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Servo mapping
#define LEFT_EYE_SERVO 0
#define RIGHT_EYE_SERVO 1
#define HEAD_SERVO 2
#define NECK_SERVO 3


String renderHTML(String bodyContent) {
  String html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8" />
  <meta name="viewport" content="width=device-width, initial-scale=1.0" />
  <style>
    * {
      margin: 0;
      padding: 0;
      box-sizing: border-box;
      font-family: Arial, sans-serif;
    }
    body {
      display: flex;
      justify-content: center;
      align-items: center;
      height: 100vh;
      background-color: #f4f4f9;
      text-align: center;
    }
    .container {
      max-width: 600px;
      background-color: #ffffff;
      padding: 20px;
      border-radius: 12px;
      box-shadow: 0 8px 16px rgba(0,0,0,0.1);
    }
    h1 {
      color: #007bff;
      font-size: 2rem;
    }
    p {
      margin-top: 10px;
      font-size: 1.2rem;
      color: #555;
    }
    @media (max-width: 600px) {
      h1 {
        font-size: 1.8rem;
      }
      p {
        font-size: 1rem;
      }
    }
  </style>
</head>
<body>
)rawliteral";
  html += bodyContent;
  html += "</body></html>";
  return html;
}

void handleRoot() {
  if (server.hasArg("action")) {
    String action = server.arg("action");
    if (action == "min") {
      pwm.setPWM(HEAD_SERVO, 0, 0);
      Serial.println("Servo Angle: 0° (Min)");
    } else if (action == "max") {
      pwm.setPWM(HEAD_SERVO, 0, 180);
      Serial.println("Servo Angle: 180° (Max)");
    }
  }

  String message = R"rawliteral(
       <h1>Servo Control using Max & Min Buttons</h1>\
      <button onclick='controlServo(\"min\")'>Min (0°)</button>\
      <button onclick='controlServo(\"max\")'>Max (180°)</button>\
      <p>Servo Status: <span id='status'>Waiting...</span></p>\
      <script>\
        function controlServo(action) {\
          alert(\"eeee:\);\
        }\
      </script>\
    )rawliteral";
  server.send(200, "text/html", renderHTML(message));
}


void setup() {
  Wire.begin(12, 13);  // Di
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 initialization failed"));
    for (;;)
      ;
  }

  Serial.begin(115200);
  pwm.begin();
  pwm.setPWMFreq(50);  // Analog servos run at 50 Hz

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 10);
  display.println("WallE Waking up");
  delay(1000);

  //
  display.println("Connecting to WiFi...");
  display.display();
  delay(500);

  // Connect to wifi
  WiFi.begin(ssid, password);
  display.println("Connected to u!");
  display.display();

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  display.println(WiFi.localIP());
  display.display();

  server.on("/", handleRoot);
  server.begin();
  pwm.setPWM(0, 0, 300);  // Channel 0
  delay(1000);
  pwm.setPWM(0, 0, 100);  // Channel 0
}

unsigned long previousMillis = 0;
const long interval = 1000;  // 1 second
bool isMaxPosition = false;

int count = 0;

void loop() {
  server.handleClient();
  // Move Motor 1 from 0 to 180 degrees
  for (int posDegrees = 0; posDegrees <= 180; posDegrees++) {

    // Determine PWM pulse width
    int pwm1 = map(posDegrees, 0, 180, SERVOMIN, SERVOMAX);
    // Write to PCA9685
    pwm.setPWM(0, 0, pwm1);
    // Print to serial monitor
    Serial.print("Motor 1 = ");
    Serial.println(posDegrees);
    delay(30);
  }
}

#include <Arduino.h>
#include "display/display.hpp"
#include "buttons/buttons.hpp"
#include "bitmaps.h"

// p4 motor

// OTA
#include <WiFi.h>
#include <ESPmDNS.h>
#include <NetworkUdp.h>
#include <ArduinoOTA.h>

#define IR_SEND_PIN 7
#define SEND_PWM_BY_TIMER  // Use hardware PWM timer

#include <IRremote.hpp>

// test 4, 5, 6
// A3 is DPAD
// A7 is top btns
// A4 is middle 6
// A5 is bottom 2

#define BTNGRP_DPAD A3
#define BTNGRP_MIDDLE A4
#define BTNGRP_BOTTOM A5
#define BTNGRP_TOP A7

Display display(128, 64, 2, 3, 0x3C);
ButtonGroup top(BTNGRP_TOP, 2, 3.3);
ButtonGroup middle(BTNGRP_MIDDLE, 6, 3.3);
ButtonGroup bottom(BTNGRP_BOTTOM, 2, 3.3);
ButtonGroup dpad(BTNGRP_DPAD, 4, 3.3);

const int vibration_pin = 1;      // Use any available GPIO pin
const int pwm_freq = 5000;    // 5 kHz PWM frequency
const int pwm_resolution = 8;

const char* ssid = "coco-wifi";
const char* password = "ekde2006";
uint32_t last_ota_time = 0;

#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WebSerial.h>

AsyncWebServer server(80);

void vibrate(int ms) {
  ledcWrite(vibration_pin, 30);
  delay(ms);
  ledcWrite(vibration_pin, 0);
}

void setup() {
  IrSender.begin();

  Serial.begin(115200);
  analogReadResolution(12); // sure
  display.init();
  delay(1000);
  display.clear();
  display.draw_bmp(BOOT);
  Serial.println("Booting");

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  ledcAttach(vibration_pin, pwm_freq, pwm_resolution);

  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    delay(5000);
    ESP.restart();
  }

  ArduinoOTA.setHostname("espremote");

  ArduinoOTA
    .onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH) {
        type = "sketch";
      } else {  // U_SPIFFS
        type = "filesystem";
      }

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    })
    .onEnd([]() {
      Serial.println("\nEnd");
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      if (millis() - last_ota_time > 500) {
        Serial.printf("Progress: %u%%\n", (progress / (total / 100)));
        last_ota_time = millis();
      }
    })
    .onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) {
        Serial.println("Auth Failed");
      } else if (error == OTA_BEGIN_ERROR) {
        Serial.println("Begin Failed");
      } else if (error == OTA_CONNECT_ERROR) {
        Serial.println("Connect Failed");
      } else if (error == OTA_RECEIVE_ERROR) {
        Serial.println("Receive Failed");
      } else if (error == OTA_END_ERROR) {
        Serial.println("End Failed");
      }
    });

  ArduinoOTA.begin();

  WebSerial.begin(&server);
  server.begin();

  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  display.clear();
  display.draw_bmp(MENU);
}

// we need a separate OTA mode so we don't block the OTA loop...
// ok maybe its fine? but we should still have one so we can show IP on screen and stuff

void loop() {
  ArduinoOTA.handle();
  top.update();
  dpad.update();
  middle.update();
  bottom.update();
  WebSerial.printf("Top Button: %d (%d)", top.get_press());
  WebSerial.printf("DPAD Button: %d (%d)", dpad.get_press());
  WebSerial.printf("Middle Button: %d (%d)", middle.get_press());
  WebSerial.printf("Bottom Button: %d (%d)", bottom.get_press());

  if (top.get_press() == 0) {
    IrSender.sendSony(0x1, 0x15, 2);
    WebSerial.println("Sent!");
    vibrate(80);
  }

  delay(10);
}

// ESP32-S2 Motor Control at 5% speed (Arduino Core 3.x)
/*const int motorPin = 1;      // Use any available GPIO pin
const int pwmFreq = 5000;    // 5 kHz PWM frequency
const int pwmResolution = 8; // 8-bit resolution (0-255)

void setup() {
  Serial.begin(115200);
  
  // Configure PWM (new API)
  ledcAttach(motorPin, pwmFreq, pwmResolution);
  
  // Set motor to 5% speed
  // 5% of 255 = 12.75, round to 13
  int dutyCycle = 20;  // 5% duty cycle
  ledcWrite(motorPin, dutyCycle);
  
  Serial.println("Motor running at 5% speed");
}

void loop() {
  // Motor continues running at 5%
  // Add your code here if needed
}*/
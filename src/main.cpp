#include "bitmaps.h"
#include "buttons/buttons.hpp"
#include "display/display.hpp"
#include <Arduino.h>

// p4 motor

// OTA
#include <ArduinoOTA.h>
#include <ESPmDNS.h>
#include <NetworkUdp.h>
#include <WiFi.h>

#define IR_SEND_PIN 7
#define SEND_PWM_BY_TIMER // Use hardware PWM timer

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

ScreenManager screen_manager(display);

MenuScreen* menu = new MenuScreen(WIFI);
AddDeviceScreen* add_device = new AddDeviceScreen();

std::vector<MenuItem> menu_items = {
    {&SETTINGS, "Settings"},
    {&PLUS, "Add Device"},
    {&POWER, "Power"}
};

const int vibration_pin = 1; // Use any available GPIO pin
const int pwm_freq = 5000;   // 5 kHz PWM frequency
const int pwm_resolution = 8;

const char *ssid = "coco-wifi";
const char *password = "ekde2006";
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

void vibrate_raw(int pct) { ledcWrite(vibration_pin, (pct * 255) / 100); }

void setup() {
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_8, 1);
  
  Serial.begin(115200);
  
  // Check if waking from deep sleep
  esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();
  if (wakeup_reason == ESP_SLEEP_WAKEUP_EXT0) {
    Serial.println("Waking from deep sleep");
  } else {
    Serial.println("Cold boot");
  }
  
  IrSender.begin();
  analogReadResolution(12);
  ledcAttach(vibration_pin, pwm_freq, pwm_resolution);
  
  // Initialize display and show boot screen
  display.init();
  delay(100);
  display.clear();
  display.draw_bmp(BOOT);
  
  // Setup screens
  menu->set_items(menu_items);
  screen_manager.add_screen(1, menu);
  screen_manager.add_screen(2, add_device);
  
  // Properly reset WiFi after deep sleep
  WiFi.disconnect(true);  // Disconnect and erase credentials from memory
  delay(100);
  WiFi.mode(WIFI_OFF);    // Turn off WiFi
  delay(100);
  WiFi.mode(WIFI_STA);    // Turn back on in Station mode
  delay(100);
  
  WiFi.begin(ssid, password);
  
  display.draw_text("Init...", 16, 55, 1, true);
  
  int wifi_timeout = 0;
  while (WiFi.status() != WL_CONNECTED && wifi_timeout < 3) {
    delay(500);
    Serial.print(".");
    wifi_timeout++;
  }
  Serial.println();
  
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi connection failed!");
    ESP.restart();
  }
  
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  // Setup OTA
  ArduinoOTA.setHostname("espremote");
  ArduinoOTA
      .onStart([]() {
        String type;
        if (ArduinoOTA.getCommand() == U_FLASH) {
          type = "sketch";
        } else {
          type = "filesystem";
        }
      })
      .onEnd([]() { Serial.println("\nEnd"); })
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
  
  // Setup WebSerial
  WebSerial.begin(&server);
  server.begin();
  
  // Show main screen
  screen_manager.set_screen(1);
  Serial.println("Ready");
}
// we need a separate OTA mode so we don't block the OTA loop...
// ok maybe its fine? but we should still have one so we can show IP on screen
// and stuff

void loop() {
  ArduinoOTA.handle();
  top.update();
  dpad.update();
  middle.update();
  bottom.update();
  //WebSerial.printf("Top Button: %d (%d)\n", top.get_press(), analogRead(BTNGRP_TOP));
  //WebSerial.printf("DPAD Button: %d (%d)\n", dpad.get_press(), analogRead(BTNGRP_DPAD));
  // WebSerial.printf("DPAD Button: %d (%d)", dpad.get_press());
  // WebSerial.printf("Middle Button: %d (%d)", middle.get_press());
  // WebSerial.printf("Bottom Button: %d (%d)", bottom.get_press());

  if (top.get_press() == 0) {
    IrSender.sendSony(0x1, 0x15, 2);
    //WebSerial.println("Sent!");
    //display.draw_text("Sent!", 10, 10, 2);
    vibrate(80);
    delay(500);
    display.clear();
  }

  if (dpad.get_press() == 0) {
    screen_manager.handle_left();
    delay(300);
  }

  else if (dpad.get_press() == 2) {
    screen_manager.handle_right();
    delay(300);
  }

  else if (dpad.get_press() == 3) {
    screen_manager.handle_select();
    delay(300);
  }

  delay(10);
}
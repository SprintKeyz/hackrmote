#pragma once

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "bitmap.hpp"

#define OLED_RESET -1

class Display
{
public:
    Display(uint8_t width, uint8_t height, uint8_t sda, uint8_t scl, uint8_t addr);

    bool init();
    void clear(bool update = true);
    void draw_bmp(const s_Bitmap& bmp, int x = INT_MIN, int y = INT_MIN, bool update = true);
    void show();

    // animation
    void blink_bmp(const s_Bitmap& bmp, uint8_t reps, int on_ms, int off_ms = INT_MIN, int x = INT_MIN, int y = INT_MIN);

private:
    Adafruit_SSD1306 m_display;

    uint8_t m_DISP_WIDTH;
    uint8_t m_DISP_HEIGHT;
    uint8_t m_SDA;
    uint8_t m_SCL;
    uint8_t m_ADDR;
};
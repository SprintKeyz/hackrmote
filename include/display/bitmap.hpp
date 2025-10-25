#pragma once

#include <iostream>

struct s_Bitmap {
    const int16_t width;
    const int16_t height;
    const uint16_t color;

    const uint8_t* data;
};

#define MAKE_BITMAP(name, w, h, c, ...)                  \
    const uint8_t name##_data[] PROGMEM = { __VA_ARGS__ }; \
    const s_Bitmap name = { w, h, c, name##_data }

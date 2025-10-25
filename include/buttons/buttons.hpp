#pragma once

#include <Arduino.h>

class ButtonGroup {
public:
    ButtonGroup(uint8_t analog_pin, uint8_t num_buttons, float vcc);

    bool is_pressed(uint8_t index);
    int8_t get_press();
    void update();

private:
    uint8_t m_pin;
    uint8_t m_num_buttons;
    float m_vcc;

    int8_t m_last_pressed = -1;
};
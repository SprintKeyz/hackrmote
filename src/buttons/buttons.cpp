#include "buttons/buttons.hpp"

ButtonGroup::ButtonGroup(uint8_t analog_pin, uint8_t num_buttons, float vcc) {
    m_pin = analog_pin;
    m_num_buttons = num_buttons;
    m_vcc = vcc;
}

void ButtonGroup::update() {
    int val = analogRead(m_pin);
    long num = (long)val * m_num_buttons + (4095/2);  // <-- change: remove +1
    int idx = num / 4095;
    int btn = m_num_buttons - idx;                    // <-- change: remove +1

    if (btn < 0 || btn >= m_num_buttons) m_last_pressed = -1;
    else m_last_pressed = btn;
}


bool ButtonGroup::is_pressed(uint8_t index) {
    return index == m_last_pressed;
}

int8_t ButtonGroup::get_press() {
    return m_last_pressed;
}
#include "display/display.hpp"

void Display::clear(bool update) {
    m_display.clearDisplay();
    if (update) Display::show();
}
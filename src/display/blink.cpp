#include "display/display.hpp"

void Display::blink_bmp(const s_Bitmap &bmp, uint8_t reps, int on_ms, int off_ms, int x, int y)
{
    off_ms = (off_ms == INT_MIN) ? on_ms : off_ms;

    for (int i = 0; i < 3; i++)
    {
        Serial.println(i);
        Display::draw_bmp(bmp, true, x, y);
        delay(on_ms);
        Display::clear();
        delay(off_ms);
    }
}
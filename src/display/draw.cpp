#include "display/display.hpp"

void Display::draw_bmp(const s_Bitmap& bmp, int x, int y, bool update) {
    x = (x == INT_MIN) ? ((m_DISP_WIDTH - bmp.width) / 2) : x;
    y = (y == INT_MIN) ? ((m_DISP_HEIGHT - bmp.height) / 2) : y;
    m_display.drawBitmap(x, y, bmp.data, bmp.width, bmp.height, bmp.color);
    if (update) Display::show();
}
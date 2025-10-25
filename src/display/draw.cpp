#include "display/display.hpp"

void Display::draw_bmp(const s_Bitmap &bmp, int x, int y, float scale,
                       bool update) {
  if (scale == 1.0) {
    x = (x == INT_MIN) ? ((m_DISP_WIDTH - bmp.width) / 2) : x;
    y = (y == INT_MIN) ? ((m_DISP_HEIGHT - bmp.height) / 2) : y;
    m_display.drawBitmap(x, y, bmp.data, bmp.width, bmp.height, bmp.color);
  }
  else {
    // Calculate scaled dimensions
    int16_t scaled_width = (int16_t)(bmp.width * scale);
    int16_t scaled_height = (int16_t)(bmp.height * scale);
    
    // Center if x or y is INT_MIN
    x = (x == INT_MIN) ? ((m_DISP_WIDTH - scaled_width) / 2) : x;
    y = (y == INT_MIN) ? ((m_DISP_HEIGHT - scaled_height) / 2) : y;
    
    // Draw scaled bitmap using nearest-neighbor sampling
    for (int16_t j = 0; j < scaled_height; j++) {
      for (int16_t i = 0; i < scaled_width; i++) {
        // Map scaled coordinates back to original bitmap coordinates
        int16_t src_x = (int16_t)(i / scale);
        int16_t src_y = (int16_t)(j / scale);
        
        // Clamp to bitmap bounds
        if (src_x >= bmp.width) src_x = bmp.width - 1;
        if (src_y >= bmp.height) src_y = bmp.height - 1;
        
        // Check if source pixel is set
        if (bmp.data[src_y * ((bmp.width + 7) / 8) + src_x / 8] & (128 >> (src_x & 7))) {
          m_display.drawPixel(x + i, y + j, bmp.color);
        }
      }
    }
  }
  if (update)
    Display::show();
}

void Display::draw_text(std::string text, int x, int y, int size, bool update) {
  m_display.setTextSize(size);
  m_display.setTextColor(SSD1306_WHITE);
  
  // Calculate text dimensions for centering
  int16_t x1, y1;
  uint16_t text_width, text_height;
  m_display.getTextBounds(text.c_str(), 0, 0, &x1, &y1, &text_width, &text_height);
  
  // Center if x or y is INT_MIN
  x = (x == INT_MIN) ? ((m_DISP_WIDTH - text_width) / 2) : x;
  y = (y == INT_MIN) ? ((m_DISP_HEIGHT - text_height) / 2) : y;
  
  m_display.setCursor(x, y);
  m_display.println(F(text.c_str()));
  
  if (update)
    Display::show();
}

void Display::draw_rect(int x, int y, int width, int height, bool update) {
  m_display.drawRect(x, y, width, height, SSD1306_WHITE);
  if (update)
    Display::show();
}

void Display::draw_rect_rounded(int x, int y, int width, int height, int radius,
                                bool update) {
  m_display.drawRoundRect(x, y, width, height, radius, SSD1306_WHITE);
  if (update)
    Display::show();
}

void Display::draw_icon(const s_Bitmap &bmp, int x, int y, float scale,
                        int padding, int radius, bool update) {
  // Calculate scaled dimensions
  int16_t scaled_width = (int16_t)(bmp.width * scale);
  int16_t scaled_height = (int16_t)(bmp.height * scale);
  
  // Calculate box dimensions with padding
  int16_t box_width = scaled_width + (padding * 2);
  int16_t box_height = scaled_height + (padding * 2);
  
  // Center if x or y is INT_MIN
  int16_t box_x = (x == INT_MIN) ? ((m_DISP_WIDTH - box_width) / 2) : x;
  int16_t box_y = (y == INT_MIN) ? ((m_DISP_HEIGHT - box_height) / 2) : y;
  
  // Calculate bitmap position (centered in the box)
  int16_t bmp_x = box_x + padding;
  int16_t bmp_y = box_y + padding;
  
  // Draw the rounded rectangle outline
  m_display.drawRoundRect(box_x, box_y, box_width, box_height, radius, SSD1306_WHITE);
  
  // Draw the bitmap centered inside
  draw_bmp(bmp, bmp_x, bmp_y, scale, false);
  
  if (update)
    Display::show();
}
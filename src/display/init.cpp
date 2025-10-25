#include "display/display.hpp"

Display::Display(uint8_t width, uint8_t height, uint8_t sda, uint8_t scl, uint8_t addr) : m_display(width, height, &Wire, OLED_RESET)
{
    m_DISP_WIDTH = width;
    m_DISP_HEIGHT = height;
    m_SDA = sda;
    m_SCL = scl;
    m_ADDR = addr;
}

bool Display::init()
{
    Wire.begin(m_SDA, m_SCL);
    return m_display.begin(SSD1306_SWITCHCAPVCC, m_ADDR);
}
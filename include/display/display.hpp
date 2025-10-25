#pragma once

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <map>

#include "bitmap.hpp"

#define OLED_RESET -1


class Display
{
    public:
    Display(uint8_t width, uint8_t height, uint8_t sda, uint8_t scl, uint8_t addr);
    
    bool init();
    void clear(bool update = true);
    void draw_bmp(const s_Bitmap& bmp, int x = INT_MIN, int y = INT_MIN, float scale = 1.0, bool update = true);
    void draw_icon(const s_Bitmap& bmp, int x = INT_MIN, int y = INT_MIN, float scale = 1.0, int padding = 4, int radius = 4, bool update = true);
    void draw_text(std::string text, int x, int y, int size, bool update = true);
    void draw_rect(int x, int y, int width, int height, bool update = true);
    void draw_rect_rounded(int x, int y, int width, int height, int radius, bool update = true);
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

struct MenuItem {
    const s_Bitmap* icon;  // Changed to pointer
    std::string title;
};

// Forward declaration
class ScreenManager;

// Base class for all screens
class Screen {
public:
    virtual ~Screen() = default;
    virtual void draw(Display& display) = 0;
    virtual void on_left() {}
    virtual void on_right() {}
    virtual void on_select(ScreenManager& manager) {}
};

// Menu screen
class MenuScreen : public Screen {
public:
    MenuScreen(const s_Bitmap& wifi_icon);
    void set_items(const std::vector<MenuItem>& items);
    void draw(Display& display) override;
    void on_left() override;
    void on_right() override;
    void on_select(ScreenManager& manager) override;
    int get_selected_index() const { return m_selected_index; }

private:
    const s_Bitmap& m_wifi_icon;
    std::vector<MenuItem> m_items;
    int m_selected_index;
    
    int get_left_index() const;
    int get_right_index() const;
};

// Example: Add Device screen
class AddDeviceScreen : public Screen {
public:
    AddDeviceScreen();
    void draw(Display& display) override;
    void on_select(ScreenManager& manager) override;
};

// Screen Manager
class ScreenManager {
public:
    ScreenManager(Display& display);
    void set_screen(int screen_id);
    void add_screen(int screen_id, Screen* screen);
    void draw();
    void clear();
    void handle_left();
    void handle_right();
    void handle_select();
    int get_current_screen_id() const { return m_current_screen_id; }

private:
    Display& m_display;
    std::map<int, Screen*> m_screens;
    int m_current_screen_id;
};
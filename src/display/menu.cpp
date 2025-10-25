
#include "display/display.hpp"

// MenuScreen implementation
MenuScreen::MenuScreen(const s_Bitmap& wifi_icon) 
    : m_wifi_icon(wifi_icon), m_selected_index(0) {}

void MenuScreen::set_items(const std::vector<MenuItem>& items) {
    m_items = items;
    m_selected_index = 0;
}

int MenuScreen::get_left_index() const {
    return (m_selected_index - 1 + m_items.size()) % m_items.size();
}

int MenuScreen::get_right_index() const {
    return (m_selected_index + 1) % m_items.size();
}

void MenuScreen::draw(Display& display) {
    if (m_items.empty()) return;
    
    display.clear(false);
    
    // Left icon (smaller)
    display.draw_icon(*m_items[get_left_index()].icon, 3, 18, 0.47, 4, 4, false);
    
    // Center icon (larger, selected)
    display.draw_icon(*m_items[m_selected_index].icon, 42, 6, 0.65, 6, 4, false);
    
    // Right icon (smaller)
    display.draw_icon(*m_items[get_right_index()].icon, 95, 18, 0.47, 4, 4, false);
    
    // WiFi indicator
    display.draw_bmp(m_wifi_icon, 112, 0, 0.75, false);
    
    // Title text
    display.draw_text(m_items[m_selected_index].title, INT_MIN, 54, 1, false);
    
    display.show();
}

void MenuScreen::on_left() {
    if (m_items.empty()) return;
    m_selected_index = (m_selected_index - 1 + m_items.size()) % m_items.size();
}

void MenuScreen::on_right() {
    if (m_items.empty()) return;
    m_selected_index = (m_selected_index + 1) % m_items.size();
}

void MenuScreen::on_select(ScreenManager& manager) {
    // Map selected index to screen IDs
    // For example: index 1 (PLUS) goes to screen 2
    if (m_selected_index == 1) {  // PLUS icon
        manager.set_screen(2);  // Go to Add Device screen
    }

    else if (m_selected_index == 2) {  // POWER icon
        // Put device into deep sleep
        manager.clear();
        esp_deep_sleep_start();
    }
}

// AddDeviceScreen implementation
AddDeviceScreen::AddDeviceScreen() {}

void AddDeviceScreen::draw(Display& display) {
    display.clear(false);
    display.draw_text("Add New Device", INT_MIN, 10, 1, false);
    display.draw_text("Scanning...", INT_MIN, 30, 1, false);
    display.show();
}

void AddDeviceScreen::on_select(ScreenManager& manager) {
    // Go back to main menu
    manager.set_screen(1);
}

// ScreenManager implementation
ScreenManager::ScreenManager(Display& display) 
    : m_display(display), m_current_screen_id(1) {}

void ScreenManager::add_screen(int screen_id, Screen* screen) {
    m_screens[screen_id] = screen;
}

void ScreenManager::set_screen(int screen_id) {
    if (m_screens.find(screen_id) != m_screens.end()) {
        m_current_screen_id = screen_id;
        draw();
    }
}

void ScreenManager::draw() {
    if (m_screens.find(m_current_screen_id) != m_screens.end()) {
        m_screens[m_current_screen_id]->draw(m_display);
    }
}

void ScreenManager::handle_left() {
    if (m_screens.find(m_current_screen_id) != m_screens.end()) {
        m_screens[m_current_screen_id]->on_left();
        draw();
    }
}

void ScreenManager::handle_right() {
    if (m_screens.find(m_current_screen_id) != m_screens.end()) {
        m_screens[m_current_screen_id]->on_right();
        draw();
    }
}

void ScreenManager::handle_select() {
    if (m_screens.find(m_current_screen_id) != m_screens.end()) {
        m_screens[m_current_screen_id]->on_select(*this);
    }
}

void ScreenManager::clear() {
    m_display.clear();
}
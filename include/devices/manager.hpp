#pragma once

#include "types/tv.hpp"
#include <Preferences.h>
#include <memory>
#include <string>
#include <vector>

class DeviceManager {
public:
	bool save();
	bool load();
	void clear();

	Device* get_device_by_idx(uint8_t idx);
	bool remove_by_idx(uint8_t idx);
	int8_t add_device(std::string name, Device::Type type);
	uint8_t get_device_count();

private:
	std::vector<std::unique_ptr<Device>> m_devices;
	Preferences m_prefs;
};
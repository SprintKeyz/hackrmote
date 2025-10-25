#include "devices/manager.hpp"

bool DeviceManager::save() {
	DynamicJsonDocument doc(1024);
	JsonArray array = doc.to<JsonArray>();

	for (auto &device : m_devices) {
		JsonObject obj = array.createNestedObject();
		device->serialize(obj);
	}

	std::string json;
	serializeJson(doc, json);
	m_prefs.begin("devices");
	Serial.println("JSON: " + String(json.c_str()));
	bool success = m_prefs.putString("data", json.c_str());
	m_prefs.end();
	return success;
}

bool DeviceManager::load() {
	m_prefs.begin("devices", true);
	std::string json = m_prefs.getString("data", "[]").c_str();
	m_prefs.end();

	if (json == "[]")
		return true; // skip compute

	DynamicJsonDocument doc(1024);
	DeserializationError error = deserializeJson(doc, json);

	if (error)
		return false;

	m_devices.clear();
	for (JsonObject obj : doc.as<JsonArray>()) {
		auto device = Device::create_from_json(obj);
		if (device) {
			Serial.println("Created device from JSON");
			m_devices.push_back(std::move(device));
		} else {
			Serial.println("Failed to create device");
		}
	}

	return true;
}

void DeviceManager::clear() {
    m_devices.clear();
}

int8_t DeviceManager::add_device(std::string name, Device::Type type) {
	switch (type) {
	case Device::Type::TV:
		auto tv = std::make_unique<TVDevice>(name);
		m_devices.push_back(std::move(tv));
		return m_devices.size() - 1;
	};

	return -1;
}

Device *DeviceManager::get_device_by_idx(uint8_t idx) { return (idx < m_devices.size()) ? m_devices[idx].get() : nullptr; }

bool DeviceManager::remove_by_idx(uint8_t idx) {
	if (idx >= m_devices.size()) {
		Serial.println("Invalid index");
		return false;
	}

	std::string device_name = m_devices[idx]->name;
	m_devices.erase(m_devices.begin() + idx);
	Serial.println("Removed device " + String(device_name.c_str()));
	return true;
}

uint8_t DeviceManager::get_device_count() {
    return m_devices.size();
}

std::unique_ptr<Device> Device::create_from_json(JsonObject &json) {
	Type type = json["type"].as<Type>();
	switch (type) {
	case Type::TV:
		auto tv = std::make_unique<TVDevice>(json["name"].as<std::string>());
		tv->deserialize(json);
		return (tv);
	}

	return nullptr;
}
#pragma once

#include <ArduinoJson.h>
#include <map>
#include <memory>
#include <string>

#include "ir_struct.hpp"

// abstract device class
class Device {
public:
	enum Type { TV, LIGHT, FAN };
	enum Comms { IR, WIFI, NONE };

	Type type;
	std::string name;

	Device(Type t, std::string n) : type(t), name(n) {}
	virtual ~Device() = default;

	virtual void serialize(JsonObject &json) = 0;
	virtual void deserialize(JsonObject &json) = 0;

    // scan for updates from HA
    virtual void step_update_loop() = 0;

	// a static method to create devices from JSON, used when restoring from flash
	static std::unique_ptr<Device> create_from_json(JsonObject &json);

protected:
	void serialize_base(JsonObject &json);
	void deserialize_base(JsonObject &json);

	void set_entity_name(std::string name);
	void add_ir_code(std::string name, s_IRData data);
	std::string get_entity_name();
	s_IRData get_ir_code(std::string name);

private:
	// preferred comms if multiple available
	Comms m_preferred_comms = Comms::NONE;
	// device entity name (if applicable)
	std::string m_entity_name = "";
	// IR map (if applicable)
	std::map<std::string, s_IRData> m_ir_codes;
};

// how this works:
// when creating a device, we get to pick between IR and HA
// this is not determined in the ctor as codes must be added dynamically and not everything needs HA
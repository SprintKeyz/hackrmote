#include "devices/device.hpp"

void Device::set_entity_name(std::string name) {
    m_entity_name = name;
}

std::string Device::get_entity_name() {
    return m_entity_name;
}

void Device::add_ir_code(std::string name, s_IRData data) {
    m_ir_codes.insert({name, data});
}

s_IRData Device::get_ir_code(std::string name) {
    return m_ir_codes.at(name);
}

// serialization
void Device::serialize_base(JsonObject& json) {
    // general data
    json["type"] = static_cast<int>(type);
    json["name"] = name;
    json["preferred_comms"] = static_cast<int>(m_preferred_comms);
    json["entity_name"] = m_entity_name;
    
    // ir data
    JsonObject ir_codes = json.createNestedObject("ir_codes");
    for (const auto& pair : m_ir_codes) {
        JsonObject ir_data = ir_codes.createNestedObject(pair.first);
        ir_data["protocol"] = static_cast<int>(pair.second.protocol);
        ir_data["address"] = pair.second.addr;
        ir_data["command"] = pair.second.command;
    }
}

void Device::deserialize_base(JsonObject& json) {
    type = json["type"].as<Type>();
    name = json["name"].as<std::string>();
    m_preferred_comms = json["preferred_comms"].as<Comms>();
    m_entity_name = json["entity_name"].as<std::string>();

    JsonObject ir_codes = json["ir_codes"];
    m_ir_codes.clear();
    for (JsonPair kv_pair : ir_codes) {
        s_IRData ir_data;
        JsonObject ir_obj = kv_pair.value();
        ir_data.protocol = ir_obj["protocol"].as<IR_DECODE_TYPE>();
        ir_data.addr = ir_obj["address"].as<uint8_t>();
        ir_data.command = ir_obj["command"].as<uint8_t>();

        m_ir_codes.insert({kv_pair.key().c_str(), ir_data});
    }
}
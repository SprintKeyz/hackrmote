#include "devices/types/tv.hpp"

void TVDevice::serialize(JsonObject& json) {
    serialize_base(json); // serialize base device
    json["volume"] = m_volume;
    json["source"] = m_source;
}

void TVDevice::deserialize(JsonObject& json) {
    deserialize_base(json);
    m_volume = json["volume"];
    m_source = json["source"].as<std::string>();
}

void TVDevice::step_update_loop() {
    // if ha entity is not defined, do nothing!
    if(get_entity_name() == "") {
        return;
    }
}

void TVDevice::change_volume(int step) {
    if (m_volume != -1) m_volume += step;
    // TODO: Send volume command
}
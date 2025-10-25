#pragma once

#include "../device.hpp"

class TVDevice : public Device {
public:
    TVDevice(std::string name) : Device(Type::TV, name) {}

    void serialize(JsonObject& json) override;
    void deserialize(JsonObject& json) override;

    void step_update_loop() override;

    void change_volume(int step);

private:
    int m_volume = -1; // read from tv if available over HA
    std::string m_source = ""; // read from tv if available over HA
};
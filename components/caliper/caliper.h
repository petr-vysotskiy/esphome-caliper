#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"

namespace esphome {
namespace caliper_sensor {

class CaliperSensor : public sensor::Sensor, public PollingComponent  {
    void setup() override;
    void update() override;
    void dump_config() override;
    float decode();
};

}
}
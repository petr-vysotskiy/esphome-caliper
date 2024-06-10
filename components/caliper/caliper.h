#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"

namespace esphome {
namespace caliper_sensor {

class CaliperSensor : public sensor::Sensor, public PollingComponent  {
    public:
        void setup() override;
        void loop() override;
        void update() override;
        float decode(u32_t p);
};

}
}
import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import CONF_ID
from esphome.const import (
    STATE_CLASS_MEASUREMENT,
)

empty_sensor_ns = cg.esphome_ns.namespace('caliper_sensor')

CaliperSensor = empty_sensor_ns.class_('CaliperSensor', sensor.Sensor, cg.PollingComponent)


CONFIG_SCHEMA = sensor.sensor_schema(
        unit_of_measurement="mm",
        state_class=STATE_CLASS_MEASUREMENT,
        accuracy_decimals=2,
    ).extend({
        cv.GenerateID(): cv.declare_id(CaliperSensor),
    }).extend(cv.polling_component_schema('10s'))


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await sensor.register_sensor(var, config)
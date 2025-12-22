import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import esp32_ble_tracker, esp32_ble_client
from esphome.const import CONF_ID

shelly_locate_ns = cg.esphome_ns.namespace("shelly_locate")
ShellyLocate = shelly_locate_ns.class_("ShellyLocate", cg.Component, esp32_ble_client.BLEClientBase)

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(ShellyLocate),
})

def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)
    yield esp32_ble_client.register_ble_client(var)

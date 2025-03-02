import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor, button, sensor
from esphome.const import (CONF_ID, DEVICE_CLASS_DURATION,
                           DEVICE_CLASS_RUNNING, ICON_TIMER,
                           STATE_CLASS_MEASUREMENT, UNIT_MILLISECOND)

CODEOWNERS = ["@renggli"]
AUTO_LOAD = ['sensor', 'binary_sensor', 'button']

CONF_START_BUTTON = 'start'
CONF_STOP_BUTTON = 'stop'
CONF_RESET_BUTTON = 'reset'
CONF_RUNNING_BINARY_SENSOR = 'running'
CONF_ELAPSED_SENSOR = 'elapsed'

clock_ns = cg.esphome_ns.namespace('clock')
Stopwatch = clock_ns.class_('Stopwatch', cg.PollingComponent)

StartButton = clock_ns.class_('StartButton', button.Button)
StopButton = clock_ns.class_('StopButton', button.Button)
ResetButton = clock_ns.class_('ResetButton', button.Button)

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(Stopwatch),
    cv.Optional(CONF_START_BUTTON): button.button_schema(
        StartButton,
        icon='mdi:timer-play-outline',
    ),
    cv.Optional(CONF_STOP_BUTTON): button.button_schema(
        StopButton,
        icon='mdi:timer-stop-outline',
    ),
    cv.Optional(CONF_RESET_BUTTON): button.button_schema(
        ResetButton,
        icon='mdi:restart',
    ),
    cv.Optional(CONF_RUNNING_BINARY_SENSOR): binary_sensor.binary_sensor_schema(
        device_class=DEVICE_CLASS_RUNNING,
        icon=ICON_TIMER,
    ),
    cv.Optional(CONF_ELAPSED_SENSOR): sensor.sensor_schema(
        unit_of_measurement=UNIT_MILLISECOND,
        accuracy_decimals=0,
        icon=ICON_TIMER,
        device_class=DEVICE_CLASS_DURATION,
        state_class=STATE_CLASS_MEASUREMENT,
    ),
}).extend(cv.polling_component_schema('1s'))

async def to_code(config):
    component = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(component, config)

    if conf := config.get(CONF_START_BUTTON):
        entity = await button.new_button(conf)
        await cg.register_parented(entity, config[CONF_ID])
        cg.add(component.set_start_button(entity))

    if conf := config.get(CONF_STOP_BUTTON):
        entity = await button.new_button(conf)
        await cg.register_parented(entity, config[CONF_ID])
        cg.add(component.set_stop_button(entity))

    if conf := config.get(CONF_RESET_BUTTON):
        entity = await button.new_button(conf)
        await cg.register_parented(entity, config[CONF_ID])
        cg.add(component.set_reset_button(entity))

    if conf := config.get(CONF_RUNNING_BINARY_SENSOR):
        entity = await binary_sensor.new_binary_sensor(conf)
        cg.add(component.set_running_binary_sensor(entity))

    if conf := config.get(CONF_ELAPSED_SENSOR):
        entity = await sensor.new_sensor(conf)
        cg.add(component.set_elapsed_sensor(entity))

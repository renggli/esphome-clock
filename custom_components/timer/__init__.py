import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import button, number, sensor, text_sensor
from esphome.const import (CONF_ID, DEVICE_CLASS_DURATION, ICON_TIMER,
                           STATE_CLASS_MEASUREMENT, UNIT_MILLISECOND)

CODEOWNERS = ["@renggli"]
AUTO_LOAD = ['button', 'number', 'sensor', 'text_sensor']

CONF_DURATION_NUMBER = 'duration'
CONF_START_BUTTON = 'start'
CONF_PAUSE_BUTTON = 'pause'
CONF_STOP_BUTTON = 'stop'
CONF_STATE_TEXT_SENSOR = 'state'
CONF_REMAINING_SENSOR = 'remaining'

clock_ns = cg.esphome_ns.namespace('clock')
TimerComponent = clock_ns.class_('TimerComponent', cg.PollingComponent)

DurationTimerNumber = clock_ns.class_('DurationTimerNumber', number.Number)
StartTimerButton = clock_ns.class_('StartTimerButton', button.Button)
PauseTimerButton = clock_ns.class_('PauseTimerButton', button.Button)
StopTimerButton = clock_ns.class_('StopTimerButton', button.Button)

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(TimerComponent),
    cv.Optional(CONF_DURATION_NUMBER): number.number_schema(
        DurationTimerNumber,
        unit_of_measurement=UNIT_MILLISECOND,
        icon=ICON_TIMER,
        device_class=DEVICE_CLASS_DURATION,
    ),
    cv.Optional(CONF_START_BUTTON): button.button_schema(
        StartTimerButton,
        icon='mdi:timer-play-outline',
    ),
    cv.Optional(CONF_PAUSE_BUTTON): button.button_schema(
        PauseTimerButton,
        icon='mdi:timer-pause-outline',
    ),
    cv.Optional(CONF_STOP_BUTTON): button.button_schema(
        StopTimerButton,
        icon='mdi:timer-stop-outline',
    ),
    cv.Optional(CONF_STATE_TEXT_SENSOR): text_sensor.text_sensor_schema(
        icon=ICON_TIMER,
    ),
    cv.Optional(CONF_REMAINING_SENSOR): sensor.sensor_schema(
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

    if conf := config.get(CONF_DURATION_NUMBER):
        entity = await number.new_number(conf, min_value=0, max_value=0xffffffff, step=1)
        await cg.register_parented(entity, config[CONF_ID])
        cg.add(component.set_duration_number(entity))

    if conf := config.get(CONF_START_BUTTON):
        entity = await button.new_button(conf)
        await cg.register_parented(entity, config[CONF_ID])
        cg.add(component.set_start_button(entity))

    if conf := config.get(CONF_PAUSE_BUTTON):
        entity = await button.new_button(conf)
        await cg.register_parented(entity, config[CONF_ID])
        cg.add(component.set_pause_button(entity))

    if conf := config.get(CONF_STOP_BUTTON):
        entity = await button.new_button(conf)
        await cg.register_parented(entity, config[CONF_ID])
        cg.add(component.set_stop_button(entity))

    if conf := config.get(CONF_STATE_TEXT_SENSOR):
        entity = await text_sensor.new_text_sensor(conf)
        cg.add(component.set_state_text_sensor(entity))

    if conf := config.get(CONF_REMAINING_SENSOR):
        entity = await sensor.new_sensor(conf)
        cg.add(component.set_remaining_sensor(entity))

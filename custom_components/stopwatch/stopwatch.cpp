
#include "stopwatch.h"

#include "esphome/core/hal.h"
#include "esphome/core/log.h"

#define PUBLISH_IF_CHANGED(sensor, value)                   \
  if (sensor != nullptr) {                                  \
    auto updated = value;                                   \
    if (!sensor->has_state() || sensor->state != updated) { \
      sensor->publish_state(updated);                       \
    }                                                       \
  }

namespace esphome::clock {

namespace {
static const char *const TAG = "clock.stopwatch";
}

void StopwatchComponent::dump_config() {
  ESP_LOGCONFIG(TAG, "Stopwatch:", this);
  LOG_BUTTON("  ", "StartButton", start_button_);
  LOG_BUTTON("  ", "StopButton", stop_button_);
  LOG_BUTTON("  ", "ResetButton", reset_button_);
  LOG_BINARY_SENSOR("  ", "RunningBinarySensor", running_binary_sensor_);
  LOG_SENSOR("  ", "ElapsedSensor", elapsed_sensor_);
}

void StopwatchComponent::update() {
  PUBLISH_IF_CHANGED(running_binary_sensor_, is_running());
  PUBLISH_IF_CHANGED(elapsed_sensor_, get_elapsed());
  if (!is_running_) stop_poller();
}

void StopwatchComponent::start() {
  if (is_running_) return;
  start_ms_ = millis();
  is_running_ = true;
  start_poller();
  update();
}

void StopwatchComponent::stop() {
  if (!is_running_) return;
  elapsed_ms_ += millis() - start_ms_;
  is_running_ = false;
  update();
}

void StopwatchComponent::reset() {
  start_ms_ = 0;
  elapsed_ms_ = 0;
  is_running_ = false;
  update();
}

uint32_t StopwatchComponent::get_elapsed() {
  return is_running_ ? elapsed_ms_ + (millis() - start_ms_) : elapsed_ms_;
}

}  // namespace esphome::clock

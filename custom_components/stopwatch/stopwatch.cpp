
#include "stopwatch.h"

#include "esphome/core/hal.h"
#include "esphome/core/log.h"

namespace esphome::clock {
namespace {
static const char *const TAG = "clock.stopwatch";
}

void Stopwatch::dump_config() {
  ESP_LOGCONFIG(TAG, "Stopwatch:", this);
  LOG_BUTTON("  ", "StartButton", start_button_);
  LOG_BUTTON("  ", "StopButton", stop_button_);
  LOG_BUTTON("  ", "ResetButton", reset_button_);
  LOG_BINARY_SENSOR("  ", "RunningBinarySensor", running_binary_sensor_);
  LOG_SENSOR("  ", "ElapsedSensor", elapsed_sensor_);
}

void Stopwatch::update() {
  if (running_binary_sensor_ != nullptr) {
    running_binary_sensor_->publish_state(is_running());
  }
  if (elapsed_sensor_ != nullptr) {
    elapsed_sensor_->publish_state(get_elapsed());
  }
  if (!is_running_) stop_poller();
}

void Stopwatch::start() {
  if (is_running_) return;
  start_ms_ = millis();
  is_running_ = true;
  start_poller();
  update();
}

void Stopwatch::stop() {
  if (!is_running_) return;
  elapsed_ms_ += millis() - start_ms_;
  is_running_ = false;
  update();
}

void Stopwatch::reset() {
  start_ms_ = 0;
  elapsed_ms_ = 0;
  is_running_ = false;
  update();
}

uint32_t Stopwatch::get_elapsed() {
  return is_running_ ? elapsed_ms_ + (millis() - start_ms_) : elapsed_ms_;
}

}  // namespace esphome::clock

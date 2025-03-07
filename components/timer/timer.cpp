#include "timer.h"

#include <cmath>

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
static const char* const TAG = "clock.timer";
}

void TimerComponent::dump_config() {
  ESP_LOGCONFIG(TAG, "Timer:", this);
  LOG_NUMBER("  ", "DurationNumber", duration_number_);
  LOG_BUTTON("  ", "StartButton", start_button_);
  LOG_BUTTON("  ", "PauseButton", pause_button_);
  LOG_BUTTON("  ", "StopButton", stop_button_);
  LOG_TEXT_SENSOR("  ", "StateTextSensor", state_text_sensor_);
  LOG_SENSOR("  ", "RemainingSensor", remaining_sensor_);
}

void TimerComponent::update() {
  if (state_ == TimerState::RUNNING || state_ == TimerState::EXPIRED) {
    state_ = get_remaining() > 0 ? TimerState::RUNNING : TimerState::EXPIRED;
  }
  PUBLISH_IF_CHANGED(duration_number_, get_duration());
  PUBLISH_IF_CHANGED(state_text_sensor_, get_state());
  PUBLISH_IF_CHANGED(remaining_sensor_, get_remaining());
  if (state_ == TimerState::STOPPED || state_ == TimerState::PAUSED) {
    stop_poller();
  }
}

void TimerComponent::set_duration(uint32_t duration_ms) {
  duration_ms_ = duration_ms;
  update();
}

void TimerComponent::start() {
  state_ = TimerState::RUNNING;
  start_ms_ = millis();
  elapsed_ms_ = 0;
  start_poller();
  update();
}

void TimerComponent::pause() {
  if (state_ == TimerState::RUNNING || state_ == TimerState::EXPIRED) {
    state_ = TimerState::PAUSED;
    elapsed_ms_ += millis() - start_ms_;
  } else if (state_ == TimerState::PAUSED) {
    state_ = TimerState::RUNNING;
    start_ms_ = millis();
    start_poller();
  }
  update();
}

void TimerComponent::stop() {
  state_ = TimerState::STOPPED;
  start_ms_ = 0;
  elapsed_ms_ = 0;
  update();
}

const char* TimerComponent::get_state() {
  switch (state_) {
    case TimerState::STOPPED:
      return "Stopped";
    case TimerState::RUNNING:
      return "Running";
    case TimerState::PAUSED:
      return "Paused";
    case TimerState::EXPIRED:
      return "Expired";
    default:
      return "Unknown";
  }
}

int TimerComponent::get_remaining() {
  if (state_ == TimerState::RUNNING || state_ == TimerState::EXPIRED) {
    return duration_ms_ - elapsed_ms_ - (millis() - start_ms_);
  } else if (state_ == TimerState::PAUSED) {
    return duration_ms_ - elapsed_ms_;
  } else {
    return duration_ms_;
  }
}

}  // namespace  esphome::clock

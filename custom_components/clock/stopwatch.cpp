
#include "stopwatch.h"

#include "esphome.h"

namespace esphome::clock {

void Stopwatch::setup() {
  register_service(&Stopwatch::start, "start");
  register_service(&Stopwatch::stop, "stop");
  register_service(&Stopwatch::reset, "reset");

  if (running_sensor_ != nullptr) {
    running_sensor_.publish_initial_state(false);
    running_sensor_.add_on_state_callback(
        [this](bool state) { state ? start() : stop(); });
  }
}

void Stopwatch::start() {
  if (is_running_) return;
  start_ms_ = millis();
  is_running_ = true;
  if (running_sensor_ != nullptr) {
    running_sensor_.publish_state(true);
  }
  if (elapsed_sensor_ != nullptr) {
    set_interval("stopwatch", 1000,
                 [this]() { elapsed_sensor_->publish_state(get_elapse()); });
  }
}

void Stopwatch::stop() {
  if (!is_running_) return;
  stop_ms_ = millis();
  elapsed_ms_ += stop_ms_ - start_ms_;
  is_running_ = false;
  if (running_sensor_ != nullptr) {
    running_sensor_.publish_state(false);
  }
  if (elapsed_sensor_ != nullptr) {
    clear_interval("stopwatch", 1000);
  }
}

void Stopwatch::reset() {
  stop();
  start_ms_ = 0;
  stop_ms_ = 0;
  elapsed_ms_ = 0;
}

uint32_t Stopwatch::get_elapsed() {
  return is_running_ ? elapsed_ms_ + stop_ms_ - start_ms_ : elapsed_ms_;
}

}  // namespace esphome::clock

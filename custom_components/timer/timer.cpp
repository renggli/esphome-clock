#include "timer.h"

namespace  esphome::clock {

void Timer::setup() {
  register_service(&Timer::start, "start");
  register_service(&Timer::start, "stop");
  register_service(&Timer::set_duration, "set_duration", {"duration_ms"});
}

void Timer::dump_config() {}

void Timer::start() {
  start_ms_ = millis();
  is_running_ = true;
}

void Timer::stop() {
  start_ms_ = 0;
  is_running_ = false;
}

int Timer::get_remaining() {
  if (!is_running_) return 0;
  int ellapsed_ms = millis() - start_ms_;
  int remaining = duration_ms_ - ellapsed_ms;
  return remaining;
}

}  // namespace  esphome::clock

#pragma once

#include "component.h"
#include "custom_api_device.h"

namespace esphome::clock {

/// A timer that triggers after a specified duration.
class Timer : public Component, public api::CustomAPIDevice {
 public:
  void setup() override;
  void dump_config() override;

  void set_duration(uint32_t duration_ms) { duration_ms_ = duration_ms; }
  uint32_t get_duration() { return duration_ms_; }

  void start();
  void stop();

  bool is_running() { return is_running_; }
  int get_remaining();

 protected:
  uint32_t duration_ms_ = 0;
  uint32_t start_ms_ = 0;
  bool is_running_ = false;
};

}  // namespace esphome::clock

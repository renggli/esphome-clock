#pragma once

#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/button/button.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/core/component.h"

namespace esphome::clock {

/// A stopwatch that tracks the time since it started.
class StopwatchComponent : public PollingComponent {
  SUB_BUTTON(start)
  SUB_BUTTON(stop)
  SUB_BUTTON(reset)
  SUB_BINARY_SENSOR(running)
  SUB_SENSOR(elapsed)

 public:
  void dump_config() override;
  void update() override;

  void start();
  void stop();
  void reset();

  bool is_running() { return is_running_; }
  uint32_t get_elapsed();

 protected:
  uint32_t start_ms_ = 0;
  uint32_t elapsed_ms_ = 0;
  bool is_running_ = false;
};

class StartStopwatchButton : public button::Button,
                             public Parented<StopwatchComponent> {
 public:
  StartStopwatchButton() = default;

 protected:
  void press_action() override { parent_->start(); }
};

class StopStopwatchButton : public button::Button,
                            public Parented<StopwatchComponent> {
 public:
  StopStopwatchButton() = default;

 protected:
  void press_action() override { parent_->stop(); }
};

class ResetStopwatchButton : public button::Button,
                             public Parented<StopwatchComponent> {
 public:
  ResetStopwatchButton() = default;

 protected:
  void press_action() override { parent_->reset(); }
};

}  // namespace  esphome::clock

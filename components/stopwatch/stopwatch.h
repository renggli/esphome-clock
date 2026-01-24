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

  /// Start the stopwatch. If already running, this does nothing.
  void start();

  /// Stop the stopwatch. If not running, this does nothing.
  void stop();

  /// Reset the stopwatch to zero. This stops the stopwatch if it is running.
  void reset();

  /// Check if the stopwatch is currently running.
  bool is_running() { return is_running_; }

  /// Get the elapsed time in milliseconds.
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

} // namespace  esphome::clock

#pragma once

#include "esphome/components/button/button.h"
#include "esphome/components/number/number.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include "esphome/core/component.h"

namespace esphome::clock {

/// The different states the timer can be in.
enum class TimerState {
  STOPPED,
  RUNNING,
  PAUSED,
  EXPIRED,
};

/// A timer that runs for a predefined duration.
class TimerComponent : public PollingComponent {
  SUB_NUMBER(duration)
  SUB_BUTTON(start)
  SUB_BUTTON(pause)
  SUB_BUTTON(stop)
  SUB_TEXT_SENSOR(state)
  SUB_SENSOR(remaining)

public:
  void dump_config() override;
  void update() override;

  /// Set the duration of the timer in milliseconds.
  void set_duration(uint32_t duration_ms);

  /// Get the duration of the timer in milliseconds.
  uint32_t get_duration() { return duration_ms_; }

  /// Start the timer. If paused, it resumes from where it left off.
  void start();

  /// Pause the timer. If running, it stops and can be resumed later.
  void pause();

  /// Stop the timer and reset the elapsed time.
  void stop();

  /// Get the current state of the timer as a string (Stopped, Running, Paused,
  /// Expired).
  const char *get_state();

  /// Get the remaining time in milliseconds.
  int get_remaining();

protected:
  TimerState state_ = TimerState::STOPPED;
  uint32_t duration_ms_ = 0;
  uint32_t elapsed_ms_ = 0;
  uint32_t start_ms_ = 0;
};

class DurationTimerNumber : public number::Number,
                            public Parented<TimerComponent> {
public:
  DurationTimerNumber() = default;

protected:
  void control(float value) override {
    publish_state(value);
    parent_->set_duration((uint32_t)round(value));
  }
};

class StartTimerButton : public button::Button,
                         public Parented<TimerComponent> {
public:
  StartTimerButton() = default;

protected:
  void press_action() override { parent_->start(); }
};

class PauseTimerButton : public button::Button,
                         public Parented<TimerComponent> {
public:
  PauseTimerButton() = default;

protected:
  void press_action() override { parent_->pause(); }
};

class StopTimerButton : public button::Button, public Parented<TimerComponent> {
public:
  StopTimerButton() = default;

protected:
  void press_action() override { parent_->stop(); }
};

} // namespace esphome::clock

#pragma once

#include "component.h"
#include "custom_api_device.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/binary_sensor/binary_sensor.h"

namespace  esphome::clock {

/// A stopwatch that tracks the time since it started.
class Stopwatch : public Component, public api::CustomAPIDevice {
 public:
  void set_running_sensor(binary_sensor::BinarySensor* running_sensor) {
    running_sensor_ = running_sensor;
  }
  void set_elapsed_sensor(sensor::Sensor* elapsed_sensor) {
    elapsed_sensor_ = elapsed_sensor;
  }

  void setup() override;
  void dump_config() override;

  /// (Re)starts the stopwatch.
  void start();

  /// Stops the stopwatch.
  void stop();

  /// Resets the stopwatch.
  void reset();

  /// Returns true, if the stopwatch is running.
  bool is_running() { return is_running_; }

  /// Returns the elapsed time of the stopwatch.
  uint32_t get_elapsed();

 protected:
  uint32_t start_ms_ = 0;
  uint32_t stop_ms_ = 0;
  uint32_t elapsed_ms_ = 0;
  bool is_running_ = false;

  binary_sensor::BinarySensor* running_sensor_ = nullptr;
  sensor::Sensor* elapsed_sensor_ = nullptr;
};

}  // namespace  esphome::clock

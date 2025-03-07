# ESPHome Clock

[![Build Status](https://github.com/renggli/esphome-clock/actions/workflows/ci.yml/badge.svg)](https://github.com/renggli/esphome-clock/actions/workflows/ci.yml)
[![GitHub Issues](https://img.shields.io/github/issues/renggli/esphome-clock.svg)](https://github.com/renggli/esphome-clock/issues)
[![GitHub Forks](https://img.shields.io/github/forks/renggli/esphome-clock.svg)](https://github.com/renggli/esphome-clock/network)
[![GitHub Stars](https://img.shields.io/github/stars/renggli/esphome-clock.svg)](https://github.com/renggli/esphome-clock/stargazers)
[![GitHub License](https://img.shields.io/badge/license-MIT-blue.svg)](https://raw.githubusercontent.com/renggli/esphome-clock/main/LICENSE)

A set of [ESPHome](https://esphome.io) custom components to simulate a timer and
stopwatch as seen on common clocks. Both interfaces can be controlled from
lambda functions or [Home Assistant](https://www.home-assistant.io/) sensors.

To include in your own configuration add the following external component:

```yaml
external_components:
  - source: github://renggli/esphome-clock
```

Then define one or more timers and stopwatches as follows:

```yaml
# A default stopwatch
stopwatch:
  id: my_stopwatch

# A default timer
timer:
  id: my_timer
```

Use lambda functions to start, stop or read the state of your clocks. Find the
documentation of their API in [timer.h](components/timer/timer.h) and
[stopwatch.h](components/stopwatch/stopwatch.h).

See [example.yaml](example.yaml) for the configuration necessary to integrate with Home
Assistant.

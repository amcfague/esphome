#pragma once

#include "esphome/components/uart/uart.h"
#include "esphome/core/component.h"
#include "esphome/core/defines.h"
#include "esphome/core/helpers.h"

#ifdef USE_BINARY_SENSOR
#include "esphome/components/binary_sensor/binary_sensor.h"
#endif
#ifdef USE_SENSOR
#include "esphome/components/sensor/sensor.h"
#endif

#include <array>
#include <stdint.h>

namespace esphome {
namespace ld2450 {

class PresenceZone;
class Target;

class LD2450Component : public ::esphome::Component, public ::esphome::uart::UARTDevice {
#ifdef USE_BINARY_SENSOR
  SUB_BINARY_SENSOR(any_presence)
#endif
#ifdef USE_SENSOR
  SUB_SENSOR(all_target_counts)
#endif

 public:
  LD2450Component();
  virtual ~LD2450Component();

  void dump_config() override;
  void loop() override;
  void setup() override;

  void add_zone(PresenceZone *zone);
  Target &get_target(uint8_t n) const;
  void set_throttle(uint16_t throttle) { throttle_ = throttle; };

 protected:
  inline void parseAndPublishRecord_(const uint8_t *buffer);

  // Specifies the last time fresh values were published.
  uint32_t next_periodic_millis_ = millis();

  // Specifies the number of milliseconds to wait in between publishing values
  // to the sensors.
  uint32_t throttle_;

  // 4 bytes header + 3 * (8 bytes) + 2 bytes = 30 bytes per record
  // NUM_TARGETS is used in case HILINK extends the number of targets.
  static const size_t LD2450_RECORD_LENGTH = 4 + NUM_TARGETS * 8 + 2;
  static const size_t BUFFER_CAPACITY = LD2450_RECORD_LENGTH * 2;

  uint8_t uartBuf_[BUFFER_CAPACITY];
  size_t uartBufSize_ = 0;

  std::array<Target *, NUM_TARGETS> targets_;

  std::vector<PresenceZone *> zones;
};

}  // namespace ld2450
}  // namespace esphome
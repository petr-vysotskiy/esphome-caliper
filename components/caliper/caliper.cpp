#include "esphome.h"
#include "esphome/core/log.h"
#include "caliper.h"

#define CLOCK_PIN D6
#define DATA_PIN  D5

#define MILLIS_BETWEEN_PACKETS 100

#define INVERTED true
#if INVERTED
// Sync and data input are default low
#define TRAILING_EDGE FALLING
#else  // Non-inverted
// Sync and data input are default high
#define TRAILING_EDGE RISING
#endif

volatile int last_millis = 0;
volatile u32_t packet = 0;
volatile bool data_ready = false;
volatile u32_t data_ready_packet = 0;
volatile u32_t last_data_ready_packet = 0;
volatile char last_value[33];
volatile int bit_count = 0;

void IRAM_ATTR clock_isr() {
  int new_data = digitalRead(DATA_PIN) ^ INVERTED;
  unsigned long new_millis = millis();
  if (new_millis - last_millis > MILLIS_BETWEEN_PACKETS)
  {
    if (bit_count == 24) {
      last_data_ready_packet = packet;
      data_ready = true;
    }
    packet = new_data;
    bit_count = 1;
  }
  else
  {
    packet = packet << 1 | new_data;
    bit_count++;
  }
  
  last_millis  = new_millis;
}

namespace esphome {
namespace caliper_sensor {

static const char *TAG = "sensor.caliper_sensor";

u32_t reverse(u32_t p)
{
    u32_t r=0;
    for(int i=0;i<24;i++) 
    {
      r |= ((p>>i) & 1)<<(23-i);
    }
    return r;
}

void CaliperSensor::setup() {
  pinMode(CLOCK_PIN, INPUT);
  pinMode(DATA_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(CLOCK_PIN), clock_isr, TRAILING_EDGE);
}

void CaliperSensor::loop() {
  float r = decode(last_data_ready_packet);
  ESP_LOGCONFIG(TAG, "Current value: %.4f", r);
}

float CaliperSensor::decode(u32_t p) {
  bool inch = p & 0x1;
  float sign = p & 0x8 ? -1.0 : 1.0;
  p &= 0xfffff0;
  p = reverse(p);
  
  float value = inch ? p * 0.0005 : p * 0.01;
  value *= sign;

  return value;
}

void CaliperSensor::update() {
  float result = this->decode(last_data_ready_packet);
  this->publish_state(result);
}

}
}
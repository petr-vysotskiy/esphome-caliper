#include "esphome.h"
#include "esphome/core/log.h"
#include "caliper.h"

#define CLOCK_PIN D6
#define DATA_PIN  D5

namespace esphome {
namespace caliper_sensor {

static const char *TAG = "sensor.caliper_sensor";

void CaliperSensor::setup() {
  pinMode(CLOCK_PIN, INPUT);
  pinMode(DATA_PIN, INPUT);
}

float CaliperSensor::decode() {
  int sign = 1;
  long value = 0;

  for (int i = 0; i < 24; i++) 
  {
    //wait until clock returns to HIGH - the first bit is not needed
    while ( digitalRead( CLOCK_PIN ) == LOW ) {}
    
    //wait until clock returns to LOW
    while ( digitalRead( CLOCK_PIN) == HIGH ) {}
    
    int t = digitalRead( DATA_PIN );
    digitalWrite(LED_BUILTIN, HIGH);
    if ( t != HIGH ) 
    {
      if (i < 20) 
      {
        value |= 1 << i;
      }
      if (i == 20) 
      {
        sign = -1;
      }
    }
  }

  return ( value * sign ) / 100.00;
}

void CaliperSensor::update() {
  while(1) {
    while (digitalRead( CLOCK_PIN) == HIGH ) {}
    unsigned long tempmicros = micros();
    while ( digitalRead( CLOCK_PIN ) == LOW ) {}
    if ( ( micros() - tempmicros ) > 100 ) {
      break;
    }
  }
  
  float result = decode();
  this->publish_state(result);
}

void CaliperSensor::dump_config() {
    ESP_LOGCONFIG(TAG, "Empty custom sensor");
}

}
}
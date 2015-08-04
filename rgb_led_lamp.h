#include "Arduino.h"

typedef struct {
  // the number of millis since the arduino started when this should fire
  unsigned long fire_time;
  byte red;
  byte green;
  byte blue;
} color_step;



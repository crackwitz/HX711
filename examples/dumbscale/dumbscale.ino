#include "HX711.h"

#define DOUT  3
#define CLK  2

HX711 scale(DOUT, CLK);

void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.println(scale.read());
}

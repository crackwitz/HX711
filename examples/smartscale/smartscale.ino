#include "HX711.h"

#define DOUT  3
#define CLK  2

HX711 scale(DOUT, CLK);

float calibration_factor = 717.0e-6; // works for me

void setup() {
  Serial.begin(9600);
  Serial.println("HX711 calibration sketch");
  Serial.println("taring...");

  scale.read_average(2);
  scale.tare();  //Reset the scale to 0
  scale.set_scale(calibration_factor); //Adjust to this calibration factor

  Serial.println("Press t to tare");
  Serial.println("Press a to increase calibration factor");
  Serial.println("Press z to decrease calibration factor");
}

void loop() {
  
  Serial.print("Reading: ");
  Serial.print(scale.get_units(4), 2);
  Serial.print(" g");
  //Serial.print(",raw: "); Serial.print(scale.read());
  Serial.println();


  if(Serial.available())
  {
    char temp = Serial.read();

    if (temp == 't')
    {
      scale.tare();
    }
    else if (temp == 'a')
    {
      calibration_factor += 0.2e-6;
      scale.set_scale(calibration_factor);
    }
    else if(temp == 'z')
    {
      calibration_factor -= 0.2e-6;
      scale.set_scale(calibration_factor);
    }
  }
}

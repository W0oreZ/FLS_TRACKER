#include "src/fls.h"

FLS sensor(PB1, PA7);
float avgCap, minCap, maxCap, error, oldcap = 0;
void setup() {
  Serial.begin(115200);
  delay(100);
  sensor.Logging(true);
}

void loop(){
  sensor.AllCap(&avgCap, &minCap, &maxCap, &error, 2000);
  if(error < (float)5)
  {

    Serial.print(avgCap,3);
    Serial.print(" nF (");
    Serial.print(floor(10000*(avgCap-oldcap))/10000,3);
    Serial.println(")");
    oldcap = avgCap;
  }
  else
  {
    Serial.println("SENSOR-ERROR");
  }
}
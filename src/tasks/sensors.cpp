#include <tasks/sensors.h>

FLS sensor(PB1, PB0);
float avgCap, minCap, maxCap, error, oldcap = 0;

void FLS_SENSOR::main(void *p) {

  sensor.Logging(true);


  while(1)
  {
    sensor.AllCap(&avgCap, &minCap, &maxCap, &error, 3000);
    if(error < (float)5)
    {
      float variation = abs(floor(10000*(avgCap-oldcap))/10000);
      Serial.print(avgCap,3);
      Serial.print(" nF (");
      Serial.print(variation,3);
      Serial.println(")");
      
      if(variation > (float)0.002)
      {
        oldcap = avgCap;
        ai1 = String(avgCap,3);
      }
    }
    else
    {
      Serial.println("SENSOR-ERROR");
    }

    vTaskDelay(100);
  }
}
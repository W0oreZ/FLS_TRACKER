#ifndef FLS_H
#define FLS_H

#include <Arduino.h>

class FLS
{
  public:
    FLS(int outPin, int inPin);
    void Calibrate(float strayCap, float pullupRes);
    float Cap();
    float RawCap();
    bool  AllCap(float* AvgCap,float* MinCap, float* MaxCap, float* error, uint32_t sampleTime);
    void Logging(bool on);
    void SetResolution(int bits);
  private:
    int _ADC_MAX = 4096;
    float _charg_percent = 2588.04;
    static float _ZEROCAP;
    static float _RES;
    int _disPin;
    int _chaPin;
    bool _showLogs;
};

#endif

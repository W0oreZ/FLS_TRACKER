#include "Arduino.h"
#include <fls.h>

#define ZERO_CAP (17.50);//80.50
#define RES_PULL (34.80);//30.8

float FLS::_ZEROCAP = ZERO_CAP;
float FLS::_RES = RES_PULL;

FLS::FLS(int outPin, int inPin)
{
    _disPin = outPin;
    _chaPin = inPin;
    digitalWrite(_disPin, LOW);
    digitalWrite(_chaPin, LOW);
    pinMode(_disPin, OUTPUT);
    pinMode(_chaPin, OUTPUT);
    analogReadResolution(12);
    _showLogs = false;
}

void FLS::Calibrate(float Cap, float Res)
{
    _ZEROCAP = Cap;
    _ZEROCAP = Res;
}

void FLS::Logging(bool debug)
{
    _showLogs = debug;
}

void FLS::SetResolution(int bits)
{
  analogReadResolution(bits);
  _ADC_MAX = (1 << bits) - 1;
  _charg_percent = (float)((_ADC_MAX*63.2)/100.00);
}

float FLS::Cap()
{
    float capacitance;

    pinMode(_disPin, INPUT);
    digitalWrite(_chaPin, HIGH);
    int rADC = analogRead(_disPin);
    digitalWrite(_chaPin, LOW);

    if (rADC < 1000)
    {
        pinMode(_disPin, OUTPUT);

        capacitance = ((float)rADC * _ZEROCAP / (float)(_ADC_MAX - rADC))/1000.00;

        if(_showLogs)
        {
            Serial.print(F("CAP:  "));
            Serial.print(capacitance, 3);
            Serial.print(F(" pF ("));
            Serial.print(rADC);
            Serial.println(F(") "));
        }
    }
    else
    {
        pinMode(_disPin, OUTPUT);
        delay(1);
        pinMode(_chaPin, INPUT_PULLUP);
        unsigned long u1 = micros();
        unsigned long t;
        int digVal;

        do
        {
            digVal = digitalRead(_chaPin);
            unsigned long u2 = micros();
            t = u2 > u1 ? u2 - u1 : u1 - u2;
        } 
        while ((digVal < 1) && (t < 400000L));

        pinMode(_chaPin, INPUT);  
        rADC = analogRead(_chaPin);
        digitalWrite(_disPin, HIGH);
        int dischargeTime = (int)(t / 1000L) * 5;
        delay(dischargeTime);   
        pinMode(_chaPin, OUTPUT);  
        digitalWrite(_chaPin, LOW);
        digitalWrite(_disPin, LOW);

        capacitance = (float)(-(float)t / _RES / log(1.0 - (float)rADC / (float)_ADC_MAX));

        if(_showLogs)
        {
            Serial.print("CAP: ");
            if (capacitance > 1000.0)
            {
                Serial.print(capacitance / 1000.0, 2);
                Serial.print(" uF");
            }
            else
            {
                Serial.print(capacitance, 2);
                Serial.print(" nF");
            }

            Serial.print(" (");
            Serial.print("tc= ");
            Serial.print(t);
            Serial.print(" us, ADC= ");
            Serial.print(rADC);
            Serial.println(")");
        }
    }
    capacitance = (int)(capacitance * 1000);
    capacitance = (float)capacitance/1000;
    return capacitance;
}

float FLS::RawCap()
{
    float capacitance;

    pinMode(_chaPin, INPUT);
    digitalWrite(_disPin, HIGH);
    int val = analogRead(_chaPin);
    digitalWrite(_disPin, LOW);
    pinMode(_chaPin, OUTPUT);

    if (val < _ADC_MAX - (_ADC_MAX / 42))
    {
        capacitance = (float)val * _ZEROCAP / (float)(max(_ADC_MAX - val, 1));
        if (_showLogs)
        {
          Serial.print("capacitance: ");
          Serial.println(val);
        }
    }
    else
    {
        delayMicroseconds(2);

        pinMode(_disPin, INPUT_PULLUP);
        unsigned long u1 = micros();
        
        delayMicroseconds(200);

        pinMode(_chaPin, INPUT);
        unsigned long u2 = micros();
        digitalWrite(_disPin, HIGH);
        pinMode(_disPin, OUTPUT);
        int adcVal = analogRead(_chaPin);
        val = _ADC_MAX - adcVal;

        unsigned long u3 = 0;
        unsigned long u4 = 0;
        if (val < _ADC_MAX / 100)
        {
            pinMode(_disPin, INPUT_PULLUP);
            pinMode(_chaPin, OUTPUT);
            u3 = micros();
            delay(20);
            pinMode(_chaPin, INPUT);
            u4 = micros();
            digitalWrite(_disPin, HIGH);
            pinMode(_disPin, OUTPUT);
            adcVal = analogRead(_chaPin);
            val = _ADC_MAX - adcVal;

            while (adcVal < _ADC_MAX - _ADC_MAX / 8)
            {
              pinMode(_chaPin, INPUT_PULLUP);
              delay(5);
              pinMode(_chaPin, INPUT);
              adcVal = analogRead(_chaPin);
            }
        }
        else
        {
            pinMode(_chaPin, INPUT_PULLUP);
            delay(1);
            pinMode(_chaPin, INPUT);
        }

        unsigned long t = (u2 > u1 ? u2 - u1 : u1 - u2) + (u4 > u3 ? u4 - u3 : u3 - u4);
      
        digitalWrite(_disPin, LOW);
        digitalWrite(_chaPin, LOW);
        pinMode(_chaPin, OUTPUT);

        capacitance = -(float)t * 1000.0 / _RES
                        / log(1.0 - (float)val / (float)_ADC_MAX);
        if (_showLogs)
        {
            Serial.print("High val cap t: ");
            Serial.print(t);
            Serial.print(", adc: ");
            Serial.println(val);
        }
    }
    return capacitance;
}

bool FLS::AllCap(float* AvgCap,float* MinCap, float* MaxCap, float* error, uint32_t sampleTime)
{
    bool res = true;
    uint32_t dt;
    int samples = 0;
    float prevcap = 0;
    if(_showLogs == true)
        _showLogs=false;
    
    *AvgCap = 0;*MinCap = FLS::Cap();*MaxCap = 0;*error = 0;prevcap = *MinCap;*error = 0;
    dt = millis();
    while(millis() - dt < sampleTime)
    {
        float cap = FLS::Cap();
        *AvgCap += cap;
        if(*MaxCap < cap)
            *MaxCap = cap;
        if(*MinCap > cap)
            *MinCap = cap;
        *error += prevcap - cap;
        prevcap = cap;
        samples++;
    }

    float wut = *error < 0 ? ((*error) * -1): *error;
    *error = wut;

    *AvgCap = *AvgCap / (float)samples;
    return res;
}
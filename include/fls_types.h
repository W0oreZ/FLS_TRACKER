#ifndef INC_TYPES_h
#define INC_TYPES_H

struct SIM_CONFIG
{ 
  char apn[25];
  char gprsUser[15];
  char gprsPass[15];
  char broker[25];
  int port;
  int interval;
};

#endif
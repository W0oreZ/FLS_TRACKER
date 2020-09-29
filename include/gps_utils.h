#ifndef GPS_UTILS_H
#define GPS_UTILS_H
#include <globals.h>

bool vGPS(TinyGsm* modem,String* lat, String *lon, String *speed, String *alt, String *vsat, String *usat);
String RAWGPS_TO_JSON(TinyGsm* modem, String imei, bool debug = false);
String GPS_TO_JSON(TinyGsm* modem, String imei, bool debug = false);


#endif
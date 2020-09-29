#include <gps_utils.h>

String RAWGPS_TO_JSON(TinyGsm* modem, String imei, bool debug)
{
  String gpsData = "";
  //String raw = modem.getGPSraw();

  return gpsData;
}

String GPS_TO_JSON(TinyGsm* modem, String imei, bool debug)
{
  String gpsData = "";
  
  String dt = modem->getGSMDateTime(TinyGSMDateTimeFormat::DATE_DATE) + " " + modem->getGSMDateTime(TinyGSMDateTimeFormat::DATE_TIME);
  
  String lat,lng,speed;
  String alt,gsmlev,gpslev;
  vGPS(modem,&lat,&lng,&speed,&alt,&gsmlev,&gpslev);

  gpsData += "{\"imei\":\""+ imei + "\",";
  gpsData += "\"dt\":\""+ dt + "\",";
  gpsData += "\"lat\":\""+ lat + "\",";
  gpsData += "\"lng\":\""+ lng + "\",";
  gpsData += "\"ai1\":\""+ ai1 + "\",";
  gpsData += "\"ai2\":\""+ ai2 + "\",";
  gpsData += "\"di1\":\""+ di1 + "\",";
  gpsData += "\"di2\":\""+ di2 + "\",";
  gpsData += "\"speed\":\""+ (speed) + "\",";
  gpsData += "\"alt\":\""+ (alt) + "\",";
  gpsData += "\"gsmlev\":\""+ (gsmlev) + "\",";
  gpsData += "\"gpslev\":\""+ (gpslev) + "\"}";
  //1,1,20191221111447.000,32.327250,-9.259618,133.842,0.00,285.2,1,,0.9,1.2,0.9,,11,11,,,51,,
  //1 -on/off, 1 - fix status , time, lat, lng, altitude, speed, angle, fixmode, ,hdop, pdop,vdop, , gpslev, gnslev, glonas, , c/n0max, hpa, vpa
  return gpsData;
}

// get GPS informations
  // works only with ans SIM808 V2
  bool vGPS(TinyGsm* modem,String* lat, String *lon, String *speed, String *alt, String *vsat, String *usat) {
    //String buffer = "";
    // char chr_buffer[12];
    bool fix = false;

    modem->sendAT(GF("+CGNSINF"));
    if (modem->waitResponse(GF(GSM_NL "+CGNSINF:")) != 1) {
      return false;
    }

    modem->stream.readStringUntil(','); // mode
    if ( modem->stream.readStringUntil(',').toInt() == 1 ) fix = true;
    modem->stream.readStringUntil(','); //utctime
    *lat =  modem->stream.readStringUntil(','); //lat
    *lon =  modem->stream.readStringUntil(','); //lon
    if (alt != NULL) *alt =  modem->stream.readStringUntil(','); //lon
    if (speed != NULL) *speed = modem->stream.readStringUntil(','); //speed
    modem->stream.readStringUntil(',');
    modem->stream.readStringUntil(',');
    modem->stream.readStringUntil(',');
    modem->stream.readStringUntil(',');
    modem->stream.readStringUntil(',');
    modem->stream.readStringUntil(',');
    modem->stream.readStringUntil(',');
    if (vsat != NULL) *vsat = modem->stream.readStringUntil(','); //viewed satelites
    if (usat != NULL) *usat = modem->stream.readStringUntil(','); //used satelites
    modem->stream.readStringUntil('\n');

    modem->waitResponse();

    return fix;
  }
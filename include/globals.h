#ifndef GLOBALS_H
#define GLOBALS_H
#include <Arduino.h>
#include <EEPROM.h>

#define TINY_GSM_MODEM_SIM808     //SIM module Version
#define GSM_AUTOBAUD_MIN 115200   //BAUDRATE 
#define GSM_AUTOBAUD_MAX 115200   //BAUDRATE
#define DEBUG_BAUDRATE 115200

#include <STM32FreeRTOS.h>
#include <STM32FreeRTOSConfig.h>
#include <TinyGsmClient.h>
#include <PubSubClient.h>

#include <fls_types.h>
#include <fls_memory.h>

#define DI1 PB12
#define DI2 PB13
#define DO1 PB14
#define DO2 PB15

extern HardwareSerial Serial2;


extern TaskHandle_t CONTROLLER_TASK_HANDLE;
extern TaskHandle_t NET_TASK_HANDLE;
extern TaskHandle_t SENSOR_TASK_HANDLE;

extern TinyGsm modem;
extern TinyGsmClient client;
extern PubSubClient Connection;

extern SIM_CONFIG simconfig;

extern String  ai1;
extern String  ai2;
extern String  di1;
extern String  di2;

#endif

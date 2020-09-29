#include<globals.h>
#include <tasks/network.h>
#include <tasks/sensors.h>

HardwareSerial Serial2(PA3, PA2);

TaskHandle_t CONTROLLER_TASK_HANDLE = NULL;
TaskHandle_t NET_TASK_HANDLE = NULL;
TaskHandle_t SENSOR_TASK_HANDLE = NULL;

String ai1 = "0.00";
String ai2 = "0.00";
String di1 = "0";
String di2 = "0";

SIM_CONFIG simconfig;

void SETUP_HARDWARE()
{
  //SETUP UART
  Serial.begin(DEBUG_BAUDRATE);
  Serial2.begin(GSM_AUTOBAUD_MAX);
  delay(500);
  //SETUP IOs
	pinMode(DI1, INPUT);
	pinMode(DI2, INPUT);
	pinMode(DO1, OUTPUT);
	pinMode(DO2, OUTPUT);
}

void LOAD_CONFIGURATION()
{
  if(!vDeserialize(&simconfig))
  {
    Serial.println("No Configuration Detected");
    Serial.println("Creating Factory Default Configuration");
    strcpy(simconfig.apn,"www.inwi.ma");
    strcpy(simconfig.broker,"41.140.242.182");
    simconfig.port = 1338;
    simconfig.interval = 5000;
    strcpy(simconfig.gprsPass,"");
    strcpy(simconfig.gprsUser,"");

    Serial.println("Saving Configuration");
    vSerialize(&simconfig);
  }
  else
  {
    Serial.println("Configuration Loaded");
  }
}

void setup() {
  SETUP_HARDWARE();
  LOAD_CONFIGURATION();
  delay(500);

  //Start the IDLE Task
  xTaskCreate(FLS_NETWORK::main, (const portCHAR *)"FLS_NETWORK_TASK", 512, NULL, 2, &NET_TASK_HANDLE);
  xTaskCreate(FLS_SENSOR::main, (const portCHAR *)"FLS_SENSOR_TASK", 512, NULL, 2, &SENSOR_TASK_HANDLE);

  vTaskStartScheduler();
  while(1);
}

void loop(){}
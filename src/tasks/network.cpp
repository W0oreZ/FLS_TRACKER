#include <tasks/network.h>
#include <gps_utils.h>

TinyGsm modem(Serial2);
TinyGsmClient client(modem);
PubSubClient Connection(client);

const char* DeviceID = "expV010_0001";
const char* topic = "exp/FLS/data";

String imei = "";

void FLS_NETWORK::main(void* p)
{

  Serial.println("PROGRAME RUNNING NET TASK");
  uint32_t lastReconnectAttempt = 0;
  uint32_t lastTransmition = 0;
  
  FLS_NETWORK::BOOTSTRAP_MODEM();
  for(;;)
  {
    uint32_t time = millis();

    if (!Connection.connected()) 
    {
      if (time - lastReconnectAttempt > 5000) 
      {
        lastReconnectAttempt = time;
        if (FLS_NETWORK::StartConnection()) {
          lastReconnectAttempt = 0;
        }
      }
      vTaskDelay(1000);
    }
    else
    {
      if(time - lastTransmition > (uint32_t)simconfig.interval)
      {
        String gpsData;
        byte buffer[1024];


        gpsData = GPS_TO_JSON(&modem, imei, false);
        Serial.println(gpsData);
        gpsData.getBytes(buffer,gpsData.length()+1);

        if(Connection.beginPublish(topic,gpsData.length()+1,false) == 1)
        {
          Connection.write(buffer,gpsData.length()+1);
          if(Connection.endPublish()==1)
          {
            lastTransmition = time;
          }
          else
          {
            Serial.println("SEND FAIL");
          }
        }
      }
    }
    
    Connection.loop();

    vTaskDelay(1);
  }
}

void FLS_NETWORK::BOOTSTRAP_MODEM()
{
  modem.init();
  imei = modem.getIMEI();
  modem.gprsConnect(simconfig.apn, simconfig.gprsUser, simconfig.gprsPass);

  if (!modem.isNetworkConnected()) 
  {
    Serial.print("Connecting to networks !");
    while(!modem.waitForNetwork(5000)) 
    {
      Serial.print(" !");
      vTaskDelay(1000);
    }
    Serial.println("");
  }

  while(!modem.gprsConnect(simconfig.apn, simconfig.gprsUser, simconfig.gprsPass)) 
  {
    Serial.println("Connecting to APN !");
    vTaskDelay(5000);
  }

  modem.isGprsConnected();
  Connection.setServer(simconfig.broker, simconfig.port);
  Connection.setCallback(FLS_NETWORK::MessageHandler);
  modem.enableGPS();
}

void FLS_NETWORK::MessageHandler(char* topic, byte* payload, unsigned int len) 
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("]: ");
  Serial.write(payload, len);
  Serial.println();
}

bool FLS_NETWORK::StartConnection() 
{
  if (!Connection.connect(DeviceID)) {
    return false;
  }

  Connection.publish("etrack", "exp20 connected");
  Connection.subscribe("stm32/topic");

  return Connection.connected();
}
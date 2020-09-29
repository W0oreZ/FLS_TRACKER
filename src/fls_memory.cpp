#include <fls_memory.h>

void vSerialize( SIM_CONFIG* simco )
{
  short len = 0;
  short i=0;

  EEPROM[i] = '#';
  i++;
  len = (short)strlen(simco->apn);
  for(short j=0;j<len;j++)
  {
    EEPROM[i] = simco->apn[j];
    i++;
  }
  EEPROM[i] = '#';
  i++;
  len = (short)strlen(simco->broker);
  for(short j=0;j<len;j++)
  {
    EEPROM[i] = simco->broker[j];
    i++;
  }
  EEPROM[i]='#';
  i++;
  len = (short)strlen(simco->gprsUser);
  for(short j=0;j<len;j++)
  {
    EEPROM[i] = simco->gprsUser[j];
    i++;
  }
  EEPROM[i] = '#';
  i++;
  len = (short)strlen(simco->gprsPass);
  for(short j=0;j<len;j++)
  {
    EEPROM[i] = simco->gprsPass[j];
    i++;
  }
  EEPROM[i] = '#';
  i++;
  EEPROM[i] = simco->port & 0x000000ff;
  i++;
  EEPROM[i] = ( simco->port & 0x0000ff00 ) >> 8;
  i++;
  EEPROM[i] = ( simco->port & 0x00ff0000 ) >> 16;
  i++;
  EEPROM[i] = ( simco->port & 0xff000000 ) >> 24;
  i++; 
  EEPROM[i] = '#';
  i++;
  EEPROM[i] = simco->interval & 0x000000ff;
  i++;
  EEPROM[i] = ( simco->interval & 0x0000ff00 ) >> 8;
  i++;
  EEPROM[i] = ( simco->interval & 0x00ff0000 ) >> 16;
  i++;
  EEPROM[i] = ( simco->interval & 0xff000000 ) >> 24;
  i++;
  EEPROM[i] = '#';
  i++;
}

bool vDeserialize( SIM_CONFIG* simco )
{
  if(EEPROM[0] == (byte)'#')
  {
    byte intArr[4];
    int len = 200;
    int* port;
    int* interval;

    short index = 1;
    short next = 0;

    for(index=1; index<len;index++)
    {
      if(EEPROM[index] == (byte)'#')
      {
        simco->apn[next] = '\0';
        next = 0;
        break;
      }
      else
      {
        simco->apn[next] = (char)EEPROM[index];
        next++;
      }
    }
    index++;
    for(index=index; index<len ; index++ )
    {
      if(EEPROM[index] == (byte)'#')
      {
        simco->broker[next] = '\0';
        next = 0;
        break;
      }
      else
      {
        simco->broker[next] = (char)EEPROM[index];
        next++;
      }
    }
    index++;
    for(index=index; index<len ; index++ )
    {
      if(EEPROM[index] == (byte)'#')
      {
        simco->gprsUser[next] = '\0';
        next = 0;
        break;
      }
      else
      {
        simco->gprsUser[next] = (char)EEPROM[index];
        next++;
      }
    }
    index++;
    for(index=index; index<len ; index++ )
    {
      if(EEPROM[index] == '#')
      {
        simco->gprsPass[next] = '\0';
        next = 0;
        break;
      }
      else
      {
        simco->gprsPass[next] = (char)EEPROM[index];
        next++;
      }
    }
    index++;
    for(index=index; index<len ; index++ )
    {
      if(EEPROM[index] == (byte)'#')
      {
        next = 0;
        break;
      }
      else
      {
        intArr[next] = EEPROM[index];
        next++;
      }
    }
    port = (int*)intArr;
    simco->port = *port;
    index++;
    for(index=index; index<len ; index++ )
    {
      if(EEPROM[index] == (byte)'#')
      {
        next = 0;
        break;
      }
      else
      {
        //Serial.print((char)EEPROM[index]);
        intArr[next] = EEPROM[index];
        next++;
      }
    }
    interval = (int*)intArr;
    simco->interval = *interval;
    index++;
    return true;
  }
  else
  {
    return false;
  }
}

#ifndef FLS_NETWORK_H
#define FLS_NETWORK_H
#include <globals.h>

namespace FLS_NETWORK
{
  void main(void* p);
  void BOOTSTRAP_MODEM();
  bool StartConnection();
  void MessageHandler(char* topic, byte* payload, unsigned int len);
}

#endif
#ifndef MEMORY_H
#define MEMORY_H
#include <globals.h>
#include <StringSplitter.h>

void vSerialize( SIM_CONFIG* simco );
bool vDeserialize( SIM_CONFIG* simco );

#endif
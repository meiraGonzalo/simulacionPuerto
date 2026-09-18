#ifndef AUTOMATICO_H_INCLUDED
#define AUTOMATICO_H_INCLUDED

#include "../simulacion.h"

int accionesAutomaticas (tEstadoSim* sis, tCola* camionesEspera, tCola* buquesEspera, tOperador* usuario);
int asignarCamiones (tEstadoSim* sis, tCola* camionesEspera);
int asignarBuques(tEstadoSim* sis, tCola* buquesEspera);
void desatracarVacios (tEstadoSim* sis, tOperador* usuario);

#endif // AUTOMATICO_H_INCLUDED

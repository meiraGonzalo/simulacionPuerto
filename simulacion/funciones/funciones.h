#ifndef FUNCIONES_H_INCLUDED
#define FUNCIONES_H_INCLUDED
#include "../simulacion.h"

int todosMuellesVacios (tEstadoSim* sis);
int todosMuellesLlenos (tEstadoSim* sis);
void mostrarMuellesOcupados (const tEstadoSim* sis);
int contarBuquesEspera (tCola* buquesEspera);
int contarCamionesEspera (tCola* camionesEspera);
void mostrarCamionesEspera (tCola* camionesEspera);
void mostrarZonas (tEstadoSim* sis);

int cmpMuelle (const void* a, const void* b);
int cmpZona (const void* a, const void* b);
int cmpMuelleCod (const void* a, const void* b);
int cmpZonaCod (const void* a, const void* b);

#endif // FUNCIONES_H_INCLUDED

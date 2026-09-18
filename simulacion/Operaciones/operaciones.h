#ifndef OPERACIONES_H_INCLUDED
#define OPERACIONES_H_INCLUDED
#include "../simulacion.h"

int operaciones (FILE* log, tConfiguracion* config, tEstadoSim* sis, tCola* camionesEspera, tCola* buquesEspera, tOperador* usuario);
int seleccionarOperacion(char* op, char* p1, char* p2);
int descargar(FILE* log, const tConfiguracion* config, tEstadoSim* sis, char* muelle, char* zona);
int reubicar(FILE* log, const tConfiguracion* config, tEstadoSim* sis, char* origen, char* destino, tOperador* usu);
int entregar(FILE* log, const tConfiguracion* config, tEstadoSim* sis, tCola* camionesEspera, tOperador* usuario);
void verEstado (tEstadoSim* sis, tCola* camionesEspera, tCola* buquesEspera, tOperador* usuario);
void esperar (FILE* log, tEstadoSim* sis);
void avanzar (FILE* log, tConfiguracion* config, tEstadoSim* sis, char* t);
void opsDisponibles();


#endif // OPERACIONES_H_INCLUDED

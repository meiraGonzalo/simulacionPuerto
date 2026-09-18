#ifndef SIMULACION_H_INCLUDED
#define SIMULACION_H_INCLUDED

#define TAM_BUFFER_OP 15
#define TAM_OPERACION 4
#define TAM_PARAM 4

#include "entidades/entidades.h"
#include "configuracion/configuracion.h"
#include "escenario/escenario.h"
#include "usuario/usuario.h"
#include "ProcesosAutomaticos/automatico.h"
#include "funciones/funciones.h"
#include "operaciones/operaciones.h"
#include <ctype.h>

void simulacion();
int bloqueoOperativo (tEstadoSim* sis, tCola* camionesEspera);
int mostrarResumenMov (FILE* log);
void mostrarResumenUser (const tConfiguracion* config, const tEstadoSim* sis, const tOperador* usuario);
int archivarJornada (FILE* log, tOperador* usuario, int cantMov);

#endif // SIMULACION_H_INCLUDED

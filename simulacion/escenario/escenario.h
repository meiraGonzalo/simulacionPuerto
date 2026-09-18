#ifndef ESTADO_H_INCLUDED
#define ESTADO_H_INCLUDED
#include "../configuracion/configuracion.h"
#include "../entidades/entidades.h"
#include <stdlib.h>
#include <stdio.h>
#define NOMBRE_ARCHIVO_PUERTO "puerto.txt"

typedef struct
{
    int tiempo_actual;
    tCola buques_programados;
    tCola camiones_programados;
    tLista muelles;
    tLista zonas;
} tEstadoSim;

void estadoIniciar (tEstadoSim* sistema);
void estadoVaciar (tEstadoSim* sistema);

int iniciarEscenario (const tConfiguracion* config, tEstadoSim* sistema);

void escrituraParametrosConfiguracion (FILE* arch, const tConfiguracion* config);
int cargaManifiestos(FILE* arch, const tConfiguracion* config, tEstadoSim* sistema);
int manifiestoBuques(FILE* arch, const tConfiguracion* config, tEstadoSim* sistema, char contenedoresGenerados[][TAM_MAXIMO_ID_CONTENEDOR], int limiteArribo);
int manifiestoCamiones(FILE* arch, const tConfiguracion* config, tEstadoSim* sistema, char contenedoresGenerados[][TAM_MAXIMO_ID_CONTENEDOR], int totalContenedores, int limiteArribo);
int cargaMuelles (const tConfiguracion* config, tEstadoSim* sistema);
int cargaZonas (const tConfiguracion* config, tEstadoSim* sistema);

int generarRandom (int minimo, int maximo);
void mezclarContenedores(char contenedores_generados[][TAM_MAXIMO_ID_CONTENEDOR], int cant_total_contenedores);
void generarTiemposRandom (int vect[], int cantidadElementos, int limite);
void ordenarTiempos(int tiemposGenerados[], int cantidadElementos);

#endif // ESTADO_H_INCLUDED

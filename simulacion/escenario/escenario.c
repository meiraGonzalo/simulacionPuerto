#include "escenario.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void estadoIniciar(tEstadoSim* sistema)
{
    crearCola(&sistema->buques_programados);
    crearCola(&sistema->camiones_programados);
    crearLista(&sistema->muelles);
    crearLista(&sistema->zonas);
    sistema->tiempo_actual = 0;
}

void estadoVaciar (tEstadoSim* sistema)
{
        tBuque buque_temp;
        while(desacolar(&sistema->buques_programados, &buque_temp, sizeof(tBuque)) == EXITO)
        {
            vaciarCola(&buque_temp.contenedores);
        }
        vaciarCola(&sistema->camiones_programados);
        vaciarLista(&sistema->muelles);
        vaciarLista(&sistema->zonas);
}

int iniciarEscenario(const tConfiguracion* config, tEstadoSim* sistema)
{
    FILE* arch = fopen (NOMBRE_ARCHIVO_PUERTO, "wt");
    if (!arch)
    {
        fprintf(stderr, "ERROR AL CREAR EL ARCHIVO DE PUERTO\n");
        return FALLO;
    }
    estadoIniciar(sistema);
    srand(time(NULL));

    escrituraParametrosConfiguracion(arch, config);
    if(!cargaManifiestos(arch, config, sistema))
    {
        fprintf(stderr, "ERROR: NO SE PUDO GENERAR CORRECTAMENTE EL ESCENARIO\n");
        estadoVaciar(sistema);
        fclose(arch);
        return FALLO;
    }

    cargaMuelles(config, sistema);
    cargaZonas(config, sistema);

    fclose(arch);
    puts("Escenario creado correctamente");
    return EXITO;
}

void escrituraParametrosConfiguracion (FILE* arch, const tConfiguracion* config)
{
    fprintf(arch, "JORNADA: %d\n", config->duracion_jornada);
    fprintf(arch, "MUELLES: %d\n", config->cantidad_muelles);
    fprintf(arch, "ZONAS: %d\n", config->cantidad_zonas);
    fprintf(arch, "CAPACIDAD_PILA: %d\n", config->capacidad_pila);
    fprintf(arch, "\n");
}

int cargaManifiestos(FILE* arch, const tConfiguracion* config, tEstadoSim* sistema)
{
    int maximoContenedores = (config->cantidad_contenedores)*(config->cantidad_buques), totalContenedores, limiteArribo = config->duracion_jornada-((config->tiempo_descarga+config->tiempo_carga)* maximoContenedores);
    int posiblePilas = (config->cantidad_zonas*config->capacidad_pila) - maximoContenedores;
    char contenedoresGenerados[maximoContenedores][TAM_MAXIMO_ID_CONTENEDOR];

    if(limiteArribo < 0)
    {
        fprintf(stderr, "ERROR: LA DURACION DE LA JORNADA NO PERMITE QUE SE FINALICE CORRECTAMENTE\n");
        return FALLO;
    }
    if (posiblePilas <0)
    {
        fprintf(stderr, "ERROR: LA CANTIDAD DE ZONAS Y LA CAPACIDAD DE LAS PILAS NO PERMITE QUE SE FINALICE CORRECTAMENTE\n");
        return FALLO;
    }

    totalContenedores = manifiestoBuques(arch, config, sistema, contenedoresGenerados, limiteArribo);
    if (!totalContenedores)
    {
        fprintf(stderr, "ERROR: NO SE PUDO COMPLETAR EL MANIFIESTO DE LOS BUQUES\n");
        return FALLO;
    }
    if(!manifiestoCamiones(arch, config, sistema, contenedoresGenerados, totalContenedores, limiteArribo))
    {
        fprintf(stderr, "ERROR: NO SE PUDO COMPLETAR EL MANIFIESTO DE LOS CAMIONES\n");
        return FALLO;
    }
    return EXITO;
}

int manifiestoBuques(FILE* arch, const tConfiguracion* config, tEstadoSim* sistema, char contenedoresGenerados[][TAM_MAXIMO_ID_CONTENEDOR], int limiteArribo)
{
    int i, j, cantBuques, cantContenedores, totalContenedores = 0, tiemposArribos[config->cantidad_buques];
    tBuque buque;
    tContenedor conte;

    fprintf(arch, "[BUQUES]\n");

    cantBuques = generarRandom(1, config->cantidad_buques);
    generarTiemposRandom(tiemposArribos, cantBuques, limiteArribo);
    ordenarTiempos(tiemposArribos, cantBuques);

    for (i=1; i<=cantBuques; i++)
    {
        fprintf(arch, "B%03d;", i);
        snprintf(buque.id,sizeof(buque.id), "B%03d", i);

        fprintf(arch,"T=%d;C=", tiemposArribos[i-1]);
        buque.minuto_llegada = tiemposArribos[i-1];

        cantContenedores = generarRandom(1, config->cantidad_contenedores);
        crearCola(&buque.contenedores);
        for (j=1; j<=cantContenedores; j++)
        {
            fprintf(arch, "C%d%02d", i, j); //print en puerto.txt
            sprintf(contenedoresGenerados[totalContenedores],"C%d%02d",i,j); //print en vector para camiones
            snprintf(conte.id,sizeof(conte.id), "C%d%02d",i,j); //print en contenedor
            totalContenedores++;

            if(!acolar(&buque.contenedores, &conte, sizeof(conte))) //acolamos contenedores dentro del buque
            {
                fprintf(stderr, "ERROR: NO HAY SUFICIENTE MEMORIA PARA CARGAR LOS BUQUES\n");
                vaciarCola(&buque.contenedores);
                return FALLO;
            }

            if (j<cantContenedores)
                fprintf(arch, ",");
        }
        fprintf(arch,"\n");

        if (!acolar(&sistema->buques_programados, &buque, sizeof(buque)))
        {
            fprintf(stderr, "ERROR: NO HAY SUFICIENTE MEMORIA PARA PROGRAMAR LOS BUQUES\n");
            vaciarCola(&buque.contenedores);
            return FALLO;
        }
    }
    fprintf(arch, "\n");
    return totalContenedores;
}

int manifiestoCamiones (FILE* arch, const tConfiguracion* config, tEstadoSim* sistema, char contenedoresGenerados[][TAM_MAXIMO_ID_CONTENEDOR], int totalContenedores, int limiteArribo)
{
    int cantCamiones, i, tiemposArribos[config->cantidad_camiones];
    tCamion camion;

    fprintf(arch, "[CAMIONES]\n");
    cantCamiones = generarRandom(1, config->cantidad_camiones);
    if (cantCamiones>totalContenedores)
        cantCamiones = totalContenedores;

    generarTiemposRandom(tiemposArribos, cantCamiones, limiteArribo);
    ordenarTiempos(tiemposArribos, cantCamiones);
    mezclarContenedores(contenedoresGenerados, totalContenedores);

    for (i=1;i<=cantCamiones;i++)
    {
        fprintf(arch, "K%03d;", i);
        snprintf(camion.id, sizeof(camion.id), "K%03d", i);

        camion.minuto_llegada = tiemposArribos[i-1];
        fprintf(arch,"T=%d;C=%s", tiemposArribos[i-1], contenedoresGenerados[i-1]);

        strcpy(camion.contenedor_solicitado, contenedoresGenerados[i-1]);

        if(!acolar(&sistema->camiones_programados, &camion, sizeof(camion)))
        {
            fprintf(stderr, "ERROR: NO HAY SUFICIENTE MEMORIA PARA PROGRAMAR LOS CAMIONES\n");
            return FALLO;
        }
        fprintf(arch, "\n");
    }
    return EXITO;
}

int cargaMuelles(const tConfiguracion* config, tEstadoSim* sistema)
{
    tMuelle muelle;
    int i;

    for(i=0; i < config->cantidad_muelles; i++)
    {
        sprintf(muelle.id, "M%d", i+1);
        muelle.buque_atracado = NULL;
        if (!ponerAlFinal(&sistema->muelles, &muelle, sizeof(tMuelle)))
        {
            fprintf(stderr, "ERROR: MEMORIA INSUFICIENTE PARA ESTABLECER LOS MUELLES\n");
            return FALLO;
        }
    }
    return EXITO;
}

int cargaZonas(const tConfiguracion* config, tEstadoSim* sistema)
{
    tZona zona;
    int i;

    for(i=0; i<config->cantidad_zonas; i++)
    {
        sprintf(zona.id, "Z%d", i+1);
        crearPila(&zona.contenedores);
        zona.cantElem = 0;
        if(!ponerAlFinal(&sistema->zonas, &zona, sizeof(tZona)))
        {
            fprintf (stderr, "ERROR: MEMORIA INSUFICIENTE PARA ESTABLECER LAS ZONAS DE ALMACENAMIENTO\n");
            return FALLO;
        }
    }
    return EXITO;
}

void generarTiemposRandom (int vect[], int cantidadElementos, int limite)
{
    int i;
    for(i=0; i<cantidadElementos; i++)
    {
        vect[i] = generarRandom(0, limite);
    }
}

void ordenarTiempos(int tiemposGenerados[], int cantidadElementos)
{
    int i, j, temp;

    for (i = 0; i < cantidadElementos - 1; i++)
    {
        for (j = 0; j < cantidadElementos - i - 1; j++)
        {
            if (tiemposGenerados[j] > tiemposGenerados[j + 1])
            {
                temp = tiemposGenerados[j];
                tiemposGenerados[j] = tiemposGenerados[j + 1];
                tiemposGenerados[j + 1] = temp;
            }
        }
    }
}

int generarRandom (int minimo, int maximo)
{
    return (rand() % (maximo - minimo + 1)) + minimo;
}

void mezclarContenedores(char contenedores_generados[][TAM_MAXIMO_ID_CONTENEDOR], int cant_total_contenedores)
{
    int i, random;
    char temp[TAM_MAXIMO_ID_CONTENEDOR];

    for (i = cant_total_contenedores - 1; i > 0; i--)
    {
        random = generarRandom(0, i);
        strcpy(temp, contenedores_generados[i]);
        strcpy(contenedores_generados[i], contenedores_generados[random]);
        strcpy(contenedores_generados[random], temp);
    }
}

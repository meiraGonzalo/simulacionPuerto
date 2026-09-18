#ifndef ENTIDADES_H_INCLUDED
#define ENTIDADES_H_INCLUDED

#include "../bibliotecas/tdaPila.h"
#include"../bibliotecas/tdaCola.h"
#include "../bibliotecas/tdaLista.h"
#include "../bibliotecas/tdaVector.h"
#include "../bibliotecas/tdaArbol.h"

#define TAM_MAXIMO_ID_CONTENEDOR 25
#define TAM_MAXIMO_ID_BUQUE 15
#define TAM_MAXIMO_ID_CAMION 15
#define TAM_MAXIMO_ID_MUELLE 15
#define TAM_MAXIMO_ID_ZONA 15
#define TAM_MAXIMO_NOMBRE_OP 51

#define EXITO 1
#define FALLO 0


typedef struct
{
    char id[TAM_MAXIMO_ID_CONTENEDOR];
} tContenedor;

typedef struct
{
    char id[TAM_MAXIMO_ID_BUQUE];
    int minuto_llegada;
    tCola contenedores;
} tBuque;

typedef struct
{
    char id[TAM_MAXIMO_ID_CAMION];
    int minuto_llegada;
    char contenedor_solicitado[TAM_MAXIMO_ID_CONTENEDOR];
} tCamion;

typedef struct
{
    char id[TAM_MAXIMO_ID_MUELLE];
    tBuque* buque_atracado;
} tMuelle;

typedef struct
{
    char id[TAM_MAXIMO_ID_ZONA];
    tPila contenedores;
    int cantElem;
} tZona;

typedef struct
{
    char nombre[TAM_MAXIMO_NOMBRE_OP];
    int puntacion;
    int contenEntregados;
    int buquesDescargados;
    int camionesPend;
    int cantReubicar;
    int jornadas;
} tOperador;

#endif // ENTIDADES_H_INCLUDED

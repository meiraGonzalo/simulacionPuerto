#ifndef TDACOLA_H_INCLUDED
#define TDACOLA_H_INCLUDED
#include "nodo.h"
#include <stdlib.h>
#include <string.h>

typedef struct
{
    tNodo* pri;
    tNodo* ult;
} tCola;

void crearCola (tCola *c);
int colaVacia (const tCola* c);
int colaLlena (const tCola* c, unsigned tam);
int acolar (tCola* c, const void* d, unsigned tam);
int desacolar (tCola* c, void* d, unsigned tam);
int verPrimero (const tCola* c, void* d, unsigned tam);
void vaciarCola (tCola* c);

#endif // TDACOLA_H_INCLUDED

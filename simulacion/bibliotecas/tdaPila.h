#ifndef TDAPILA_H_INCLUDED
#define TDAPILA_H_INCLUDED

#include <stdlib.h>
#include <string.h>
#include "nodo.h"

typedef tNodo* tPila;

void crearPila (tPila* p);
int pilaLlena (const tPila* p, unsigned tam);
int pilaVacia (const tPila* p);
int apilar (tPila* p, const void* d, unsigned tam);
int desapilar (tPila* p, void* d, unsigned tam);
int verTope (const tPila* p, void* d, unsigned tam);
void vaciarPila (tPila* p);

#endif // TDAPILA_H_INCLUDED

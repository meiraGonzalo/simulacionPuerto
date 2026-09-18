#ifndef TDALISTA_H_INCLUDED
#define TDALISTA_H_INCLUDED
#include "nodo.h"
#include <stdlib.h>
#include <string.h>

typedef tNodo* tLista;

typedef int (*Cmp)(const void*, const void*);
typedef tLista tIterador;

void crearLista(tLista *p);
int listaVacia(const tLista *p);
int listaLlena(const tLista *p, unsigned cantBytes);
void vaciarLista(tLista *p);
int ponerAlComienzo(tLista *p, const void *d, unsigned cantBytes);
int sacarPrimeroLista(tLista *p, void *d, unsigned cantBytes);
int verPrimeroLista(const tLista *p, void *d, unsigned cantBytes);
int ponerAlFinal(tLista *p, const void *d, unsigned cantBytes);
int sacarUltimoLista(tLista *p, void *d, unsigned cantBytes);
int verUltimoLista(const tLista *p, void *d, unsigned cantBytes);

void listaIniciarIterador(tIterador *it, const tLista *p);
int listaVerSiguiente(tIterador *it, void *d, unsigned cantBytes);
int listaVerSiguienteCircular(tIterador *it, const tLista *p, void *d, unsigned cantBytes);
int listaBuscarElemento(const tLista *p, void *d, unsigned cantBytes, Cmp cmp);
int listaBuscarElementoCod(const tLista *p, void *d, unsigned cantBytes, const void *cod, Cmp cmp);
int listaSacarElemento(tLista *p, void *d, unsigned cantBytes, Cmp cmp);
int listaModificarElemento(tLista *p, const void *d, unsigned cantBytes, Cmp cmp);

#endif // TDALISTA_H_INCLUDED

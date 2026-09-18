#include "tdaPila.h"

#define MINIMO(X,Y) ((X) <= (Y) ? (X) : (Y))
#define EXITO 1
#define FALLO 0

void crearPila(tPila* p)
{
    *p = NULL;
}

int pilaLlena(const tPila* p, unsigned tam)
{
    tNodo* aux = (tNodo*) malloc(sizeof(tNodo));
    void* info = malloc (tam);

    free(aux);
    free(info);

    if (aux == NULL || info == NULL) return EXITO;
    else return FALLO;
}

int pilaVacia(const tPila* p)
{
    if(*p == NULL) return EXITO;
    else return FALLO;
}

int apilar(tPila* p, const void* d, unsigned tam)
{
    tNodo* nuevo;

    if(((nuevo = (tNodo*)malloc(sizeof(tNodo))) == NULL) || (nuevo->dato = malloc(tam)) == NULL)
    {
        free(nuevo);
        return FALLO;
    }
    memcpy(nuevo->dato, d, tam);
    nuevo->tamDato = tam;
    nuevo->siguiente = *p;
    *p = nuevo;
    return EXITO;
}

int verTope(const tPila* p, void* d, unsigned tam)
{
    if (*p == NULL) return FALLO;

    memcpy(d, (*p)->dato, MINIMO(tam,(*p)->tamDato));
    return EXITO;
}

int desapilar(tPila* p, void* d, unsigned tam)
{
    tNodo* aux = *p;

    if (aux == NULL) return FALLO;
    *p = aux->siguiente;
    memcpy(d, aux->dato, MINIMO(tam, aux->tamDato));
    free(aux->dato);
    free(aux);
    return EXITO;
}

void vaciarPila(tPila* p)
{
    while (*p)
    {
        tNodo* aux = *p;
        *p = aux->siguiente;
        free(aux->dato);
        free(aux);
    }
}

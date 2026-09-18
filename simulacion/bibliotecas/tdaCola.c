#include "tdaCola.h"

#define MINIMO(X,Y) ((X) <= (Y) ? (X) : (Y))
#define EXITO 1
#define FALLO 0

void crearCola(tCola* c)
{
    c->pri = NULL;
    c->ult = NULL;
}

int colaLlena(const tCola*c, unsigned tam)
{
    tNodo* aux = (tNodo*)malloc(sizeof(tNodo));
    void* info = malloc(tam);
    free(aux);
    free(info);
    if (aux == NULL || info == NULL) return EXITO;
    else return FALLO;
}

int colaVacia(const tCola* c)
{
    if(c->pri == NULL) return EXITO;
    else return FALLO;
}

int acolar(tCola* c, const void* d, unsigned tam)
{
    tNodo* nuevo = (tNodo*) malloc (sizeof(tNodo));
    if ((nuevo == NULL) || ((nuevo->dato = malloc(tam)) == NULL))
    {
        free(nuevo);
        return FALLO;
    }
    memcpy(nuevo->dato, d, tam);
    nuevo->tamDato = tam;
    nuevo->siguiente = NULL;
    if (c->ult)
        c->ult->siguiente = nuevo;
    else
        c->pri = nuevo;
    c->ult = nuevo;
    return EXITO;
}

int verPrimero(const tCola* c, void* d, unsigned tam)
{
    if (c->pri == NULL)
        return FALLO;
    memcpy(d, c->pri->dato, MINIMO(tam, c->pri->tamDato));
    return EXITO;
}

int desacolar(tCola* c, void* d, unsigned tam)
{
    tNodo* aux = c->pri;
    if (aux == NULL) return FALLO;
    c->pri = aux->siguiente;
    memcpy(d, aux->dato, MINIMO(tam, aux->tamDato));
    free (aux->dato);
    free (aux);
    if (c->pri == NULL)
        c->ult = NULL;
    return EXITO;
}

void vaciarCola(tCola* c)
{
    while (c->pri)
    {
        tNodo* aux = c->pri;
        c->pri = aux->siguiente;
        free (aux->dato);
        free(aux);
    }
    c->ult = NULL;
}

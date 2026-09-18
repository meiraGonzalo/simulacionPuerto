#include "tdaLista.h"

#define MINIMO(X,Y) ((X) <= (Y) ? (X) : (Y))
#define EXITO 1
#define FALLO 0

void crearLista(tLista *p)
{
    *p = NULL;
}

int listaVacia(const tLista *p)
{
    return *p == NULL;
}

int listaLlena(const tLista *p, unsigned cantBytes)
{
    tNodo *aux = (tNodo *)malloc(sizeof(tNodo));
    void  *info = malloc(cantBytes);

    free(aux);
    free(info);
    return aux == NULL || info == NULL;
}

void vaciarLista(tLista *p)
{
    while(*p)
    {
        tNodo *aux = *p;

        *p = aux->siguiente;
        free(aux->dato);
        free(aux);
    }
}

int ponerAlComienzo(tLista *p, const void *d, unsigned cantBytes)
{
    tNodo *nue;

    if((nue = (tNodo *)malloc(sizeof(tNodo))) == NULL ||
       (nue->dato = malloc(cantBytes)) == NULL)
    {
        free(nue);
        return 0;
    }
    memcpy(nue->dato, d, cantBytes);
    nue->tamDato = cantBytes;
    nue->siguiente = *p;
    *p = nue;
    return 1;
}

int sacarPrimeroLista(tLista *p, void *d, unsigned cantBytes)
{
    tNodo *aux = *p;

    if(aux == NULL)
        return 0;
    *p = aux->siguiente;
    memcpy(d, aux->dato, MINIMO(cantBytes, aux->tamDato));
    free(aux->dato);
    free(aux);
    return 1;
}

int verPrimeroLista(const tLista *p, void *d, unsigned cantBytes)
{
    if(*p == NULL)
        return 0;
    memcpy(d, (*p)->dato, MINIMO(cantBytes, (*p)->tamDato));
    return 1;
}

int ponerAlFinal(tLista *p, const void *d, unsigned cantBytes)
{
    tNodo *nue;

    while(*p)
        p = &(*p)->siguiente;
    if((nue = (tNodo *)malloc(sizeof(tNodo))) == NULL ||
       (nue->dato = malloc(cantBytes)) == NULL)
    {
        free(nue);
        return 0;
    }
    memcpy(nue->dato, d, cantBytes);
    nue->tamDato = cantBytes;
    nue->siguiente = NULL;
    *p = nue;
    return 1;
}

int sacarUltimoLista(tLista *p, void *d, unsigned cantBytes)
{
    if(*p == NULL)
        return 0;
    while((*p)->siguiente)
        p = &(*p)->siguiente;
    memcpy(d, (*p)->dato, MINIMO(cantBytes, (*p)->tamDato));

    free((*p)->dato);
    free(*p);
    *p = NULL;
    return 1;
}

int verUltimoLista(const tLista *p, void *d, unsigned cantBytes)
{
    if(*p == NULL)
        return 0;
    while((*p)->siguiente)
        p = &(*p)->siguiente;
    memcpy(d, (*p)->dato, MINIMO(cantBytes, (*p)->tamDato));
    return 1;
}

void listaIniciarIterador(tIterador* it, const tLista *p)
{
    *it = *p;
}

int listaVerSiguiente(tIterador* it, void *d, unsigned cantBytes)
{
    if(*it == NULL)
        return FALLO;

    memcpy(d, (*it)->dato, MINIMO(cantBytes, (*it)->tamDato));
    *it = (*it)->siguiente;

    return EXITO;
}

int listaVerSiguienteCircular(tIterador* it, const tLista *p, void *d, unsigned cantBytes)
{
    if (*p == NULL)
        return FALLO;

    if (*it == NULL)
        *it = *p;

    memcpy(d, (*it)->dato, MINIMO(cantBytes, (*it)->tamDato));

    *it = (*it)->siguiente;

    return EXITO;
}

int listaBuscarElemento(const tLista *p, void *d, unsigned cantBytes, Cmp cmp)
{
    while(*p)
    {
        if(cmp((*p)->dato, d) == 0)
        {
            memcpy(d, (*p)->dato, MINIMO(cantBytes, (*p)->tamDato));
            return EXITO;
        }
        p = &(*p)->siguiente;
    }

    return FALLO;
}

int listaBuscarElementoCod(const tLista *p, void *d, unsigned cantBytes, const void *cod, Cmp cmp)
{
    while(*p)
    {
        if(cmp((*p)->dato, cod) == 0)
        {
            memcpy(d, (*p)->dato, MINIMO(cantBytes, (*p)->tamDato));
            return EXITO;
        }
        p = &(*p)->siguiente;
    }

    return FALLO;
}

int listaSacarElemento(tLista *p, void *d, unsigned cantBytes, Cmp cmp)
{
    while(*p)
    {
        if(cmp((*p)->dato, d) == 0)
        {
            tNodo *aux = *p;

            *p = aux->siguiente;
            memcpy(d, aux->dato, MINIMO(cantBytes, aux->tamDato));

            free(aux->dato);
            free(aux);

            return EXITO;
        }
        p = &(*p)->siguiente;
    }

    return FALLO;
}

int listaModificarElemento(tLista *p, const void *d, unsigned cantBytes, Cmp cmp)
{
    while(*p)
    {
        if(cmp((*p)->dato, d) == 0)
        {
            // Se sobrescribe la información actualizada directamente en la memoria del nodo original
            memcpy((*p)->dato, d, MINIMO(cantBytes, (*p)->tamDato));
            return EXITO;
        }
        p = &(*p)->siguiente;
    }

    return FALLO;
}

#include "tdaVector.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define FALLO 0
#define EXITO 1
#define MINIMO(X,Y) ((X) <= (Y) ? (X) : (Y))

int vectorCrear(tVector* v, unsigned tamElem)
{
    v->ce=0;
    v->cap = 0;
    v->tamElem = 0;

    v->vec = malloc(CAP_INI * tamElem);

    if(v->vec == NULL)
    {
        return FALLO;
    }

    v->cap = CAP_INI;
    v->tamElem = tamElem;

    return EXITO;
};

int vectorInsertar(tVector* v, const void* elem)
{
    void* dirFinal;

    if(v->ce == v->cap)
    {
        if(!vectorAmpliar(v))
        {
            return FALLO;
        }
    }

    dirFinal = (char*)v->vec + (v->ce * v->tamElem);

    memcpy(dirFinal, elem, v->tamElem);

    v->ce++;

    return EXITO;
}

int vectorAmpliar(tVector* v)
{
    unsigned nCap = v->cap * FACTOR_INCR;
    void* nVec = realloc(v->vec, nCap * v->tamElem);

    if(!nVec)
    {
        return FALLO;
    }

    v->cap = nCap;
    v->vec = nVec;

    return EXITO;
}

int vectorObtenerElemento(const tVector* v, unsigned pos, void* elem, unsigned tam)
{
    void* dirOrigen;

    if (v == NULL || v->vec == NULL)
    {
        return FALLO;
    }

    if (pos >= v->ce)
    {
        return FALLO;
    }

    dirOrigen = (char*)v->vec + (pos * v->tamElem);

    memcpy(elem, dirOrigen, MINIMO(tam, v->tamElem));

    return EXITO;
}

int vectorModificarElemento(tVector* v, unsigned pos, const void* elem)
{
    if (v == NULL || v->vec == NULL || elem == NULL)
    {
        return FALLO;
    }

    if (pos >= v->ce)
    {
        return FALLO;
    }

    void* dirDestino = (char*)v->vec + (pos * v->tamElem);

    memcpy(dirDestino, elem, v->tamElem);

    return EXITO;
}

int vectorEliminarElemento(tVector* v, unsigned pos)
{
    if (v == NULL || v->vec == NULL)
    {
        return FALLO;
    }

    if (pos >= v->ce)
    {
        return FALLO;
    }

    if (pos < v->ce - 1)
    {
        void* dirDestino = (char*)v->vec + (pos * v->tamElem);
        void* dirOrigen = (char*)v->vec + ((pos + 1) * v->tamElem);
        unsigned bytesAMover = (v->ce - pos - 1) * v->tamElem;

        memmove(dirDestino, dirOrigen, bytesAMover);
    }

    v->ce--;

    return EXITO;
}

void vectorDestruir(tVector* v)
{
    free(v->vec);
    v->vec = NULL;
    v->ce = 0;
    v->cap = 0;
    v->tamElem = 0;
}

int vectorCantidadElmentos(const tVector* v)
{
    return v->ce;
}

int vectorInsertarOrdenado(tVector* v, const void* elem, Cmp cmp)
{
    unsigned i = 0;
    void* dirActual;

    if (v == NULL || v->vec == NULL || elem == NULL || cmp == NULL)
    {
        return FALLO;
    }

    if (v->ce == v->cap)
    {
        if (!vectorAmpliar(v))
        {
            return FALLO;
        }
    }

    while (i < v->ce)
    {
        dirActual = (char*)v->vec + (i * v->tamElem);

        if (cmp(elem, dirActual) <= 0)
        {
            break;
        }
        i++;
    }

    if (i < v->ce)
    {
        void* dirDestino = (char*)v->vec + ((i + 1) * v->tamElem);
        void* dirOrigen = (char*)v->vec + (i * v->tamElem);
        unsigned bytesAMover = (v->ce - i) * v->tamElem;

        memmove(dirDestino, dirOrigen, bytesAMover);
    }

    void* dirInsercion = (char*)v->vec + (i * v->tamElem);
    memcpy(dirInsercion, elem, v->tamElem);

    v->ce++;

    return EXITO;
}

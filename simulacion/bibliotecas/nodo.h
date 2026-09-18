#ifndef NODO_H_INCLUDED
#define NODO_H_INCLUDED

typedef struct sNodo
{
    void* dato;
    unsigned tamDato;
    struct sNodo* siguiente;
} tNodo;

#endif // NODO_H_INCLUDED

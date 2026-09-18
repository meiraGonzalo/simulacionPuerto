#ifndef TDAVECTOR_H_INCLUDED
#define TDAVECTOR_H_INCLUDED
#define CAP_INI 50
#define FACTOR_INCR 2

typedef struct
{
    void* vec;
    unsigned ce;
    unsigned cap;
    unsigned tamElem;
} tVector;

typedef int (*Cmp)(const void*, const void*);

int vectorCrear(tVector* v, unsigned tamElem);
int vectorInsertar(tVector* v, const void* elem);
int vectorAmpliar(tVector* v);
int vectorObtenerElemento(const tVector* v, unsigned pos, void* elem, unsigned tam);
int vectorModificarElemento(tVector* v, unsigned pos, const void* elem);
int vectorEliminarElemento(tVector* v, unsigned pos);
void vectorDestruir(tVector* v);
int vectorCantidadElmentos (const tVector* v);
int vectorInsertarOrdenado(tVector* v, const void* elem, Cmp cmp);

#endif // TDAVECTOR_H_INCLUDED

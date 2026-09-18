#ifndef RANKING_H_INCLUDED
#define RANKING_H_INCLUDED

#include "../simulacion/bibliotecas/tdaVector.h"
#include "../simulacion/entidades/entidades.h"

void ranking ();
int cmpPuntuacion (const void* a, const void* b);
void mostrarHistorialOperador(const char* nombreBuscado);

#endif // RANKING_H_INCLUDED

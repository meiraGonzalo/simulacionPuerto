#include <stdio.h>
#include <stdlib.h>
#include "menu/menu.h"
#include "simulacion/simulacion.h"
#include "ranking/ranking.h"

#define EXIT 4
#define SIM 1
#define RANK 2
#define HISTORIAL 3
#define STANDBY 5

void dirigir (int opcion);

void ranking();

int main()
{
    int opcion = STANDBY;

    while (opcion != EXIT)
    {
        menu();
        seleccionarMenu(&opcion);
        dirigir(opcion);
    }

    return 0;
}

void dirigir(int opcion)
{
    char nombre[TAM_MAXIMO_NOMBRE_OP];
    switch(opcion)
    {
    case SIM:
        simulacion();
        break;
    case RANK:
        ranking();
        break;
    case HISTORIAL:
        puts("Ingrese el nombre del operador:");
        fgets(nombre, TAM_MAXIMO_NOMBRE_OP, stdin);
        nombre[strcspn(nombre, "\n")] = '\0';
        mostrarHistorialOperador(nombre);
    break;
    case EXIT:
        puts("");
        puts ("Cerrando aplicacion...\n");
        break;
    }
}

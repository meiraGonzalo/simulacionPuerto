#include "menu.h"
#include <stdio.h>
#include <stdlib.h>

#define EXIT 4
#define SIM 1
#define TAM_BUFFER 50

void menu()
{
    system("cls");

    printf("===== OPERACION CONTRARRELOJ =====\n");
    puts("");
    printf("1. Iniciar nueva jornada\n");
    printf("2. Ver ranking de operadores\n");
    printf ("3. Mostrar historial de un operador\n");
    printf("4. Salir\n");
    puts("");
    printf ("Seleccione una opcion: ");
}


void seleccionarMenu(int* opcion)
{
    int temp, esValido = 0;
    char bufferEntrada [TAM_BUFFER];
    do {
        fgets(bufferEntrada, sizeof(bufferEntrada), stdin);
        if (sscanf(bufferEntrada, "%d", &temp) != 1)
            fprintf(stderr, "ERROR: ENTRADA INVALIDA\n");
        else if (temp < SIM || temp > EXIT)
            fprintf(stderr, "ERROR: OPCION INVALIDA EN EL MENU\n");
            else
                esValido = 1;
    } while (esValido == 0);
    *opcion = temp;
}

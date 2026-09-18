#include "ranking.h"
#include "../simulacion/simulacion.h"

void ranking()
{
    FILE* ops;
    tVector v;
    tOperador usuario;
    int i, cant;

    ops = fopen("operadores.dat", "rb");
    if (!ops)
    {
        fprintf(stderr, "ERROR: NO SE PUDO ABRIR EL ARCHIVO DE OPERADORES\n");
        return;
    }

    if (!vectorCrear(&v, sizeof(tOperador)))
    {
        fprintf(stderr, "ERROR: SIN MEMORIA PARA CREAR EL VECTOR DEL RANKING\n");
        fclose(ops);
        return;
    }

    while (fread(&usuario, sizeof(tOperador), 1, ops))
    {
        if (!vectorInsertarOrdenado(&v, &usuario, cmpPuntuacion))
        {
            fprintf(stderr, "ERROR: SIN MEMORIA PARA INSERTAR OPERADOR EN EL RANKING\n");
            vectorDestruir(&v);
            fclose(ops);
            return;
        }
    }

    cant = vectorCantidadElmentos(&v);
    puts("RANKING DE OPERADORES\n");

    for (i = 0; i < cant; i++)
    {
        vectorObtenerElemento(&v, i, &usuario, sizeof(tOperador));
        printf("OPERADOR: %s\tPUNTUACION ACUMULADA: %d\tJORNADAS REALIZADAS: %d\n", usuario.nombre, usuario.puntacion, usuario.jornadas);
    }

    vectorDestruir(&v);
    fclose(ops);
    system("pause");
}

int cmpPuntuacion(const void* a, const void* b)
{
    const tOperador* ap = (tOperador*)a;
    const tOperador* bp = (tOperador*)b;
    return (ap->puntacion-bp->puntacion);
}

void mostrarHistorialOperador(const char* nombreBuscado)
{
    FILE* arch = fopen("jornadas.dat", "rb");
    if (!arch) return;

    tJornada cabecera;
    tMovimiento mov;
    int jornada_nro = 1;

    while (fread(&cabecera, sizeof(tJornada), 1, arch) == 1) {

        if (strcmpi(cabecera.nombreOp, nombreBuscado) == 0) {
            printf("\n--- Jornada %d de %s (Puntos: %d) ---\n", jornada_nro, cabecera.nombreOp, cabecera.puntuacion);

            for (int i = 0; i < cabecera.cantidad_movimientos; i++) {
                fread(&mov, sizeof(tMovimiento), 1, arch);
                printf("%s\n", mov.operacion);
            }
            jornada_nro++;
        }
        else {
            long bytes_a_saltar = cabecera.cantidad_movimientos * sizeof(tMovimiento);
            fseek(arch, bytes_a_saltar, SEEK_CUR);
        }
    }
    fclose(arch);
    system("pause");
}

#include "usuario.h"
#include <stdio.h>
#include <ctype.h>


void iniciarUsuario(tOperador* usuario)
{
    char linea[TAM_MAXIMO_NOMBRE_OP];

    do{
    puts("Ingrese el nombre del operador de la jornada:");
    fgets(linea, TAM_MAXIMO_NOMBRE_OP, stdin);
    } while (!lineaValida(linea));
    linea[strcspn(linea, "\n")] = '\0';
    strcpy(usuario->nombre, linea);
    usuario->puntacion = 0;
    usuario->buquesDescargados = 0;
    usuario->contenEntregados = 0;
    usuario->cantReubicar = 0;
    usuario->camionesPend = 0;
    usuario->jornadas = 0;
    generarIndice(usuario);
    puts ("Usuario iniciado exitosamente");
}

int lineaValida (const char *linea)
{
    while (*linea != '\0')
    {
        if (!isspace((unsigned char)*linea)) {
            return EXITO;
        }
        linea++;
    }
    return FALLO;
}

int cmpIndice(const void* a, const void* b)
{
    const tIndice *ai = (const tIndice *)a;
    const tIndice *bi = (const tIndice *)b;

    return strcmpi(ai->nombre, bi->nombre);
}

void accionGuardarIndice(void *info, unsigned tam, unsigned n, void *params)
{
    FILE *pf = (FILE *)params;
    fwrite(info, tam, 1, pf);
}

void generarIndice (tOperador* usuario)
{
    FILE* index, *ops = NULL;
    tArbolBinBusq arbolIndices;
    tIndice ind;

    strcpy(ind.nombre, usuario->nombre);

    crearArbolBinBusq(&arbolIndices);
    cargarArchivoBinOrdenadoArbolBinBusq(&arbolIndices, "indices.idx", sizeof(tIndice));

    if(!buscarElemArbolBinBusq(&arbolIndices, &ind, sizeof(tIndice),cmpIndice))
    {
        ops = fopen("operadores.dat", "r+b");
        if(!ops)
        {
            ops = fopen ("operadores.dat", "w+b");
            if (!ops)
            {
                fprintf(stderr, "ERROR: NO SE PUDO CREAR EL ARCHIVO\n");
                return;
            }
        }
        fseek(ops, 0, SEEK_END);
        ind.pos = ftell(ops) / sizeof(tOperador);
        fwrite(usuario, sizeof(tOperador), 1, ops);
        insertarArbolBinBusq(&arbolIndices, &ind, sizeof(tIndice), cmpIndice);
        fclose(ops);
    }

    index = fopen("indices.idx", "wb");
    if (index)
    {
        recorrerEnOrdenArbolBinBusq(&arbolIndices, index, accionGuardarIndice);
        fclose(index);
    }
    vaciarArbolBinBusq(&arbolIndices);
}

void actualizarRegUsuario (tOperador* usuario)
{
    tArbolBinBusq arbolIndices;
    tIndice ind;
    FILE* ops;
    tOperador historico;

    strcpy(ind.nombre, usuario->nombre);

    crearArbolBinBusq(&arbolIndices);
    cargarArchivoBinOrdenadoArbolBinBusq(&arbolIndices, "indices.idx", sizeof(tIndice));

    if (buscarElemArbolBinBusq(&arbolIndices, &ind, sizeof(tIndice), cmpIndice))
    {
        ops = fopen ("operadores.dat", "r+b");
        if (ops)
        {
            fseek(ops, ind.pos*sizeof(tOperador), SEEK_SET);
            fread(&historico, sizeof(tOperador), 1, ops);

            historico.puntacion += usuario->puntacion;
            historico.buquesDescargados += usuario->buquesDescargados;
            historico.contenEntregados += usuario->contenEntregados;
            historico.camionesPend += usuario->camionesPend;
            historico.cantReubicar += usuario->cantReubicar;
            historico.jornadas++;

            fseek(ops, -1*(long)sizeof(tOperador), SEEK_CUR);
            fwrite(&historico, sizeof(tOperador), 1, ops);

            fclose(ops);
        }
    }
    vaciarArbolBinBusq(&arbolIndices);
}

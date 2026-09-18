#include "funciones.h"

int todosMuellesVacios (tEstadoSim* sis)
{
    tIterador it;
    tMuelle m;
    int todosVacios = EXITO;

    listaIniciarIterador(&it, &sis->muelles);
    while(listaVerSiguiente(&it, &m, sizeof(tMuelle)))
    {
        if (m.buque_atracado != NULL)
        {
            todosVacios = FALLO;
        }
    }
    return todosVacios;
}

int todosMuellesLlenos (tEstadoSim* sis)
{
    tIterador it;
    tMuelle m;
    int todosLlenos = EXITO;

    listaIniciarIterador(&it, &sis->muelles);
    while(listaVerSiguiente(&it, &m, sizeof(tMuelle)))
    {
        if (m.buque_atracado == NULL)
        {
            todosLlenos = FALLO;
        }
    }
    return todosLlenos;
}

int cmpMuelle (const void* a, const void* b)
{
    tMuelle *ma, *mb;

    ma = (tMuelle*)a;
    mb = (tMuelle*)b;

    return (strcmpi(ma->id, mb->id));
}

int cmpZona (const void* a, const void* b)
{
    tZona *za, *zb;

    za = (tZona*)a;
    zb = (tZona*)b;

    return (strcmpi(za->id, zb->id));
}

int cmpMuelleCod (const void* a, const void* b)
{
    tMuelle *ma;
    char *mb;

    ma = (tMuelle*)a;
    mb = (char*)b;

    return (strcmpi(ma->id, mb));
}

int cmpZonaCod (const void* a, const void* b)
{
    tZona *za;
    char *zb;

    za = (tZona*)a;
    zb = (char*)b;

    return (strcmpi(za->id, zb));
}

void mostrarMuellesOcupados (const tEstadoSim* sis)
{
    tIterador it;
    tMuelle m;
    tBuque* b;
    int alguno = FALLO;
    tContenedor c;

    puts("");
    listaIniciarIterador(&it, &sis->muelles);
    while(listaVerSiguiente(&it, &m, sizeof(tMuelle)))
    {
        if (m.buque_atracado != NULL)
        {
            b = m.buque_atracado;
            verPrimero(&b->contenedores, &c, sizeof(tContenedor));
            printf("Buque [%s] atracado en Muelle [%s] -> Proximo Contenedor [%s]\n", b->id, m.id, c.id);
            alguno = EXITO;
        }
    }
    if (!alguno)
        printf ("No hay Muelles ocupados\n");
}

int contarBuquesEspera (tCola* buquesEspera)
{
    int i= 0;
    tBuque b;
    tCola aux;
    crearCola(&aux);
    while(desacolar(buquesEspera, &b, sizeof(tBuque)))
    {
        i++;
        acolar(&aux, &b, sizeof(tBuque));
    }
    while(desacolar(&aux, &b, sizeof(tBuque)))
        acolar(buquesEspera, &b, sizeof(tBuque));

    return i;
}

int contarCamionesEspera (tCola* camionesEspera)
{
    int i= 0;
    tCamion c;
    tCola aux;
    crearCola(&aux);
    while(desacolar(camionesEspera, &c, sizeof(tCamion)))
    {
        i++;
        acolar(&aux, &c, sizeof(tCamion));
    }
    while(desacolar(&aux, &c, sizeof(tCamion)))
        acolar(camionesEspera, &c, sizeof(tCamion));

    return i;
}

void mostrarCamionesEspera (tCola* camionesEspera)
{
    int i = 0, alguno  = FALLO;
    tCamion c;
    tCola aux;
    crearCola(&aux);
    puts("");
    while (i < 3 && desacolar(camionesEspera, &c, sizeof(tCamion)))
    {
        printf ("Camion [%s] esperando el contenedor [%s]\n", c.id, c.contenedor_solicitado);
        alguno = EXITO;
        acolar(&aux, &c, sizeof(tCamion));
    }
    while (desacolar(camionesEspera, &c, sizeof(tCamion))) //terminamos de descargar la cola original
        acolar(&aux, &c, sizeof(tCamion));
    while(desacolar(&aux, &c, sizeof(tCamion))) //restauramos original
        acolar(camionesEspera, &c, sizeof(tCamion));
    if (!alguno)
        printf ("No hay Camiones en espera\n");
}

void mostrarZonas (tEstadoSim* sis)
{
    tZona z; tContenedor c; tIterador it; int alguno;
    tPila aux;  //pila auxiliar

    listaIniciarIterador(&it, &sis->zonas);
    while(listaVerSiguiente(&it, &z, sizeof(tZona)))
    {
        crearPila(&aux);
        puts("");
        alguno = FALLO;
        printf ("La Zona de Almacenamiento [%s] actualmente contiene: ", z.id);

        while (desapilar(&z.contenedores, &c, sizeof(tContenedor)))
        {
            printf ("[%s]\t", c.id);
            apilar(&aux, &c, sizeof(tContenedor));
            alguno = EXITO;
        }

        if(!alguno) {
            printf ("vacio");
        }

        while (desapilar(&aux, &c, sizeof(tContenedor)))
        {
            apilar(&z.contenedores, &c, sizeof(tContenedor));
        }

        listaModificarElemento(&sis->zonas, &z, sizeof(tZona), cmpZona);
    }
    puts("");
}

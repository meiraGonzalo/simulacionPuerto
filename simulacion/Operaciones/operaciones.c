#include "operaciones.h"

int operaciones (FILE* log, tConfiguracion* config, tEstadoSim* sis, tCola* camionesEspera, tCola* buquesEspera, tOperador* usuario)
{
    char op[TAM_OPERACION], p1[TAM_PARAM], p2[TAM_PARAM];

    puts("\nSeleccione la operacion que desea realizar:");
    while(!seleccionarOperacion(op, p1, p2))
    {
        puts("Intente nuevamente");
    }
    if (strcmpi(op, "DES"))system ("cls");
    if (strcmpi(op, "DES") == 0)
        if (!descargar(log, config, sis, p1, p2))
            return FALLO;
    if (strcmpi(op, "REU") == 0)
        if(!reubicar(log, config, sis, p1, p2, usuario))
            return FALLO;
    if (strcmpi(op, "ENT") == 0)
        if(!entregar(log, config, sis, camionesEspera, usuario))
            return FALLO;
    if (strcmpi(op, "VER") == 0)
        verEstado(sis, camionesEspera, buquesEspera, usuario);
    if (strcmpi(op, "ESP") == 0)
        esperar(log, sis);
    if (strcmpi(op, "OPS") == 0)
        opsDisponibles();
    if (strcmpi(op, "AVZ") == 0)
        avanzar(log, config, sis, p1);
    return EXITO;
}

int seleccionarOperacion(char* op, char* p1, char* p2)
{
    char linea[TAM_BUFFER_OP];
    char opDisponibles[][TAM_OPERACION] = {"DES", "REU", "ENT", "VER", "ESP", "AVZ", "OPS"};
    int cantOp = sizeof(opDisponibles)/TAM_OPERACION;
    int opValida = FALLO;
    int elementos_leidos, j;

    printf("OPERADOR>");
    fgets(linea, sizeof(linea), stdin);

    elementos_leidos = sscanf(linea, "%4s %4s %4s", op, p1, p2);

    if (elementos_leidos < 1) {
        return FALLO;
    }

    for (j = 0; j < cantOp; j++)
    {
        if (strcmpi(op, opDisponibles[j]) == 0)
        {
            opValida = EXITO;
            break;
        }
    }

    if (opValida == FALLO)
    {
        fprintf(stderr, "ERROR: OPERACION NO RECONOCIDA\n");
        return FALLO;
    }

    if (strcmpi(op, "DES") == 0)
    {
        if (elementos_leidos != 3) {
            fprintf(stderr, "ERROR: FALTAN PARAMETROS PARA LA OPERACION DESCARGA\n");
            return FALLO;
        }
        if (toupper(p1[0]) != 'M' || toupper(p2[0]) != 'Z') {
            fprintf(stderr, "ERROR: PARAMETROS INVALIDOS. FORMATO ESPERADO: DES M# Z#\n");
            return FALLO;
        }
    }
    else if (strcmpi(op, "REU") == 0)
    {
        if (elementos_leidos != 3) {
            fprintf(stderr, "ERROR: FALTAN PARAMETROS PARA LA OPERACION REUBICACION\n");
            return FALLO;
        }
        if (toupper(p1[0]) != 'Z' || toupper(p2[0]) != 'Z') {
            fprintf(stderr, "ERROR: PARAMETROS INVALIDOS. FORMATO ESPERADO: DES Z# Z#\n");
            return FALLO;
        }
    }
        else if (strcmpi(op, "AVZ") == 0)
        {
            if (elementos_leidos < 2) {
            fprintf(stderr, "ERROR: FALTAN PARAMETROS PARA LA OPERACION AVANZAR\n");
            return FALLO;
            }
            if (!isdigit(p1[0]))
            {
                fprintf (stderr, "ERROR: PARAMETRO INVALIDO. FORMATO ESPERADO: AVZ <minutos>\n");
                return FALLO;
            }
        }
    return EXITO;
}

void esperar (FILE* log, tEstadoSim* sis)
{
    fprintf(log, "CODIGO:ESP\tTiempo de Inicio:%02d\tTiempo de Fin:%02d\n", sis->tiempo_actual, sis->tiempo_actual+1);
    sis->tiempo_actual += 1;
    puts ("Se avanzo el reloj un minuto");
}

void verEstado (tEstadoSim* sis, tCola* camionesEspera, tCola* buquesEspera, tOperador* usuario)
{
    printf ("TIEMPO ACTUAL DE LA JORNADA: %d\n", sis->tiempo_actual);
    printf ("\nPUNTOS ACUMULADOS: %d\n", usuario->puntacion);
    mostrarMuellesOcupados(sis);
    printf ("\nHay [%d] Buques en espera\n", contarBuquesEspera(buquesEspera));
    mostrarCamionesEspera(camionesEspera);
    mostrarZonas(sis);
    puts("");
}

int descargar(FILE* log, const tConfiguracion* config, tEstadoSim* sis, char* muelle, char* zona)
{
    tMuelle m;
    tZona z;
    tContenedor c;
    tBuque* b;

    if(((config->duracion_jornada - sis->tiempo_actual - config->tiempo_descarga) < 0 ))
    {
        printf ("No hay tiempo suficiente para realizar la operacion de Descarga\n");
        return FALLO;
    }
    if(!listaBuscarElementoCod(&sis->muelles, &m, sizeof(tMuelle),muelle, cmpMuelleCod))
    {
        printf("Muelle Inexistente\n");
        return FALLO;
    }
    if (m.buque_atracado == NULL)
    {
        printf ("El muelle seleccionado se encuentra vacio\n");
        return FALLO;
    }
    if(!listaBuscarElementoCod(&sis->zonas, &z, sizeof(tZona), zona, cmpZonaCod))
    {
        printf("Zona de Almacenamiento Inexistente\n");
        return FALLO;
    }
    if (z.cantElem >= config->capacidad_pila)
    {
        printf ("Zona de Almacenamiento llena\n");
        return FALLO;
    }

    b = m.buque_atracado;

    if(!verPrimero(&b->contenedores, &c, sizeof(tContenedor)))
    {
        fprintf(stderr, "ERROR: El buque no tiene contenedores en cola.\n");
        return FALLO;
    }

    printf ("\nEl contenedor por descargarse es [%s]\n", c.id);

    if(!apilar(&z.contenedores, &c, sizeof(tContenedor)))
    {
        fprintf (stderr, "ERROR: sin memoria para apilar el contenedor\n");
        return FALLO;
    }
    desacolar(&b->contenedores, &c, sizeof(tContenedor));

    z.cantElem++;

    if(!listaModificarElemento(&sis->zonas, &z, sizeof(tZona), cmpZona))
    {
        printf ("ERROR: NO SE PUDO MODIFICAR LA ZONA\n");
        return FALLO;
    }

    sis->tiempo_actual += config->tiempo_descarga;

    puts ("\nContenedor descargado con exito\n");
    fprintf (log, "CODIGO:DES\tTiempo de Inicio:%02d\tTiempo de Fin:%02d\tOrigen:%s\tDestino:%s\n", sis->tiempo_actual-config->tiempo_descarga, sis->tiempo_actual,m.id,z.id);
    return EXITO;
}

int reubicar(FILE* log, const tConfiguracion* config, tEstadoSim* sis, char* origen, char* destino, tOperador* usu)
{
    tContenedor c;
    tZona z1, z2;

    if(strcmpi(origen, destino) == 0)
    {
        printf ("Origen y Destino son la misma zona\n");
        return FALLO;
    }
    if(((config->duracion_jornada - sis->tiempo_actual - config->tiempo_reubicacion) < 0 ))
    {
        printf ("No hay tiempo suficiente para realizar la operacion de Reubicacion\n");
        return FALLO;
    }
    if(!listaBuscarElementoCod(&sis->zonas, &z1, sizeof(tZona), origen, cmpZonaCod) || !listaBuscarElementoCod(&sis->zonas, &z2, sizeof(tZona), destino, cmpZonaCod))
    {
        printf("Zona de Almacenamiento Inexistente\n");
        return FALLO;
    }
    if (z2.cantElem >= config->capacidad_pila)
    {
        printf("La Zona de Almacenamiento de destino se encuentra llena\n");
        return FALLO;
    }
    if(!verTope(&z1.contenedores, &c, sizeof(tContenedor)))
    {
        printf ("La Zona de Almacenamiento de origen se encuentra vacia\n");
        return FALLO;
    }
    if(!apilar(&z2.contenedores, &c, sizeof(tContenedor)))
    {
        fprintf(stderr, "ERROR: Sin memoria para reubicar el contenedor\n");
        return FALLO;
    }
    desapilar(&z1.contenedores, &c, sizeof(tContenedor));
    z1.cantElem--;
    z2.cantElem++;
    if(!listaModificarElemento(&sis->zonas, &z1, sizeof(tZona), cmpZona) || !listaModificarElemento(&sis->zonas, &z2, sizeof(tZona), cmpZona))
    {
        fprintf(stderr, "ERROR: No se pudo actualizar el estado de las zonas en la lista.\n");
        return FALLO;
    }
    sis->tiempo_actual += config->tiempo_reubicacion;
    printf ("Contenedor [%s] Reubicado con exito\n", c.id);
    usu->cantReubicar++;
    fprintf (log, "CODIGO:REU\tTiempo de Inicio:%02d\tTiempo de Fin:%02d\tOrigen:%s\tDestino:%s\n", sis->tiempo_actual-config->tiempo_reubicacion, sis->tiempo_actual,z1.id,z2.id);
    return EXITO;
}

int entregar(FILE* log, const tConfiguracion* config, tEstadoSim* sis, tCola* camionesEspera, tOperador* usuario)
{
    tCamion camion;
    tZona z;
    tContenedor c;
    tIterador it;
    int contenedorEncontrado = FALLO;

    if(((config->duracion_jornada - sis->tiempo_actual - config->tiempo_carga) < 0))
    {
        printf("No hay tiempo suficiente para realizar la operacion de Entrega\n");
        return FALLO;
    }

    if(!verPrimero(camionesEspera, &camion, sizeof(tCamion)))
    {
        printf("No hay camiones en espera\n");
        return FALLO;
    }

    listaIniciarIterador(&it, &sis->zonas);
    while(listaVerSiguiente(&it, &z, sizeof(tZona)))
    {
        if(verTope(&z.contenedores, &c, sizeof(tContenedor)))
        {
            if(strcmpi(c.id, camion.contenedor_solicitado) == 0)
            {
                contenedorEncontrado = EXITO;
                break;
            }
        }
    }

    if(!contenedorEncontrado)
    {
        printf("El contenedor solicitado no se encuentra en el tope de ninguna zona\n");
        return FALLO;
    }

    desapilar(&z.contenedores, &c, sizeof(tContenedor));
    z.cantElem--;

    if(!listaModificarElemento(&sis->zonas, &z, sizeof(tZona), cmpZona))
    {
        fprintf(stderr, "ERROR: NO SE PUDO MODIFICAR LA ZONA\n");
        return FALLO;
    }

    desacolar(camionesEspera, &camion, sizeof(tCamion));

    sis->tiempo_actual += config->tiempo_carga;
    usuario->puntacion += 10;
    usuario->contenEntregados++;

    printf("Contenedor [%s] entregado con exito al camion [%s]\n", c.id, camion.id);
    fprintf (log, "CODIGO:ENT\tTiempo de Inicio:%02d\tTiempo de Fin:%02d\tOrigen:%s\tDestino:%s\n", sis->tiempo_actual-config->tiempo_carga,sis->tiempo_actual, camion.id, c.id);
    return EXITO;
}

void avanzar (FILE* log, tConfiguracion* config, tEstadoSim* sis, char* t)
{
    int tiempo = atoi(t);
    if ((tiempo+sis->tiempo_actual)>config->duracion_jornada)
    {
        printf ("No es posible avanzar la jornada tanto tiempo\n");
        return;
    }
    fprintf(log, "CODIGO:AVZ\tTiempo de Inicio:%02d\tTiempo de Fin:%02d\n", sis->tiempo_actual, sis->tiempo_actual+tiempo);
    sis->tiempo_actual += tiempo;
    printf ("Se avanzo el reloj %d minutos\n", tiempo);
}

void opsDisponibles ()
{
    puts ("OPERACIONES DISPONIBLES:");
    puts ("DES - Descargar y almacenar. Sintaxis: DES <muelle> <zona>");
    puts ("REU - Reubicar. Sintaxis: REU <zona_origen> <zona_destino>");
    puts ("ENT - Entregar contenedor al proximo camion. Sintaxis: ENT");
    puts ("VER - Ver estado. Sintaxis: VER");
    puts ("ESP - Esperar un minuto. Sintaxis: ESP");
    puts ("AVZ - Esperar n minutos. Sintaxis: AVZ <minutos>");
}

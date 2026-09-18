#include "simulacion.h"


void simulacion()
{
    tConfiguracion config;
    tEstadoSim sistema;
    tOperador usuario;
    tCola camionesEspera;
    tCola buquesEspera;
    FILE* log;
    int cantMov;

    system("cls");
    if(!cargarConfiguracion(&config))
    {
        system("pause");
        return;
    }
    if(!iniciarEscenario(&config, &sistema))
    {
        system("pause");
        return;
    }
    system("cls");

    iniciarUsuario(&usuario);
    system ("pause");
    system ("cls");

    log = fopen("log.txt", "w+t");
    if (!log)
    {
        fprintf(stderr, "ERROR AL ABRIR EL ARCHIVO DE REGISTRO\n");
        return;
    }

    crearCola(&camionesEspera);
    crearCola(&buquesEspera);
    usuario.jornadas++;
    puts("BIENVENIDO A OPERACION CONTRARRELOJ");
    puts("Puede ingresar el comando OPS para ver las operaciones disponibles");

    while(sistema.tiempo_actual <= config.duracion_jornada && !bloqueoOperativo(&sistema, &camionesEspera))
    {
        if(!accionesAutomaticas(&sistema,&camionesEspera,&buquesEspera, &usuario))
        {
            fprintf(stderr, "ERROR EN LAS ASIGNACIONES\n");
            return;
        }
        while (!operaciones(log, &config, &sistema, &camionesEspera, &buquesEspera, &usuario))
        {
            printf ("No se realizo la operacion correctamente\n");
        }
        system ("pause");
    }

    system ("cls");
    if (bloqueoOperativo(&sistema, &camionesEspera))
        printf ("Jornada Finalizada exitosamente\n");
    else
        printf ("Duracion de la Jornada Finalizada\n");
    system ("pause");
    system ("cls");

    usuario.camionesPend += contarCamionesEspera(&camionesEspera);
    usuario.puntacion -= usuario.camionesPend*2;
    mostrarResumenUser(&config, &sistema, &usuario);
    actualizarRegUsuario(&usuario);
    system ("pause");
    system ("cls");

    cantMov = mostrarResumenMov(log);
    archivarJornada(log, &usuario, cantMov);
    fclose(log);
    remove("log.txt");
    system ("pause");
}

int bloqueoOperativo (tEstadoSim* sis, tCola* camionesEspera)
{
    int bloqueo = EXITO;

    if (!colaVacia(&sis->buques_programados))
    {
        bloqueo = FALLO;
    }
    if(!colaVacia(&sis->camiones_programados))
    {
        bloqueo = FALLO;
    }
    if(!colaVacia(camionesEspera))
    {
        bloqueo = FALLO;
    }
    if (!todosMuellesVacios(sis))
    {
        bloqueo = FALLO;
    }
    return bloqueo;
}

int mostrarResumenMov (FILE* log)
{
    int i=0;
    char linea[150];
    fseek(log, 0, SEEK_SET);
    puts("RESUMEN DE MOVIMIENTOS");
    puts("");
    while(fgets(linea, 150, log))
    {
        puts(linea);
        i++;
    }
    return i;
}

void mostrarResumenUser (const tConfiguracion* config, const tEstadoSim* sis, const tOperador* usuario)
{
    puts ("RESUMEN DE LA JORNADA");
    puts("");
    printf ("OPERADOR: %s\n", usuario->nombre);
    printf ("BUQUES DESCARGADOS: %d\n", usuario->buquesDescargados);
    printf ("CONTENEDORES ENTREGADOS: %d\n", usuario->contenEntregados);
    printf ("CAMIONES PENDIENTES: %d\n", usuario->camionesPend);
    printf ("OPERACIONES DE REUBICACION REALIZADAS: %d\n", usuario->cantReubicar);
    puts("");
    printf ("TIEMPO UTILIZADO: %d de %d\n", sis->tiempo_actual, config->duracion_jornada);
    puts("");
    printf ("PUNTACION FINAL: %d\n", usuario->puntacion);
}

int archivarJornada (FILE* log, tOperador* usuario, int cantMov)
{
    char linea[150];
    tJornada jornada;
    tMovimiento mov;
    FILE* jor;

    jor = fopen("jornadas.dat", "a+b");
    if (!jor)
    {
        fprintf(stderr, "ERROR AL ABRIR ARCHIVO DE JORNADAS\n");
        return FALLO;
    }

    fseek(log, 0, SEEK_SET);
    strcpy(jornada.nombreOp, usuario->nombre);
    jornada.cantidad_movimientos = cantMov;
    jornada.puntuacion = usuario->puntacion;
    fwrite(&jornada, sizeof(tJornada), 1, jor);

    while(fgets(linea, 150, log))
    {
        strcpy(mov.operacion, linea);
        fwrite(&mov, sizeof(tMovimiento), 1, jor);
    }

    fclose(jor);
    return EXITO;
}

#include "automatico.h"

int accionesAutomaticas (tEstadoSim* sis, tCola* camionesEspera, tCola* buquesEspera, tOperador* usuario)
{
    desatracarVacios (sis, usuario);
    if (!asignarCamiones(sis, camionesEspera))
    {
        fprintf(stderr, "ERROR: NO SE PUDIERON ASIGNAR LOS CAMIONES\n");
        return FALLO;
    }
    if (!asignarBuques(sis, buquesEspera))
    {
        fprintf(stderr, "ERROR: NO SE PUDIERON ASIGNAR LOS BUQUES\n");
        return FALLO;
    }
    return EXITO;
}

int asignarBuques(tEstadoSim* sis, tCola* buquesEspera)
{
    tBuque buque;
    tMuelle muelle;
    tIterador it;
    listaIniciarIterador(&it, &sis->muelles);

    //ya arribados en espera
    while(!colaVacia(buquesEspera) && !todosMuellesLlenos(sis))
    {
        desacolar(buquesEspera, &buque, sizeof(tBuque));
        do{
            listaVerSiguiente(&it, &muelle, sizeof(tMuelle));
        }while (muelle.buque_atracado != NULL);

        if(((muelle.buque_atracado = (tBuque*)malloc(sizeof(tBuque))) == NULL))
        {
            fprintf(stderr, "ERROR: SIN MEMORIA PARA ATRACAR BUQUE\n");
            return FALLO;
        }
        memcpy(muelle.buque_atracado, &buque, sizeof(tBuque));
        listaModificarElemento(&sis->muelles, &muelle, sizeof(tMuelle), cmpMuelle);
    }

    //nuevos arribos
    while(!colaVacia(&sis->buques_programados))
    {
        verPrimero(&sis->buques_programados, &buque, sizeof(tBuque));
        if (buque.minuto_llegada <= sis->tiempo_actual)
        {
            if(todosMuellesLlenos(sis))
            {
                desacolar(&sis->buques_programados, &buque, sizeof(tBuque));
                acolar(buquesEspera, &buque, sizeof(tBuque));
            }
            else
            {
                desacolar(&sis->buques_programados, &buque, sizeof(tBuque));
                do{
                listaVerSiguienteCircular(&it,&sis->muelles, &muelle, sizeof(tMuelle));
                }while(muelle.buque_atracado != NULL);

                if(((muelle.buque_atracado = (tBuque*)malloc(sizeof(tBuque))) == NULL))
                {
                    fprintf(stderr, "ERROR: SIN MEMORIA PARA ATRACAR BUQUE\n");
                    return FALLO;
                }
                memcpy(muelle.buque_atracado, &buque, sizeof(tBuque));
                listaModificarElemento(&sis->muelles, &muelle, sizeof(tMuelle), cmpMuelle);
            }
        }
        else
            break;
    }

    return EXITO;
}


int asignarCamiones(tEstadoSim* sis, tCola* camionesEspera)
{
    tCamion camion;

    while(!colaVacia(&sis->camiones_programados))
    {
        verPrimero(&sis->camiones_programados, &camion, sizeof(tCamion));
        if (camion.minuto_llegada <= sis->tiempo_actual)
        {
            if(!acolar(camionesEspera, &camion, sizeof(tCamion)))
            {
                fprintf(stderr, "ERROR: SIN MEMORIA PARA COLA DE ESPERA DE CAMIONES\n");
                return FALLO;
            }
            desacolar(&sis->camiones_programados, &camion, sizeof(tCamion));
        }
        else
            break;
    }
    return EXITO;
}

void desatracarVacios (tEstadoSim* sis, tOperador* usuario)
{
    tIterador it;
    tMuelle m;
    tBuque* b;
    listaIniciarIterador(&it, &sis->muelles);
    while (listaVerSiguiente(&it, &m, sizeof(tMuelle)))
    {
        if(m.buque_atracado != NULL)
        {
            b = m.buque_atracado;
            if(colaVacia(&b->contenedores))
            {
                free(b);
                m.buque_atracado = NULL;
                listaModificarElemento(&sis->muelles, &m, sizeof(tMuelle), cmpMuelle);
                usuario->buquesDescargados++;
                usuario->puntacion += 5;
            }
        }
    }
}

Simulación interactiva desarrollada en C donde el usuario asume el rol de Operador del Puerto.   
El objetivo es gestionar la descarga de buques, el almacenamiento temporal en zonas de capacidad limitada y la entrega de contenedores a camiones, intentando completar la mayor cantidad de operaciones antes de que finalice la jornada.

El sistema utiliza el archivo config.txt para establecer los parámetros de la simulación, como la duración de la jornada, la cantidad de muelles y zonas, y los tiempos que consume cada operación.

La simulación genera de manera aleatoria un archivo puerto.txt que contiene toda la información de la jornada que se está simulando, incluyendo los tiempos de arribos programados de los buques y camiones, junto con sus contenidos.

Al finalizar, se muestran por pantalla un resumen de los movimientos de la jornada junto con las estadísticas alcanzadas por el operador, tales como la puntuación obtenida.

El sistema registra los datos históricos de cada usuario en un archivo operadores.dat, permitiendo visualizar la puntuación acumulada a lo largo de varias jornadas. El acceso al archivo de operadores se realiza mediante un archivo indices.idx que contiene el nombre de cada usuario dado de alta.

Es posible visualizar un ranking de los operadores ordenado por puntajes.

El sistema registra cada jornada en un archivo jornadas.dat, pudiendo visualizarse todas las simulaciones completadas por un mismo usuario, junto con su información detallada.

El usuario dispone de los siguientes comandos durante la simulación:

DES (Descargar): Retira un contenedor del buque seleccionado y lo apila en una zona con capacidad disponible.  
REU (Reubicar): Traslada el contenedor ubicado en el tope de una zona hacia el tope de otra zona distinta.  
ENT (Entregar): Atiende al camión al frente de la cola y le entrega su contenedor solicitado si este se encuentra en el tope de alguna zona.  
VER (Ver estado): Consulta que muestra el tiempo actual, los buques atracados, el contenido de las zonas, la ventana de planificación de camiones y la puntuación.  
ESP (Esperar): Avanza el reloj exactamente un minuto sin realizar operaciones de movimiento.   

Se desarrollaron adicionalmente dos comandos extras:  

OPS (Operaciones Disponibles): Muestra al usuario todas las operaciones disponibles durante la ejecución.  
AVZ (Avanzar): Avanza el reloj una cantidad de minutos determinadas por el usuario.  

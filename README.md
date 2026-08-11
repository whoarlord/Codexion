*This project has been created as part of the 42 curriculum by iarrien-.*

# Codexion

## Descripción

**Codexion** es un proyecto del currículo de **42** centrado en la programación concurrente en C mediante **POSIX threads (`pthread`)**.

El objetivo es simular un entorno en el que varios **coders** trabajan de forma concurrente y comparten un conjunto limitado de **dongles**. Cada coder sigue un ciclo de trabajo compuesto por:

1. Solicitar recursos al scheduler.
2. Obtener los dos dongles necesarios.
3. Compilar.
4. Liberar los dongles.
5. Depurar.
6. Refactorizar.
7. Volver a solicitar recursos.

La dificultad principal del proyecto no está solamente en crear threads, sino en coordinar correctamente el acceso concurrente a recursos compartidos, evitando deadlocks, carreras de datos, starvation y estados inconsistentes.

La implementación utiliza:

* `pthread_t` para representar los threads.
* `pthread_mutex_t` para proteger recursos compartidos.
* `pthread_cond_t` para coordinar el scheduler y los coders.
* Un heap como cola de prioridad.
* Dos estrategias de planificación:

  * **FIFO** (*First In, First Out*).
  * **EDF** (*Earliest Deadline First*).
* Un monitor encargado de detectar finalización y burnout.
* Un mutex específico para serializar la salida por pantalla.

## Objetivos

Los principales objetivos técnicos del proyecto son:

* Comprender la ejecución concurrente mediante threads.
* Sincronizar correctamente recursos compartidos.
* Evitar deadlocks.
* Coordinar productores y consumidores mediante una condición.
* Implementar planificación de tareas.
* Gestionar correctamente el ciclo de vida de los threads.
* Detectar el burnout de los coders.
* Mantener una salida por pantalla consistente.
* Gestionar correctamente la liberación de recursos y mutexes.

## Estructura del proyecto

```text
.
├── Makefile
├── tests.txt
└── src
    ├── coders
    │   ├── coders_actions.c
    │   ├── coders_loop.c
    │   ├── coders_utils.c
    |   └── check_simulation.c
    ├── init
    │   ├── ft_free.c
    │   └── ft_init.c
    ├── scheduler
    │   ├── heap_utils.c
    │   ├── heapify_utils.c
    │   └── scheduler.c
    ├── coders.h
    └── main.c
```

### `src/main.c`

Se encarga de:

* Procesar los argumentos.
* Inicializar la configuración global.
* Crear coders y dongles.
* Crear el scheduler.
* Lanzar la simulación.

### `src/coders/`

Contiene el comportamiento de los coders:

* Solicitud de recursos.
* Compilación.
* Debug.
* Refactorización.
* Cooldown.
* Detección del estado global de finalización.

### `src/scheduler/`

Contiene el sistema de planificación:

* Heap de requests.
* Inserción y extracción.
* `heapify_up`.
* `heapify_down`.
* Ordenación FIFO.
* Ordenación EDF.
* Asignación de dongles.
* Comunicación mediante `pthread_cond_t`.

### `src/init/`

Gestiona:

* Inicialización de estructuras.
* Inicialización de mutexes.
* Inicialización del heap.
* Liberación de memoria.
* Destrucción de primitivas de sincronización.

## Compilación

El proyecto utiliza `cc` y requiere soporte para POSIX threads.

El `Makefile` compila utilizando:

```text
-Wall -Wextra -Werror -pthread
```

Para compilar:

```bash
make
```

Esto genera el ejecutable:

```text
./codexion
```

Para limpiar los archivos objeto:

```bash
make clean
```

Para eliminar también el ejecutable:

```bash
make fclean
```

Para recompilar desde cero:

```bash
make re
```

## Ejecución

El programa recibe ocho argumentos:

```text
./codexion \
    number_of_coders \
    time_to_burnout \
    time_to_compile \
    time_to_debug \
    time_to_refactor \
    number_of_compiles_required \
    dongle_cooldown \
    scheduler
```

Por ejemplo:

```bash
./codexion 20 1000 100 100 100 10 0 edf
```

El último argumento determina el algoritmo de planificación:

```text
fifo
edf
```

### FIFO

Los requests se procesan en orden de llegada.

```text
Coder A → Coder B → Coder C → ...
```

### EDF

Los requests se ordenan según el tiempo de burnout previsto.

El request con el deadline más próximo obtiene mayor prioridad.

En caso de empate, se utiliza el número del coder como criterio secundario.

## Ejemplo de configuración

El repositorio contiene un archivo `tests.txt` que puede utilizarse con el target `run` del Makefile:

```text
20
1000
100
100
100
10
0
edf
```

El Makefile permite ejecutar:

```bash
make run FILE=tests.txt
```

Esto construye el proyecto y ejecuta:

```text
./codexion 20 1000 100 100 100 10 0 edf
```

## Flujo de ejecución

Cada coder ejecuta un ciclo similar a:

```text
┌──────────────────┐
│ Solicitar trabajo│
└────────┬─────────┘
         ↓
┌──────────────────┐
│ Esperar scheduler│
└────────┬─────────┘
         ↓
┌──────────────────┐
│ Obtener dongles  │
└────────┬─────────┘
         ↓
┌──────────────────┐
│    Compilar      │
└────────┬─────────┘
         ↓
┌──────────────────┐
│ Liberar dongles  │
└────────┬─────────┘
         ↓
┌──────────────────┐
│     Debug        │
└────────┬─────────┘
         ↓
┌──────────────────┐
│   Refactorizar   │
└────────┬─────────┘
         │
         └──────────► volver a solicitar
```

El monitor ejecuta simultáneamente las comprobaciones de finalización y burnout.

## Blocking cases handled

### Deadlock y condiciones de Coffman

El diseño evita que los coders compitan directamente por sus dos dongles.

En lugar de que cada coder haga:

```text
lock(dongle A)
lock(dongle B)
```

por su cuenta, el coder primero envía una petición al scheduler.

El scheduler:

1. Selecciona una petición.
2. Comprueba la disponibilidad de ambos dongles.
3. Reserva ambos recursos.
4. Despierta al coder correspondiente.

De esta manera, el coder no mantiene un dongle mientras espera indefinidamente el segundo.

Este diseño rompe principalmente la condición de **hold and wait** de Coffman y reduce el riesgo del clásico ciclo:

```text
Coder A → dongle 1 → espera dongle 2
Coder B → dongle 2 → espera dongle 1
```

Los mutex de los dongles también se adquieren siguiendo una estrategia consistente dentro del scheduler.

### Starvation

El scheduler dispone de dos políticas:

* FIFO, que conserva el orden de llegada.
* EDF, que prioriza el request con el burnout más cercano.

FIFO proporciona una política naturalmente justa basada en el orden de llegada.

EDF prioriza deadlines y puede favorecer repetidamente a tareas con deadlines más próximos. Por ello, la ausencia completa de starvation no debe considerarse garantizada por EDF en todas las cargas de trabajo.

### Cooldown

Después de utilizar un dongle, este necesita respetar un periodo de cooldown antes de volver a utilizarse.

El estado de utilización del dongle se registra mediante `last_use`.

Antes de comenzar una nueva compilación se comprueba que haya transcurrido el cooldown requerido.

El cooldown forma parte de la coordinación entre los coders y los recursos compartidos y evita reutilizar un dongle antes de tiempo.

### Burnout

El monitor comprueba continuamente el tiempo transcurrido desde la última compilación de cada coder.

Conceptualmente:

```text
current_time >= last_compile + time_to_burnout
```

Cuando se detecta un burnout:

1. Se actualiza el estado global.
2. Se serializa el mensaje de salida.
3. Se despiertan los threads que puedan estar esperando.
4. Los threads terminan progresivamente su ejecución.

### Finalización global

La simulación termina cuando todos los coders alcanzan el número requerido de compilaciones.

El monitor detecta esta condición y establece el estado global de finalización.

Los threads bloqueados en el scheduler son posteriormente despertados mediante `pthread_cond_broadcast()` para permitir una terminación limpia.

### Serialización de logs

Todos los mensajes importantes que escriben los coders utilizan `print_mutex`.

Esto evita que dos threads escriban simultáneamente en `stdout` y produzcan líneas mezcladas.

El objetivo es mantener cada evento como una operación de salida atómica desde el punto de vista lógico:

```text
timestamp coder evento
```

## Thread synchronization mechanisms

### `pthread_mutex_t`

El proyecto utiliza varios mutex con responsabilidades diferentes.

#### Mutex de los dongles

Cada dongle posee su propio:

```c
pthread_mutex_t mutex;
```

Este mutex protege el acceso concurrente al estado asociado al dongle.

El objetivo es evitar que dos coders modifiquen simultáneamente el mismo recurso.

#### Mutex del heap

El scheduler utiliza:

```c
pthread_mutex_t mutex;
```

en la estructura del heap.

Este mutex protege:

* La cola de requests.
* El tamaño del heap.
* Inserciones.
* Extracciones.
* Reorganizaciones del heap.
* La coordinación con los coders que esperan una asignación.

#### Mutex de impresión

`print_mutex` serializa el acceso a `stdout`.

Sin este mutex podrían producirse salidas intercaladas entre threads.

#### Mutex de estado global

`dead_mutex` protege:

```c
flags->is_dead
```

Las funciones:

```c
check_dead()
update_dead()
```

utilizan este mutex para evitar accesos concurrentes inconsistentes.

### `pthread_cond_t`

El scheduler dispone de una:

```c
pthread_cond_t cond;
```

Los coders se bloquean cuando han enviado una petición y todavía no tienen los recursos asignados:

```c
pthread_cond_wait(
    &coder->flags->heap->cond,
    &coder->flags->heap->mutex
);
```

El scheduler procesa las peticiones y posteriormente utiliza:

```c
pthread_cond_broadcast(&flags->heap->cond);
```

para despertar a los threads que están esperando.

La condición no constituye por sí misma el estado de sincronización: el estado real está representado por variables como `go_out` y `is_dead`, que se vuelven a comprobar después de cada wake-up.

Esto es importante porque un thread no debe asumir que despertarse implica automáticamente que el recurso está disponible.

### Implementación de eventos

El proyecto utiliza una implementación de evento basada en:

```text
pthread_cond_t
+
pthread_mutex_t
+
estado compartido
```

En particular, `go_out` actúa como predicado asociado a la espera del coder.

El flujo es:

```text
Coder
  │
  ├── bloquea heap mutex
  │
  ├── inserta request
  │
  ├── go_out = 0
  │
  └── espera en pthread_cond_wait()
             │
             ▼
        Scheduler
             │
             ├── procesa heap
             ├── reserva recursos
             ├── go_out = 1
             └── broadcast
                    │
                    ▼
                  Coder
                    │
                    └── continúa
```

La modificación del estado y la espera se realizan bajo el mismo mutex del heap, evitando que una notificación se pierda entre la comprobación del estado y la llamada a `pthread_cond_wait()`.

### Prevención de race conditions

Un ejemplo correcto de sincronización es el estado global de finalización:

```c
pthread_mutex_lock(&flags->dead_mutex);
flags->is_dead = 1;
pthread_mutex_unlock(&flags->dead_mutex);
```

y posteriormente:

```c
pthread_mutex_lock(&flags->dead_mutex);
result = flags->is_dead;
pthread_mutex_unlock(&flags->dead_mutex);
```

De esta forma, los threads no leen y escriben `is_dead` simultáneamente sin coordinación.

El mismo principio debe aplicarse a cualquier otro estado compartido, especialmente a:

* `is_busy`
* `last_use`
* `last_compile`
* `compile_count`
* `go_out`

## Scheduler y heap

El scheduler utiliza un **binary heap** para seleccionar el siguiente request.

### Inserción

Los requests se añaden mediante `heap_push()`.

El nuevo elemento se coloca al final y después se ejecuta:

```text
heapify_up()
```

para recuperar la propiedad del heap.

### Extracción

`heap_pop()` obtiene el request situado en la raíz y reorganiza el heap mediante:

```text
heapify_down()
```

### FIFO

La comparación se basa en:

```text
arrival
```

por lo que el request más antiguo obtiene prioridad.

### EDF

La comparación se basa en:

```text
time_to_burnout
```

El coder con el deadline más cercano obtiene prioridad.

En caso de empate:

```text
coder->number
```

actúa como desempate determinista.

## Gestión de memoria

La memoria dinámica se libera al finalizar la simulación.

La implementación separa:

* coders,
* dongles,
* heap,
* estructuras auxiliares,
* flags globales.

Antes de liberar las estructuras se destruyen los mutexes y variables de condición que hayan sido inicializados.

Los campos:

```c
mutex_ready
cond_ready
```

permiten saber qué primitivas llegaron a inicializarse correctamente y evitar destruir primitivas que nunca fueron creadas.

## Recursos

### POSIX Threads

Documentación y referencia para la API de threads utilizada por el proyecto:

* `pthread_create`
* `pthread_join`
* `pthread_mutex_t`
* `pthread_cond_t`
* `pthread_cond_wait`
* `pthread_cond_broadcast`

Referencia POSIX:

https://pubs.opengroup.org/onlinepubs/9699919799/

### Condition variables

La documentación de POSIX explica el uso de las variables de condición para bloquear threads hasta que se produzca un cambio en un estado compartido.

También resulta útil la documentación de referencia de `pthread_cond_broadcast()`:

https://man7.org/linux/man-pages/man3/pthread_cond_broadcast.3p.html

### Deadlocks y condiciones de Coffman

Para comprender la prevención de deadlocks resulta fundamental estudiar las cuatro condiciones necesarias:

1. Mutual exclusion.
2. Hold and wait.
3. No preemption.
4. Circular wait.

La estrategia del proyecto intenta romper principalmente la condición de **hold and wait** mediante la asignación conjunta de los dos dongles por parte del scheduler.

### Scheduling

El proyecto utiliza dos estrategias clásicas:

* FIFO.
* EDF (*Earliest Deadline First*).

EDF es especialmente relevante porque cada coder puede interpretarse como una tarea con un deadline relacionado con su burnout.

### Uso de IA

La inteligencia artificial se utilizó como herramienta de apoyo durante el desarrollo y revisión del proyecto, no como sustituto de la comprensión de la implementación.

En concreto, se utilizó para:

* Analizar estructuras de concurrencia.
* Revisar posibles deadlocks y race conditions.
* Revisar el uso de `pthread_mutex_t` y `pthread_cond_t`.
* Analizar la arquitectura del scheduler.
* Identificar posibles problemas de sincronización.
* Sugerir mejoras de organización y legibilidad.
* Ayudar a estructurar y redactar esta documentación.

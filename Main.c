#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <time.h>

#define MAX_CICLOS 3
int  pregunta;
char respuesta[50];
int refrescarTablero=0;
int aciertos = 0;
int cambiosdeVariables=1;
int palabrasBool=0;
char *respuestas[] = {"rosa", "gato", "gorila", "luna", "sol", "oreja"};
char *respuestas2[] = {"islam", "bahia", "modelo", "lana", "box", "oliva"};
char *respuestas3[] = {"aduana", "mano", "lobulo", "limar", "col", "sombra"};
char *adivinanzas[] = {
        "1: Color de la pasión y también de una flor.",
        "2: Animal con bigotes y muy buen cazador de ratones.",
        "3: Gran primate que comparte muchos genes con los humanos.",
        "4: Astro que ilumina nuestras noches.",
        "5: Astro que nos da luz y calor durante el día.",
        "6: Parte del cuerpo que usamos para oír."
};

char *adivinanzas2[] = {
        "1: Religión monoteísta que se originó en la península arábiga.",
        "2: Entrada o ensenada pequeña en la costa, generalmente tranquila y de aguas profundas.",
        "3: Persona que muestra prendas de vestir en pasarelas o fotografías.",
        "4: Fibra natural suave obtenida del vellón de las ovejas, usada en la fabricación de tejidos y prendas.",
        "5: Espacio cerrado y seguro para entrenamiento o competiciones de combate.",
        "6: Fruto verde pequeño, cultivado en el Mediterráneo, famoso por su aceite."
};


char *adivinanzas3[] = {
        "1: Instalación gubernamental en fronteras donde se controlan las mercancías que entran y salen de un país.",
        "2: Parte del cuerpo al final del brazo, usada para agarrar y manipular objetos.",
        "3: Parte inferior y blanda de la oreja, a menudo perforada para aretes.",
        "4: Acción de alisar o dar forma a un material usando una herramienta de corte.",
        "5: Lechuga pero dura",
        "6: Lo que se genera al tapar la luz"
};



int (*coordenadasActual)[6][2];




int coordenadas1[6][6][2] = {
        {{0, 1}, {1, 1}, {2, 1}, {3, 1}, {-1, -1}, {-1, -1}}, // rosa
        {{3, 0}, {3, 1}, {3, 2}, {3, 3}, {-1, -1}, {-1, -1}}, // gato
        {{2, 3}, {3, 3}, {4, 3}, {5, 3}, {6, 3}, {7, 3}},     // gorila
        {{6, 3}, {6, 4}, {6, 5}, {6, 6}, {-1, -1}, {-1, -1}}, // luna
        {{2, 5}, {2, 6}, {2, 7}, {-1, -1}, {-1, -1}, {-1, -1}}, // sol
        {{2, 6}, {3, 6}, {4, 6}, {5, 6}, {6, 6}, {-1, -1}}   // oreja
};
int coordenadas2[6][6][2] = {
        {{0, 1}, {1, 1}, {2, 1}, {3, 1}, {4, 1}, {5, 1} }, // islam
        {{3, 0}, {3, 1}, {3, 2}, {3, 3}, {3, 4}, {-1, -1}}, // bahia
        {{2, 3}, {3, 3}, {4, 3}, {5, 3}, {6, 3}, {7, 3}},     // modelo
        {{6, 3}, {6, 4}, {6, 5}, {6, 6}, {-1, -1}, {-1, -1}}, // lana
        {{2, 5}, {2, 6}, {2, 7}, {-1, -1}, {-1, -1}, {-1, -1}}, // box
        {{2, 6}, {3, 6}, {4, 6}, {5, 6}, {6, 6}, {-1, -1}} // oliva
};
int coordenadas3[6][6][2] = {
        {{0, 1}, {1, 1}, {2, 1}, {3, 1}, {4, 1}, {5, 1}}, // aduana
        {{3, 0}, {3, 1}, {3, 2}, {3, 3}, {-1, -1}, {-1, -1}}, // mano
        {{2, 3}, {3, 3}, {4, 3}, {5, 3}, {6, 3}, {7, 3}}, // lobulo
        {{6, 3}, {6, 4}, {6, 5}, {6, 6}, {6, 7}, {-1, -1}}, // limar
        {{2, 5}, {2, 6}, {2, 7}, {-1, -1}, {-1, -1}, {-1, -1}}, // col
        {{1, 6}, {2, 6}, {3, 6}, {4, 6}, {5, 6}, {6, 6}} // bordar
};

int *longitudActual;

int longitud1[] = {4, 4, 6, 4, 3, 5};
int longitud2[] = {5, 5, 6, 4, 3, 5};
int longitud3[] = {6, 4, 6, 5, 3, 6};
int respuestaDada[6] = {0};
char tablero[8][8];
pthread_mutex_t lock;

void clear_screen() {
    //claer mi terminal
    printf("\033[H\033[J");
}

void* print_thread(void *arg) {
    while (aciertos < 6) {
        pthread_mutex_lock(&lock);
        clear_screen();

        if (palabrasBool==1){
            printf("Tardaste mucho, cambiaron las palabras!\n");
            palabrasBool=0;
            refrescarTablero=1;

        }



        if (refrescarTablero == 1) {
            for (int i = 0; i < 8; i++) {
                for (int j = 0; j < 8; j++) {
                    int esParteDePalabra = 0;
                    int numeroDePregunta = 0;


                    for (int p = 0; p < 6; p++) {
                        for (int k = 0; k < longitudActual[p]; k++) {
                            if (coordenadasActual[p][k][0] == -1) break;
                            if (i == coordenadasActual[p][k][0] && j == coordenadasActual[p][k][1]) {
                                if (respuestaDada[p]) {
                                    printf("%2c ", tablero[i][j]);
                                    esParteDePalabra = 1;
                                    break;
                                } else {
                                    numeroDePregunta = p + 1;
                                }
                            }
                        }
                        if (esParteDePalabra) break;
                    }

                    // verificar si no hay letra
                    if (!esParteDePalabra && numeroDePregunta != 0) {
                        printf("%2d ", numeroDePregunta);
                    } else if (!esParteDePalabra) {
                        printf("%2c ", tablero[i][j]);
                    }
                }
                printf("\n");
            }

            for (int i = 0; i < 6; i++) {
                printf("%s\n", adivinanzas[i]);
            }

            refrescarTablero = 0;
        }



        printf("Ingrese el número de la pregunta:\n");
        scanf("%d", &pregunta);

        if (pregunta < 1 || pregunta > 6 || respuestaDada[pregunta - 1]) {
            printf("Número inválidoe.\n");
            refrescarTablero=1;

        }else{

            printf("Ingrese su respuesta (sin acentos y en minúsculas):\n");
            scanf("%s", respuesta);

            if (strcmp(respuesta, respuestas[pregunta - 1]) == 0) {
                printf("¡Correcto!\n");
                respuestaDada[pregunta - 1] = 1;
                aciertos++;
                //acrtualizar con respuesta respondida
                for (int i = 0; i < longitudActual[pregunta - 1]; i++) {
                    int x = coordenadasActual[pregunta - 1][i][0];
                    int y = coordenadasActual[pregunta - 1][i][1];
                    tablero[x][y] = respuestas[pregunta - 1][i];
                }

                refrescarTablero=1;
            } else {

                 printf("Incorrecto, intenta de nuevo.\n");
                 refrescarTablero=1;
                 sleep(1);


            }

        }


        pthread_mutex_unlock(&lock);
        sleep(1);  // refresh cada 1 unidades
    }
    return NULL;
}

void handle_update(int sig) {

    if (cambiosdeVariables < MAX_CICLOS) {

        if (cambiosdeVariables == 1) {
            coordenadasActual = coordenadas2;
            longitudActual = longitud2;
        } else if (cambiosdeVariables == 2) {
            coordenadasActual = coordenadas3;
            longitudActual = longitud3;
        }
        for (int i = 0; i < 6; i++) {
            if (!respuestaDada[i]) {
                respuestas[i] = (cambiosdeVariables == 1) ? respuestas2[i] : respuestas3[i];
                adivinanzas[i] = (cambiosdeVariables == 1) ? adivinanzas2[i] : adivinanzas3[i];
            }
        }
        palabrasBool = 1;
        cambiosdeVariables++;
    } else {

        printf("Se te acabó el tiempo, has perdido.\n");
        exit(0);
    }
}

void handle_termination(int sig) {
    printf("Se te acabó el tiempo, has perdido.\n");
    exit(0);  // termina el programa
}


int main() {
    memset(tablero, '.', sizeof(tablero));

    pthread_t tid;
    pthread_mutex_init(&lock, NULL);
    pthread_create(&tid, NULL, print_thread, NULL);

    int changecounter=0;
    int status;

    coordenadasActual = coordenadas1;
    longitudActual = longitud1;

    struct sigaction sa;
    sa.sa_handler = handle_update;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGUSR1, &sa, NULL);

    pid_t pid = fork();
    if (pid == 0) {
        while (1) {
            sleep(25);
            if (++changecounter > MAX_CICLOS) {
                kill(getppid(), SIGUSR2);
                return 0;
            } else {
                kill(getppid(), SIGUSR1);
            }
        }
    } else if (pid > 0) { // main



        refrescarTablero=1;


        signal(SIGUSR2, handle_termination);
        pthread_join(tid, NULL);
        pthread_mutex_destroy(&lock);

        waitpid(pid, &status, 0);

        //refrescar tablero por ultima vez
        if (refrescarTablero == 1) {
            for (int i = 0; i < 8; i++) {
                for (int j = 0; j < 8; j++) {
                    int esParteDePalabra = 0;
                    for (int p = 0; p < 6; p++) {
                        for (int k = 0; k < longitudActual[p]; k++) {
                            if (coordenadasActual[p][k][0] == -1) break;
                            if (i == coordenadasActual[p][k][0] && j == coordenadasActual[p][k][1]) {
                                if (!respuestaDada[p]) {
                                    printf("%2d ", p + 1);
                                } else {
                                    printf("%2c ", tablero[i][j]);
                                }
                                esParteDePalabra = 1;
                                break;
                            }
                        }
                        if (esParteDePalabra) break;
                    }
                    if (!esParteDePalabra) {
                        printf("%2c ", tablero[i][j]);
                    }
                }
                printf("\n");
            }

            for (int i = 0; i < 6; i++) {
                printf("%s\n", adivinanzas[i]);
            }

            refrescarTablero = 0;
        }





        printf("¡Felicidades! Has completado el crucigrama.\n");
        kill(pid, SIGKILL); // terminar proceso hijo
    } else {
        perror("Fork failed");
        return 1;
    }



    return 0;
}

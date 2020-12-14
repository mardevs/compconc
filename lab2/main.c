/*
  Laboratorio 2 - Computacao Concorrente
  Nome: Domenica Maria Cioci Ferreira Goncalves
  Prof: Silvana Rossetto
  Periodo: 2020.1 REMOTO
  Implementacao e avaliacao de aplicacoes concorrentes (parte 1)
*/

#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "timer.h"

#define GET_MAT_VAL(matriz, i, j, dimen) (matriz[i * dimen + j])

typedef struct
{
    int idThread;
    int nThreads;
    float* matriz_a;
    float* matriz_b;
    float* matriz_c;
    int dimensao;
} t_Args;

void *multiplica(void *arg) {
    t_Args *args = (t_Args *)arg;

    for(int i = args->idThread; i < args->dimensao; i += args->nThreads) {
        for(int j = 0; j < args->dimensao; j++) {
            GET_MAT_VAL(args->matriz_c, i, j, args->dimensao) = 0.0f;

            for(int k = 0; k < args->dimensao; k++) {
                GET_MAT_VAL(args->matriz_c, i, j, args->dimensao) += GET_MAT_VAL(args->matriz_a, i, k, args->dimensao) * GET_MAT_VAL(args->matriz_b, k, j, args->dimensao);
            }
        }
    }

    pthread_exit(NULL);
}

int main(int argc, char** argv) {
    double t0, t1;
    double inicializacao, multiplicacao, resposta;

    srand(time(NULL));

    if(argc != 3) {
        printf("Uso: %s <N_THREADS> <DIM>\n", argv[0]);
        exit(1);
    }

    int n_threads = atoi(argv[1]);
    int dimensao = atoi(argv[2]);

    pthread_t* tids = (pthread_t*) malloc(n_threads * sizeof(pthread_t));
    t_Args *args = malloc(n_threads * sizeof(t_Args)); //receberá os argumentos para a thread

    float* matriz_a = (float*) malloc(dimensao * dimensao * sizeof(float));
    float* matriz_c = (float*) malloc(dimensao * dimensao * sizeof(float));
    float* matriz_b = (float*) malloc(dimensao * dimensao * sizeof(float));    

    GET_TIME(t0);
    for(int i = 0; i < dimensao; i++) {
        for(int j = 0; j < dimensao; j++) {
            GET_MAT_VAL(matriz_a, i, j, dimensao) = (double) rand() / RAND_MAX;
            GET_MAT_VAL(matriz_b, i, j, dimensao) = (double) rand() / RAND_MAX;
        }
    }
    GET_TIME(t1);
    inicializacao = t1 - t0;

    GET_TIME(t0);
    for (int i = 0; i < n_threads; i++) {
        args[i].idThread = i; 
        args[i].nThreads = n_threads; 
        args[i].matriz_a = matriz_a;
        args[i].matriz_b = matriz_b;
        args[i].matriz_c = matriz_c;
        args[i].dimensao = dimensao;

        if (pthread_create(&tids[i], NULL, multiplica, (void*) &args[i])) {
          printf("--ERRO: pthread_create()\n");
          exit(3);
        }
    }

    // espera todas as threads terminarem
    for (int i = 0; i < n_threads; i++) {
        if (pthread_join(tids[i], NULL))
        {
            printf("--ERRO: pthread_join() \n");
            exit(4);
        }
    }
    GET_TIME(t1);
    multiplicacao = t1 - t0;

    GET_TIME(t0);
    /*printf("A:\n");
    for(int i = 0; i < dimensao; i++) {
        for(int j = 0; j < dimensao; j++) {
            printf("%f ", GET_MAT_VAL(matriz_a, i, j, dimensao));
        }
        printf("\n");
    }

    printf("B:\n");
    for(int i = 0; i < dimensao; i++) {
        for(int j = 0; j < dimensao; j++) {
            printf("%f ", GET_MAT_VAL(matriz_b, i, j, dimensao));
        }
        printf("\n");
    }

    printf("C = A * B:\n");
    for(int i = 0; i < dimensao; i++) {
        for(int j = 0; j < dimensao; j++) {
            printf("%f ", GET_MAT_VAL(matriz_c, i, j, dimensao));
        }
        printf("\n");
    }*/
    GET_TIME(t1);
    resposta = t1 - t0;

    free(matriz_a);
    free(matriz_b);
    free(matriz_c);

    free(tids);
    free(args);

    printf("Inicialização: %fs\n", inicializacao);
    printf("Multiplicação: %fs\n", multiplicacao);
    //printf("Resposta: %fs\n", resposta);
}
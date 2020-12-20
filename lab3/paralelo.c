/*
  Laboratorio 3 - Computacao Concorrente
  Nome: Domenica Maria Cioci Ferreira Goncalves
  Prof: Silvana Rossetto
  Periodo: 2020.1 REMOTO
  Implementacao e avaliacao de aplicacoes concorrentes (parte 2)
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <pthread.h>

#include "timer.h"

int n_threads;
long long int iteracoes;
double* somas;

double termo(long long int indice) {
    if(indice % 2 == 0) {
        return 1 / (double)(indice * 2 + 1);
    } else {
        return -1 / (double)(indice * 2 + 1);
    }
}

void* tarefa(void* arg) {
    int id = (int) arg;
    long long int tam = iteracoes / n_threads;
    long long int inicio = tam * id;
    long long int fim;
    if(id == n_threads -1){
        fim = iteracoes;
    } else{
        fim  = tam + inicio;
    }

    double acumulador = 0;
    for(long long int i = inicio; i < fim; i++) {
        acumulador += termo(i);
    }

    somas[id] = acumulador;
    pthread_exit(NULL);
}

int main (int argc, char** argv) {
    double t0, t1;

    if(argc != 3) {
        printf("Uso: %s <N_ITERACOES> <N_THREADS>\n", argv[0]);
        exit(1);
    }

    iteracoes = atoll(argv[1]);

    n_threads = atoi(argv[2]);

    pthread_t* tids = (pthread_t*) malloc(sizeof(pthread_t) * n_threads);

    somas = (double*) malloc(sizeof(double) * n_threads);

    GET_TIME(t0);

    for(int i = 0; i < n_threads; i++) {
        pthread_create(&tids[i], NULL, tarefa, (void*) i);
    }

    for(int i = 0; i < n_threads; i++) {
        pthread_join(tids[i], NULL);
    }

    double acumulador = 0;
    for(int i = 0; i < n_threads; i++) {
        acumulador += somas[i];
    }

    double pi = 4 * acumulador; 

    GET_TIME(t1);

    free(tids);
    free(somas);

    double diferenca = pi - M_PI;

    printf("Pi: %.15lf\n", pi);
    printf("Dif: %.15lf\n", diferenca);
    printf("Tempo: %lfs\n", t1 - t0);
} 
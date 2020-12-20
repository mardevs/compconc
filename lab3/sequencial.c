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

#include "timer.h"

double termo(long long int indice) {
    if(indice % 2 == 0) {
        return 1 / (double)(indice * 2 + 1);
    } else {
        return -1 / (double)(indice * 2 + 1);
    }
}

int main (int argc, char** argv) {
    double t0, t1;

    if(argc != 2) {
        printf("Uso: %s <N_ITERACOES>\n", argv[0]);
        exit(1);
    }

    long long int iteracoes = atoll(argv[1]);

    GET_TIME(t0);
    double acumulador = 0;
    for(long long int i = 0; i < iteracoes; i++) {
        acumulador += termo(i);
    }
    
    double pi = 4 * acumulador;
    GET_TIME(t1);

    
    double diferenca = pi - M_PI;

    printf("Pi: %.15lf\n", pi);
    printf("Dif: %.15lf\n", diferenca);
    printf("Tempo: %lfs\n", t1 - t0);
} 
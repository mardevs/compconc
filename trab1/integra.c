/*
  Trabalho de Implementação 1 - Computacao Concorrente
  Nome: Domenica Maria Cioci Ferreira Goncalves
  Prof: Silvana Rossetto
  Periodo: 2020.1 REMOTO
  Integração numérica trapezoidal
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <pthread.h>

#include "timer.h"

#define N_FUNCOES 3

typedef double (*funcao_real) (double);

// Struct para passar argumentos para a thread

typedef struct 
{
    funcao_real f;
    int n_threads;
    int id_thread;
    long long n_trapezios;
    double limite_inferior;
    double base_trapezio;

    double resultado_parcial;
} Args;

// Definicao das funcoes teste

double funcao_1(double x) {
    return x*x;
}

double primitiva_funcao_1(double x) {
    return 1.0/3.0 * x*x*x;
}

double funcao_2(double x) {
    return x*x*x - 4.0*x*x + 3.0*x - 12.0;
}

double primitiva_funcao_2(double x) {
    return 1.0/4.0 * x*x*x*x - 4.0/3.0*x*x*x + 3.0/2.0*x*x - 12.0*x;
}

double funcao_3(double x) {
    return sin(x);
}

double primitiva_funcao_3(double x) {
    return -cos(x);
}

funcao_real funcoes[N_FUNCOES] = {funcao_1, funcao_2, funcao_3};
funcao_real primitivas[N_FUNCOES] = {primitiva_funcao_1, primitiva_funcao_2, primitiva_funcao_3};

// Interface do programa

void imprime_uso(char* nome_programa) {
    printf("Uso: %s <N_TRAPEZIOS> <N_THREADS> <ID_FUNCAO> <LIMITE_INFERIOR> <LIMITE_SUPERIOR>\n", nome_programa);
    printf("Funções possíveis:\n");
    printf("(1) x^2\n");
    printf("(2) x^3 - 4x^2 + 3x - 12\n");
    printf("(3) sen(x)\n");
}

double area_trapezio(double base, double y0, double y1) {
    return ((y0 + y1) * base) / 2.0;
}

double integra_sequencial(funcao_real f, long long n_trapezios, double limite_inferior, double limite_superior) {
    double comprimento = limite_superior - limite_inferior;
    // Se limite superior for menor que limite inferior, a base será negativa
    // o que garante que o valor da integral estará correto
    double base_trapezio = comprimento / n_trapezios;

    double x0 = limite_inferior;
    double acumulador = 0;
    for(long long i = 0; i < n_trapezios; i++) {
        double x1 = x0 + base_trapezio;
        double y0 = f(x0);
        double y1 = f(x1);

        acumulador += area_trapezio(base_trapezio, y0, y1);

        x0 = x1;
    }

    return acumulador;
}

void* tarefa_integracao(void* _arg) {
    Args* args = (Args*) _arg;

    // Divisao das tarefas em blocos. 
    // Cada thread pega um bloco continuo de trapezios.

    long long tam_bloco = args->n_trapezios / args->n_threads;
    long long inicio = args->id_thread * tam_bloco;
    long long fim;

    if(args->id_thread < args->n_threads - 1) {
        fim = inicio + tam_bloco;
    } else {
        fim = args->n_trapezios;
    }

    double x0 = args->limite_inferior + (args->base_trapezio * inicio);
    double acumulador = 0;
    for(long long i = inicio; i < fim; i++) {
        double x1 = x0 + args->base_trapezio;
        double y0 = args->f(x0);
        double y1 = args->f(x1);

        acumulador += area_trapezio(args->base_trapezio, y0, y1);

        x0 = x1;
    }

    args->resultado_parcial = acumulador;

    pthread_exit(NULL);
}

double integra_paralelo(funcao_real f, int n_threads, long long n_trapezios, double limite_inferior, double limite_superior) {
    double comprimento = limite_superior - limite_inferior;
    // Se limite superior for menor que limite inferior, a base será negativa
    // o que garante que o valor da integral estará correto
    double base_trapezio = comprimento / n_trapezios;
    
    // Criacao e definicao das threads

    pthread_t* tids = (pthread_t*) malloc(sizeof(pthread_t) * n_threads);
    Args* args = (Args*) malloc(sizeof(Args) * n_threads);

    for(int i = 0; i < n_threads; i++) {
        args[i].id_thread = i;
        args[i].n_threads = n_threads;
        args[i].n_trapezios = n_trapezios;
        args[i].f = f;
        args[i].base_trapezio = base_trapezio;
        args[i].limite_inferior = limite_inferior;
        args[i].resultado_parcial = 0;

        pthread_create(&tids[i], NULL, tarefa_integracao, &args[i]);
    }

    for(int i = 0; i < n_threads; i++) {
        pthread_join(tids[i], NULL);
    }

    double acumulador = 0;
    for(int i = 0; i < n_threads; i++) {
        acumulador += args[i].resultado_parcial;
    }

    return acumulador;
}

int main (int argc, char** argv) {
    double t0, t1;

    // Entrada

    if(argc != 6) {
        imprime_uso(argv[0]);
        exit(1);
    }

    long long n_trapezios = atoll(argv[1]);
    int n_threads = atoi(argv[2]);
    int id_funcao = atoi(argv[3]) - 1;
    double limite_inferior = atof(argv[4]);
    double limite_superior = atof(argv[5]);
    
    if(n_trapezios < 1) {
        printf("ERRO: O número de trapézios deve ser positivo\n");
        imprime_uso(argv[0]);
        exit(1);
    }

    if(n_threads < 1) {
        printf("ERRO: O número de threads deve ser positivo\n");
        imprime_uso(argv[0]);
        exit(1);
    }

    if(id_funcao < 0 || id_funcao >= N_FUNCOES) {
        printf("ERRO: Id de função inválido\n");
        imprime_uso(argv[0]);
        exit(1);
    }

    GET_TIME(t0);
    double resultado;
    // Utilizando versao sequencial para comparar o overhead da criacao das threads
    // no caso de multiplas threads
    if(n_threads == 1) {
        resultado = integra_sequencial(funcoes[id_funcao], n_trapezios, limite_inferior, limite_superior);
    } else {
        resultado = integra_paralelo(funcoes[id_funcao], n_threads, n_trapezios, limite_inferior, limite_superior);
    }
    GET_TIME(t1);

    double resultado_esperado = primitivas[id_funcao](limite_superior) - primitivas[id_funcao](limite_inferior);
    double erro = resultado_esperado - resultado;

    printf("Resultado: %.12lf\n", resultado);
    printf("Resultado esperado: %.12lf\n", resultado_esperado);
    printf("Erro: %.12lf\n\n", erro);

    printf("Tempo: %lfs\n", t1 - t0);
}

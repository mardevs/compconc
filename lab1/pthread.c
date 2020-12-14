/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Aluno : Domenica Maria Cioci Ferreira Gonçalves */
/* Módulo 1 - Laboratório: 1 */
/* Laboratório 1: Implementação de programa concorrente com 2 threads */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NTHREADS 2 //total de threads a serem criadas
#define ARRAY_SIZE 100

//cria a estrutura de dados para armazenar os argumentos da thread
typedef struct
{
    int idThread;
    int nThreads;
    int* array;
    int array_size;
} t_Args;

//funcao executada pelas threads
void *Incrementa(void *arg) {
    t_Args *args = (t_Args *)arg;

    int fatia = args->array_size / args->nThreads;
    int inicio = args->idThread * fatia;
    int fim = inicio + fatia;
    int i;
    
    if (args->idThread == args->nThreads - 1){
        fim = ARRAY_SIZE;
    }

    for(i = inicio; i < fim; i++) {
        args->array[i]++;
    }


    free(arg); //aqui pode liberar a alocacao feita na main

    pthread_exit(NULL);
}

//funcao principal do programa
int main() {

    pthread_t tid_sistema[NTHREADS]; //identificadores das threads no sistema
    int i;                           //variavel auxiliar
    int valores[ARRAY_SIZE];

    for (i = 0; i < ARRAY_SIZE; i++){
        valores[i] = i;
    }

    printf("Vetor inicial:\n");

    for (i = 0; i < ARRAY_SIZE ; i++){
        
        printf("%d ", valores[i]);
    }

    printf("\n");

    for (i = 0; i < NTHREADS; i++) {
        t_Args *arg = malloc(sizeof(t_Args)); //receberá os argumentos para a thread
        if (arg == NULL)
        {
            printf("--ERRO: malloc()\n");
            exit(-1);
        }

        arg->idThread = i; 
        arg->nThreads = NTHREADS; 
        arg->array = valores;
        arg->array_size = ARRAY_SIZE;



        if (pthread_create(&tid_sistema[i], NULL, Incrementa, (void*) arg)) {
          printf("--ERRO: pthread_create()\n"); exit(-1);
        }
    }

    // espera todas as threads terminarem
    for (i = 0; i < NTHREADS; i++) {
        if (pthread_join(tid_sistema[i], NULL))
        {
            printf("--ERRO: pthread_join() \n");
            exit(-1);
        }
    }

    printf("Vetor incrementado:\n");

    for (i = 0; i < ARRAY_SIZE ; i++){
        
        printf("%d ", valores[i]);
    }

    printf("\n");

    pthread_exit(NULL);
}

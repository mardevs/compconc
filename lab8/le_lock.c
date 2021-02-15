#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <semaphore.h>

#define N_LEIT 10
#define N_ESC  3

// semaforos
pthread_mutex_t em_e; // Lock de exclusao mutua dos escritores.
pthread_mutex_t em_l; // Lock de exclusao mutua dos leitores.
sem_t escr; // Semaforo de condicao representando que um escritor pode escrever. Iniciado com 1.
sem_t leit; // Semaforo de condicao representando que um leitor pode ler. Iniciado com 1.

// globais
int e = 0; // Contador de escritores esperando
int l = 0; // Contador de leitores esperando

void* le(void* args) {
    int id = *((int*)args);

    while(1) {
        printf("L%d entrou\n", id);

        sem_wait(&leit);
        pthread_mutex_lock(&em_l); l++;

        if(l == 1) {
            printf("Leitores esperando para ler\n");
            sem_wait(&escr);
            printf("Leitores podem ler\n");
        }

        pthread_mutex_unlock(&em_l);
        sem_post(&leit);
    
        // le ...
        printf("L%d lendo ...\n", id);

        if(e_count > 0) {
            printf("Erro: L%d leu enquanto escritores estavam escrevendo\n");
        }

        // Simulando leitura
        double boba = id;
        for(int i = 0; i < 100; i++) boba /= 2;

        printf("L%d terminou de ler\n", id);

        pthread_mutex_lock(&em_l); l--;

        if(l == 0) {
            printf("Liberando acesso para escritores");
            sem_post(&escr);
        }

        pthread_mutex_unlock(&em_l);
    }

    pthread_exit(NULL);
}

void* escreve(void* args) {
    int id = *((int*)args);

    while(1) {
        printf("E%d entrou\n", id);

        pthread_mutex_lock(&em_e); e++;

        if(e == 1) {
            printf("Escritores esperando para escrever\n");
            sem_wait(&leit);
            printf("Escritores podem escrever\n");
        }
        
        pthread_mutex_unlock(&em_e);
        sem_wait(&escr);

        // escreve ...
        printf("E%d escrevendo ...\n", id);

        if(e_count > 1) {
            printf("Erro: E%d escreveu enquanto escritores estavam escrevendo\n");
        }

        if(l_count > 0) {
            printf("Erro: E%d escreveu enquanto leitores estavam lendo\n");
        }
        // Simulando escrita
        double boba = id;
        for(int i = 0; i < 100; i++) boba /= 2;

        printf("E%d terminou de escrever\n", id);

        sem_post(&escr);
        pthread_mutex_lock(&em_e); e--;
        
        if(e == 0) {
            printf("Liberando acesso para leitores");
            sem_post(&leit);
        }

        pthread_mutex_unlock(&em_e);
    }

    pthread_exit(NULL);
}

int main (int argc, char** argv) {
    pthread_t tids[N_LEIT + N_ESC];
    int ids[N_LEIT + N_ESC];

    for (int i = 0; i < N_LEIT; i++) {
        ids[i] = i + 1;
    }

    for (int i = 0; i < N_ESC; i++) {
        ids[N_LEIT + i] = i + 1;
    }

    //inicia as locks
    pthread_mutex_init(&em_e, NULL);
    pthread_mutex_init(&em_l, NULL);
    
    //inicia os semaforos
    sem_init(&escr, 0, 1);
    sem_init(&leit, 0, 1);
    
    //cria threads
    for (int i = 0; i < N_LEIT; i++) {
        pthread_create(&tids[i], NULL, le, &ids[i]);
    }

    for (int i = 0; i < N_ESC; i++) {
        pthread_create(&tids[N_LEIT + i], NULL, escreve, &ids[N_LEIT + i]);
    }
    
    //--espera todas as threads terminarem
    for (int i = 0; i < N_LEIT + N_ESC; i++) {
        pthread_join(tids[i], NULL);
    }

    pthread_mutex_destroy(&em_e);
    pthread_mutex_destroy(&em_l);

    sem_destroy(&escr);
    sem_destroy(&leit);

    pthread_exit(NULL);
}
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define NUM_THREADS 5

/*Crio todas as variaveis globais necessarias*/
int n;
pthread_t clientes[NUM_THREADS];
pthread_t banco1;
pthread_mutex_t mutex;

/*Funcoes que as threads farao*/
void *operacoes(void *arg){

    pthread_mutex_lock(&mutex); //Bloqueia o mutex para acessar regiao critica

    int *ops = (int *)malloc(n * sizeof(int));
    ops = (int *)arg;

    for(int i = 0; i < n; i++){
        if(ops[i] == 1){
            printf("O seu saldo atual eh: $1000\n");
        }
        else if(ops[i] == 2){
            printf("depositando...\n");
        }
        else if(ops[i] == 3){
            printf("Sacando...\n");
        }
        else{
            printf("Operacao invalida\n");
        }
    }

    pthread_mutex_unlock(&mutex); /*Desbloqueia a thread*/
    pthread_exit(NULL);
}

void *banco(void *arg){

    pthread_exit(NULL);
}

int main(void){
    printf("Digite a quantidade de operacoes que deseja realizar:");
    scanf("%i", &n);
 
    int *op = malloc(n * sizeof(int));

    if(op){
        for(int i = 0; i < n; i++){
            printf("Digite quais operacoes quer fazer.\n 1- Consultar Saldo\n 2- Deposito\n 3- Saque\n");
            scanf("%i", &op[i]);
        }

        for(int i = 0; i < NUM_THREADS; i ++){
            printf("Thread: %d\n", i);
            pthread_create(&clientes[i], NULL, operacoes, (void *)op);
        }

        for(int i = 0; i < NUM_THREADS; i++){
            pthread_join(clientes[i], NULL);
        }
    }
    
    free(op);
    pthread_mutex_destroy(&mutex);
    pthread_exit(NULL);
}
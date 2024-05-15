#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define NUM_THREADS 5
int n;

void *operacoes(void *arg){
    int *ops = (int *)malloc(n * sizeof(int));
    ops = (int *)arg;

    for(int i = 0; i < n; i++){
        if(ops[i] == 1){
            printf("O seu saldo atual eh: $1000\n");
        }
        else if(ops[i] = 2){
            printf("depositando...\n");
        }
        else if(ops[i] = 3){
            printf("Sacando...\n");
        }
        else{
            printf("Operacao invalida\n");
        }
    }
    pthread_exit(NULL);
}

void *banco(void *arg){

    pthread_exit(NULL);
}

int main(void){
    pthread_t clientes[NUM_THREADS];
    pthread_t banco;

    printf("Digite a quantidade de operacoes que deseja realizar:");
    scanf("%i", &n);
 
    int *op = malloc(n * sizeof(int));

    if(op){
        for(int i = 0; i < n; i++){
            printf("Digite quais operacoes quer fazer.\n 1- Consultar Saldo\n 2- Deposito\n 3- Saque\n");
            scanf("%i", &op[i]);
        }

        for(int i = 0; i < NUM_THREADS; i ++){
            int ver = pthread_create(&clientes[i], NULL, operacoes, (void *)op);

            if(ver){
                printf("Erro ao criar thread: %i", i);
                exit(-1);
            }
        }
    }
    
    free(op);
    pthread_exit(NULL);
}
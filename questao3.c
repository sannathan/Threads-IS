#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*Criacao das variaveis globais e defines*/
/*Criaremos um vetor de threads clientes, com a quantidade de clientes desejados*/
/*e criaremos apenas uma thread banco, como foi solicitado.*/
/*Logo apos, crio o mutex a ser utilizado por tanto banco quanto clientes. Esse mutex e responsavel por "travar"
  as outras threads e evitar a condicao de corrida*/

#define NUM_CLIENTS 5
#define SALDO 10000
#define NUM_OPERACOES 3

pthread_t threads_clientes[NUM_CLIENTS];
pthread_t thread_banco;
pthread_mutex_t mutex;
int continuar = 1; //Variavel que controla a continuidade da thread banco.

/*Conta bancaria a ser utilizada pelos clientes
  Que contem o saldo da conta*/
typedef struct{
    float *saldo;
    int thread;
}ContaBancaria;

//Funcao deposito, recebe o valor e deposita na conta
void *deposito(ContaBancaria *conta, float valor){
    pthread_mutex_lock(&mutex);

    *(conta->saldo) += valor;
    printf("A thread: %i fez um deposito de valor: %.2f. Saldo atual: %.2f\n", conta->thread, valor, *conta->saldo);

    pthread_mutex_unlock(&mutex);
    return NULL;
}
//Funcao saque, que recebe a variavel valor e retira esse valor do saldo
void *saque(ContaBancaria *conta, float valor){
    pthread_mutex_lock(&mutex);

    if(valor > *(conta->saldo)){
        printf("A thread: %i Tentou sacar: %.2f, mas seu saldo eh insuficiente\n", conta->thread, valor);
    }
    else{
        *(conta->saldo) -= valor;
        printf("A thread: %i fez um saque de %.2f!. Seu novo saldo eh: %.2f\n", conta->thread, valor, *conta->saldo);
    }
    pthread_mutex_unlock(&mutex);
    return NULL;
}

//Funcao saldo, apenas faz a verificacao do saldo
void *saldo(ContaBancaria *conta){
    pthread_mutex_lock(&mutex);

    printf("A thread: %i, fez uma verificacao de saldo. Saldo eh: %.2f\n", conta->thread, *conta->saldo);

    pthread_mutex_unlock(&mutex);
    return NULL;
}

//Funcao clientes que escolhe aleatoriamente entre as opcoes de saque, deposito e consulta de saldo.
void *clientes(void *arg){
    ContaBancaria *conta = (ContaBancaria *) arg;
    for(int i = 0; i < NUM_OPERACOES; i++){
        int operacao = rand() % 3;
        float valor = (rand() % 100) + 1;

        switch (operacao){
        case 0:
            saque(conta, valor);
            break;
        case 1:
            deposito(conta, valor);
            break;
        case 2:
            saldo(conta);
            break;
        }
        //Coloca a thread pra dormir e libera a CPU para outras threads
        usleep((rand() % 1000 + 100) * 1000);
    }
    pthread_exit(NULL);
}

//Funcao banco que periodicamente aumenta um pouquinho o saldo da conta
void *banco(void *arg){
    ContaBancaria *temp = (ContaBancaria*) arg;
    while(continuar){
        //para por 12 segundos a thread, liberando cpu para outras threads
        sleep(5);
        pthread_mutex_lock(&mutex);
        float juros = *(temp->saldo) * 0.01;
        *(temp->saldo) += juros;
        printf("O Banco aplicou juros de: %.2f. Saldo atual: %.2f\n", juros, *temp->saldo);
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(NULL);
}

int main(void){
    //Instancio a conta e atribuo o valor 10000 ao saldo, logo apos, inicializo o mutex para podermos utiliza-lo.
    ContaBancaria Conta[NUM_CLIENTS];
    float saldoTotal = SALDO;
    pthread_mutex_init(&mutex, NULL);

    //Crio a thread banco.
    pthread_create(&thread_banco, NULL, banco, &Conta);

    //Crio as threads clientes.
    for(int i = 0; i < NUM_CLIENTS; i++){
        Conta[i].saldo = &saldoTotal;
        Conta[i].thread = i;
        pthread_create(&threads_clientes[i], NULL, clientes, &Conta[i]);
    }

    //Espero todas se juntarem
    for(int j = 0; j < NUM_CLIENTS; j++){
        pthread_join(threads_clientes[j], NULL);
    }

    continuar = 0;
    pthread_join(thread_banco, NULL);

    printf("Todas as operacoes foram realizadas!\n");

    //Destruo o mutex e encerro o programa
    pthread_mutex_destroy(&mutex);
    return 0;
}
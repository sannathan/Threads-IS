#define _XOPEN_SOURCE 600
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

//-> Aqui definimos os valores das variáveis que definiram o tamanho das matrizes
// -> Foi pensado nos tamanhos que as matrizes que vão ser multiplicar tem que ter para ela ser possível
#define xA 2
#define yA 2
#define x 2
#define yX 1
// ---------------------------------------------------------------------------------

// -> Aqui as matrizes sendo declaradas globalmente 
double A[xA][yA] = {
        {2, 1},
        {5, 7}
};
double X[x][yX];
double B[xA][yX] = {
        {11},
        {13}
};
// ---------------------------------------------------------------------------------

pthread_barrier_t barrier;
pthread_barrier_t barrierIntermediária;

// -> Struct que tem a finalidade de armazenar quais dados será realizado por cada thread
typedef struct Thread_Data{
    double (*A)[yA]; 
    double (*X)[yX];
    double (*B)[yX];
    double X_Resposta[x][yX];
    int *id; 
    int qtdInc; 
    int startIdx;
    int endIdx;
}Thread_Data;
// ---------------------------------------------------------------------------------

void* Jacobi(void *arg);

int main(){
    
    int N; // QTD de threads
    int qtdInc = 0; // Qtd de incógnita;

    // Inicializando a matriz X e contando as incógnitas
    for(int i = 0; i < x; i++){
        for(int j = 0; j < yX; j++){
            X[i][j] = 1.0;
            qtdInc++;
        }
    }

    printf("Digite a quantidade de threads que se deseja trabalhar:\n");
    scanf("%d", &N);

    pthread_t threads[N];
    Thread_Data thread_data[N];

    int segment = qtdInc / N; // Parametro usado para definir a quantidade de incógnicas cada thread irá tratar
    int resto = qtdInc % N; // Caso a divisão não seja exata, o resto da divisão será armazenado aqui

    for (int i = 0; i < N; i++) {
        thread_data[i].A = A;
        thread_data[i].B = B;
        thread_data[i].X = X;
        thread_data[i].qtdInc = qtdInc;
        thread_data[i].startIdx = i * segment;
        if( i == N - 1){
            thread_data[i].endIdx = ((i + 1) * segment) + resto - 1; // Para o caso da divisão não seja exata, adicionamos o resto da divisão na última thread
        }
        else{
            thread_data[i].endIdx = ((i + 1) * segment) - 1;
        }
    }

    pthread_barrier_init(&barrier, NULL, N);

    for(int i = 0; i < N; i++){
        pthread_create(&threads[i], NULL, Jacobi, &thread_data[i]);
    }

    for(int i = 0; i < N; i++){
        pthread_join(threads[i], NULL);
    }
    pthread_barrier_destroy(&barrier);

    for (int i = 0; i < x; i++) {
        printf("X[%d] = %f\n", i, X[i][0]);
    }
  
    return 0;
}
void* Jacobi(void *arg){
    Thread_Data* data = (Thread_Data*)arg;
    int P = 100; // Devinimos um parametro de interaçẽs arbritrária, mas que seja alto o suficiente para resolver o problema para poucas incógnitas

    for(int k = 0; k < P; k++){
        for(int i = data->startIdx; i <= data->endIdx; i++){
            double sum = 0.0;
            for(int j = 0; j < x; j++){
                if( i != j){
                    sum += data->A[i][j] * X[j][0]; // Essa parte representa o somátorio do algoritmo, e após finalizado, faremos a subtração cujo resultado será dividido
                }
            }
            X[i][0] = (data->B[i][0] - sum) / data->A[i][i];
        }
        pthread_barrier_wait(&barrier);
    }
}

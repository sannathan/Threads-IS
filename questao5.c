#define _XOPEN_SOURCE 600
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define xA 2
#define yA 2
#define x 2
#define yX 1

double A[xA][yA] = {
        {2, 1},
        {5, 7}
};
double X[x][yX];
double B[xA][yX] = {
        {11},
        {13}
};

pthread_barrier_t barrier;
pthread_barrier_t barrierIntermediária;

typedef struct Thread_Data{
    double (*A)[yA]; //-> SIGNIFICADO: A é um ponteiro para uma matriz 2D no qual cada linha tem yA elementos
    double (*X)[yX];
    double (*B)[yX];
    double X_Resposta[x][yX];
    int *id; 
    int qtdInc; 
    int startIdx;
    int endIdx;
}Thread_Data;

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

    int segment = qtdInc / N;
    int resto = qtdInc % N;

    for (int i = 0; i < N; i++) {
        thread_data[i].A = A;
        thread_data[i].B = B;
        thread_data[i].X = X;
        thread_data[i].qtdInc = qtdInc;
        thread_data[i].startIdx = i * segment;
        if( i == N - 1){
            thread_data[i].endIdx = ((i + 1) * segment) + resto - 1;
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
    printf("ENTROU\n");
    Thread_Data* data = (Thread_Data*)arg;
    int P = 100; 

    for(int k = 0; k < P; k++){
        for(int i = data->startIdx; i <= data->endIdx; i++){
            double sum = 0.0;
            for(int j = 0; j < x; j++){
                if( i != j){
                    sum += data->A[i][j] * X[j][0];
                }
            }
            X[i][0] = (data->B[i][0] - sum) / data->A[i][i];
        }
        pthread_barrier_wait(&barrier);
    }
}

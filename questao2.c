#define _XOPEN_SOURCE 600
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 5
#define SIZE 9

typedef struct{
    int *array;
    int start;
    int end;
    int id;
}ThreadData; // Struct que usaremos para representar as N partes do array


ThreadData data[NUM_THREADS]; // Vetor das N partes
pthread_barrier_t barrier; // Barreira com a finalidade de garantir que a thread de mesclagem será acionada somente quando as threads de ordenação parcial terminarem
pthread_t threads[NUM_THREADS]; // Vetor de threads de ordenação
pthread_t  MergeThread; // Thread de mesclagem
int array[SIZE]; // Array base que será dividido em N partes
int arrayResposta[SIZE]; // Array que printará a resposta

void bubbleSort(ThreadData *arg); // Função com a finalidade de ordenar com o algoritmo pedido na questão
void merge(); // Função de mesclagem das N partes do array/

int main() {
    // ----------------------------------------------------------------------
    // Preenchemos o nosso array base de forma pseudo-aleatória a fim de demonstrar que o nosso código não há um truque para funcionar em determinados casos
    for(int i = 0 ; i < SIZE; i++){
        array[i] = rand() % 200;
    }

    // ----------------------------------------------------------------------

    
    int segmentSize = SIZE / NUM_THREADS; // Variável que armazena o tamanho padrão dos arrays parciais

    pthread_barrier_init(&barrier, NULL, NUM_THREADS + 1);  // NUM_THREAD + 1 porque estamos contando com a thread principal (main);

    for(int i = 0; i < NUM_THREADS; i++){
        data[i].array = array;
        data[i].start = i * segmentSize;
        if(i + 1 == NUM_THREADS){
            /*
                - Aqui entra o caso no qual a divisão entre o tamanho do array base com o núm. de partes não for exata;
                - Para solucionar o problema de forma prática, preferimos manter o tamanho padrão dos N-1 arrays e a qtd que sobresse seria armazenada no N array;
            */

            data[i].end = ((i+1) * segmentSize) + (SIZE % NUM_THREADS);
        }
        else{
            data[i].end = (i+1) * segmentSize;
        }
        
        data[i].id = i;
        pthread_create(&threads[i], NULL, bubbleSort, &data[i]);
    }
    
    pthread_barrier_wait(&barrier);
    pthread_create(&MergeThread, NULL, merge, NULL);

    pthread_join(MergeThread, NULL);
    for(int i = 0; i < NUM_THREADS; i++){
        pthread_join(threads[i], NULL);
    }

    pthread_barrier_destroy(&barrier);
    pthread_exit(NULL);
}

void bubbleSort(ThreadData *arg){
    int tmp;
    ThreadData *data_temp;
    data_temp = arg;

    for(int i = data_temp->start; i < data_temp->end - 1; i++){
        for(int j = data_temp->start; j < data_temp->end - (i- data_temp->start )- 1; j++){
            if(data_temp->array[j+1] < data_temp->array[j]){
                tmp = data_temp->array[j+1];
                data_temp->array[j+1] = data_temp->array[j];
                data_temp->array[j] = tmp; 
            }
        }
    }
    data[data_temp->id].array = data_temp->array; // Salvamos a ordenação feita na nossa representação dos arrays parciais;
    pthread_barrier_wait(&barrier);
}
void merge(){                 
    int indices[NUM_THREADS] = {0};
    int segment = SIZE / NUM_THREADS;

    for(int i = 0; i < NUM_THREADS; i++){
        indices[i] = i * segment; // Preenchemos  o array com os indices iniciais;
    }

    for(int x = 0; x < SIZE; x++){
        int indexMin = -1;
        int valorMin = __INT_MAX__; 
        
        for(int y = 0; y < NUM_THREADS; y++){
            if(y+1 == NUM_THREADS){ // Para o caso que o array parcial não tem o tamanho padrão
                if(indices[y] < (((y+1) * segment) + SIZE % NUM_THREADS) && array[indices[y]] < valorMin){
                valorMin = array[indices[y]];
                indexMin = y;
                }
            }
            else{
                if(indices[y] < (y+1) * segment && array[indices[y]] < valorMin){
                    valorMin = array[indices[y]];
                    indexMin = y;
                }
            }
        }
        if(indexMin != -1){
                arrayResposta[x] = array[indices[indexMin]];
                indices[indexMin]++;
            }
    }

    for (int i = 0; i < SIZE; i++) {
        printf("%d ", arrayResposta[i]);
    }
    printf("\n");
}

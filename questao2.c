/*
  -> Ideia Central do código;
1º) Dividir o array em 5 partes 
2º) Esperamos todas as threads Ordenarem pela barreira
3º) Depois chamamos a thread MergThread para mesclar as partes
*/

#define _XOPEN_SOURCE 600
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 5
#define SIZE 10

typedef struct{
    int *array;
    int start;
    int end;
    int id;
}ThreadData;

ThreadData data[NUM_THREADS]; // Os dados que irei manipular pela thread
pthread_barrier_t barrier; //Esperar as 5 threads terminarem de ordenar as N partes
pthread_t threads[NUM_THREADS];
pthread_t  MergeThread;
int array[SIZE];
int arrayResposta[SIZE];

void bubbleSort(ThreadData *arg);
void merge();


int main() {
     
    for(int i = 0 ; i < SIZE; i++){
        array[i] = rand() % 1001;
    }

    // Zona de manipulação de threads
    int segmentSize = SIZE / NUM_THREADS;

    pthread_barrier_init(&barrier, NULL, NUM_THREADS);  // -------> Barreira
    for(int i = 0; i < NUM_THREADS; i++){
        data[i].array = array;
        data[i].start = i * segmentSize;
        data[i].end = (i+1) * segmentSize;
        data[i].id = i;
        pthread_create(&threads[i], NULL, bubbleSort, &data[i]);
    }

    for(int i = 0; i < NUM_THREADS; i++){
        pthread_join(threads[i], NULL);
    }
    
    pthread_barrier_destroy(&barrier);

    pthread_create(&MergeThread, NULL, merge, NULL);
    pthread_join(MergeThread, NULL);
    
    

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
    data[data_temp->id].array = data_temp->array;
    pthread_barrier_wait(&barrier);
}
void merge(){
    int cont = 0;
    int index = 0;
    int segment = SIZE / NUM_THREADS;

    for(int i = 0; i < SIZE; i++){
        if(data[index].start >= i && data[index].end <= i){
            arrayResposta[i] = data[index].array[i];
            cont++;
        }
        if(cont == segment){
            index++;
            cont = 0;
        }
    }
    int tmp;
    for(int x = 0; x < SIZE - 1; x++){
        for(int y = 0; y < SIZE - x - 1; y++){
            if(arrayResposta[y+1] < arrayResposta[y]){
                tmp = arrayResposta[y+1];
                arrayResposta[y+1] = arrayResposta[y];
                arrayResposta[y] = tmp;
            }
        }
    }
    for(int i = 0; i < SIZE; i++){
        printf("%d ", arrayResposta[i]);
    }
}

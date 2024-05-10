//não está terminado
//problema: falha de segmentação no vs code, funciona em outros
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
char search_word[50];
pthread_mutex_t mymutex = PTHREAD_MUTEX_INITIALIZER;
int counter = 0;


void *ReadFile(FILE *file);

int main(){
    int num_files;
    printf("quantos arquivos devem ser lidos?:");
    scanf("%d", &num_files);
    char files[num_files][100];
    for(int i = 0; i < num_files; i++){
        printf("arquivo %d: ", (i+1));
        scanf("%s", files[i]);
    }
    printf("palavra a ser procurada: ");
    scanf("%s", search_word);
    pthread_t *threads[num_files];
    FILE *file_read[num_files];
    int check;
    for(int i = 0; i < num_files; i++){
        file_read[i] = fopen(files[i], "r"); 
        check = pthread_create(threads[i], NULL, ReadFile, files[i]);
    }
    printf("Resultado: %d", counter);
    for(int i = 0; i < num_files; i++){
        fclose(file_read[i]);
    }
    pthread_exit(NULL);
    return 0;
}

void *ReadFile(FILE *file){
    char string[50];
    while (fscanf(file, "%s", string) != EOF){
        if(string){
            if(strcmp(string, search_word) == 0){
                pthread_mutex_lock(&mymutex);
                counter++;
                pthread_mutex_unlock(&mymutex);
            }
        }
    }
}


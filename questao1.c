//não está terminado
//problema: falha de segmentação no vs code, funciona em outros
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
char search_word[50];
pthread_mutex_t mymutex = PTHREAD_MUTEX_INITIALIZER;
int counter = 0;


void *ReadFile(void *file);

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
    pthread_t threads[num_files];
    int check;
    for(int i = 0; i < num_files; i++){
        check = pthread_create(&threads[i], NULL, ReadFile, (void*) files[i]);
        if(check){
          printf("erro\n");
        }
    }
    printf("Resultado: %d", counter);
    return 0;
}

void *ReadFile(void *file){
    FILE *file_read;
    file_read = fopen(file, "r");
    char string[50];
    while (fscanf(file, "%s", string) != EOF){
        printf("palavra: %s\n", string);
        if(string){
            if(strcmp(string, search_word) == 0){
                pthread_mutex_lock(&mymutex);
                counter++;
                pthread_mutex_unlock(&mymutex);
            }
        }
    }
    fclose(file_read);
    pthread_exit(NULL);
}

//concluida
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
char search_word[50];
pthread_mutex_t mymutex = PTHREAD_MUTEX_INITIALIZER;
int counter = 0;

typedef struct arg{
    char path[100];

}arg;

void *ReadFile(void *file);

int main(){
    int num_files;
    printf("quantos arquivos devem ser lidos?:");
    scanf("%d", &num_files);
    arg files_name[num_files];
    for(int i = 0; i < num_files; i++){
        printf("arquivo %d: ", (i+1));
        scanf("%s", files_name[i].path);
    }
    printf("palavra a ser procurada: ");
    scanf("%s", search_word);
    pthread_t threads[num_files];
    int check;
    for(int i = 0; i < num_files; i++){
        check = pthread_create(&threads[i], NULL, ReadFile, &files_name[i]);
        if(check){
          printf("erro\n");
        }
    }
    for(int i = 0; i < num_files; i++){
      pthread_join(threads[i], NULL);
    }
    printf("Resultado: %d", counter);
    return 0;
}

void *ReadFile(void *file){
    arg *file_name = (arg *) file; 
    printf("file name: %s\n", file_name->path);

    FILE *file_read;

    file_read = fopen(file_name->path, "r");
    char string[50];
    while (fscanf(file_read, "%s", string) != EOF){
        printf("palavra: %s\n", string);
        if(string){
            if(strcmp(string, search_word) == 0){
                printf("palavra da vez: %s\n", string);
                pthread_mutex_lock(&mymutex);
                printf("palavra da vez dentro do mutex: %s\n", string);
                counter++;
                pthread_mutex_unlock(&mymutex);
            }
        }
    }
    fclose(file_read);
    pthread_exit(NULL);
}

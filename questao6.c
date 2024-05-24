#include <pthread.h>
#include <stdio.h>

#define N 4 //Quantidade de nucleos do sistema operacional e quantidade maxima de threads em execucao

pthread_t escalonador; //Thread escalonador
pthread_t threads[5];
pthread_mutex_t mutex; //Mutex a ser utilizado em conjunto com as variaveis de condicao
pthread_cond_t acordado;
pthread_cond_t sleep;

//Implementacao de fila para a criacao da lista_pronto
typedef struct link{
    char element;
    struct link *next;
}Link;

typedef struct{
    Link *front;
    Link *rear;
    int size;
}Queue;

Link *create_link(Link *nextval){
    Link *n = (Link *)malloc(sizeof(Link)) ;
    n->next = nextval;
    return n; 
}

Link *create_linkao(char it, Link *nextval){
    Link *n = (Link *)malloc(sizeof(Link));
    n->element = it;
    n->next = nextval;
    return n;
}

Queue *create_queue(){
    Queue *q = (Queue *)malloc(sizeof(Queue));
    q->rear = create_link(NULL);
    q->front = q->rear;
    q->size = 0;
    return q;Link
}

void dequeue(Queue *q){
    char it;    
    if(q->size != 0){
        it = q->front->next->element;
        Link *cache = q->front->next;
        q->front->next = q->front->next->next;
        free(cache);

        if(q->front->next == NULL){
            q->rear = q->front;
        }
        q->size--;
    }
}

void *cliente(void *arg){
    pthread_mutex_lock(&mutex);


}
int main(void){
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&varCond, NULL);

    Queue *lista_pronto = create_queue();

    pthread_create(escalonador, NULL, )


    return 0;
}
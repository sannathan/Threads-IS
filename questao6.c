#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//Implementacao de fila para a criacao da lista_pronto
typedef struct link{
    pthread_t thread_id;
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

Link *create_linkao(pthread_t it, Link *nextval){
    Link *n = (Link *)malloc(sizeof(Link));
    n->thread_id = it;
    n->next = nextval;
    return n;
}

Queue *create_queue(){
    Queue *q = (Queue *)malloc(sizeof(Queue));
    q->rear = create_link(NULL);
    q->front = q->rear;
    q->size = 0;
    return q;
}


void enqueue(Queue *q, pthread_t element) {
    // Cria um novo nó com o elemento fornecido
    Link *newLink = create_linkao(element, NULL);
    
    // Adiciona o novo nó ao final da fila
    q->rear->next = newLink;
    q->rear = newLink;
    
    // Incrementa o tamanho da fila
    q->size++;
}

pthread_t dequeue(Queue *q){
    char it;    
    if(q->size != 0){
        it = q->front->next->thread_id;
        Link *cache = q->front->next;
      	pthread_t Thread_id = cache->thread_id;
        q->front->next = q->front->next->next;
        free(cache);

        if(q->front->next == NULL){
            q->rear = q->front;
        }
        q->size--;
      	return Thread_id;
    }
}

#define N 4 //Quantidade de nucleos do sistema operacional e quantidade maxima de threads em execucao
#define true 1
#define num_Threads 10 //Quantidade de threads a serem criadas
Queue *Lista_pronto;
int emProcesso;
int threads_executadas;
pthread_t escalonador; //Thread escalonador
pthread_t threads[num_Threads]; //Threads a serem criadas
pthread_mutex_t mutex1; //Mutex a ser utilizado em conjunto com as variaveis de condicao
pthread_mutex_t mutex2; //Mutex para acessar a variavel compartilhada emProcesso
pthread_mutex_t mutex3;
pthread_cond_t cond;

void *escalonamento(void *lista_pronto){
    Queue *temp = (Queue *)lista_pronto;
		while(true){
      pthread_mutex_lock(&mutex1);
      while(temp->size == 0){
        pthread_cond_wait(&cond, &mutex1);
      }
      
      while(temp->size > 0 && emProcesso< N){
        pthread_t Thread_id = dequeue(temp);
        pthread_mutex_lock(&mutex2);
        emProcesso++;
        pthread_mutex_unlock(&mutex2);        
      }
      pthread_mutex_unlock(&mutex1);
      
    }
  return NULL;
}

void *function(void *arg){
  	Queue *temp = (Queue *)arg;
  	sleep(3);
    printf("A thread: %ld esta executando\n", pthread_self());
  	pthread_mutex_lock(&mutex2);\
    printf("Threads em processamento: %d\n",emProcesso);
  	emProcesso--;
    threads_executadas++;
  	printf("Threads em processo após decremento: %d\n", emProcesso);
  	pthread_mutex_unlock(&mutex2);
  	pthread_exit(NULL);
}
void agendar(int index){
    //Cria a thread[index] e a adiciona a lista de threads prontas
  	pthread_create(&threads[index], NULL, function, (void *)Lista_pronto);  
    pthread_mutex_lock(&mutex1);
  	enqueue(Lista_pronto, threads[index]); //Adiciona a thread[index] a lista de threads prontas
  	pthread_cond_signal(&cond); //Manda o sinal para o escalonador dizendo que tem item na lista pronto
  	pthread_mutex_unlock(&mutex1);
}
int main(void){
    //Inicializa tanto mutex quanto a variavel de condicao.
    pthread_mutex_init(&mutex1, NULL);
  	pthread_mutex_init(&mutex2, NULL);
    pthread_cond_init(&cond, NULL);
  	emProcesso = 0;
    threads_executadas = 0;

    //Cria a lista pronto
    Lista_pronto = create_queue();

  	//Cria a thread escalonador
    pthread_create(&escalonador,NULL,escalonamento,(void *)Lista_pronto);
  
  	//Utilizamos a funcao agendar, para criar as threads e adiciona-las a lista_pronto.
  	for(int i = 0; i < num_Threads; i++){
    		agendar(i);
    }

    for(int i = 0; i < num_Threads; i++){
        pthread_join(threads[i], NULL);
    }

    printf("Threads que foram executadas ao todo: %d\n", threads_executadas);
    pthread_exit(NULL);
}
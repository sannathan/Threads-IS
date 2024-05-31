#include <pthread.h>
#include <stdio.h>

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
    n->element = it;
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


pthrea enqueue(Queue *q, pthread_t element) {
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
        it = q->front->next->element;
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
#define num_Threads 10;
Queue *Lista_pronto = create_queue();
pthread_t escalonador; //Thread escalonador
pthread_t threads[num_Threads];
pthread_mutex_t mutex; //Mutex a ser utilizado em conjunto com as variaveis de condicao
pthread_cond_t cond; 

void *escalonamento(void *lista_pronto){
		while(true){
      pthread_mutex_lock(&mutex);
      while(lista_pronto->size == 0){
        pthread_cond_wait(&cond, &mutex);
      }
      
      while(lista_pronto->size > 0){
        pthread_t Thread_id = dequeue(Lista_pronto);
        pthread_mutex_unlock(&mutex);
        pthread_join(thread_id, NULL);
        pthread_mutex_lock(&mutex);
      }
      pthread_mutex_unlock(&mutex);
      
    }
  return null;
}

void *function(void *arg){
  
}
void agendar(int index){
  	pthread_create(&threads[index], NULL, function, NULL)
      
    pthread_mutex_lock(&mutex);
  	enqueue(Lista_pronto, threads[index]); //Adiciona a thread[index] a lista de threads prontas
  	pthread_cond_signal(&cond); //Manda o sinal para o escalonador dizendo que tem item na lista pronto
  	pthread_mutex_unlock(&mutex);
}
int main(void){
    //Inicializa tanto mutex quanto a variavel de condicao.
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&Cond, NULL);

  	//Cria a thread escalonador
    pthread_create(&escalonador,NULL,escalonamento,(void *)lista_pronto);
  
  	//Utilizamos a funcao agendar, para criar as threads e adiciona-las a lista_pronto.
  	for(int i = 0; i < num_Threads; i++){
    		agendar(i);
    }
    return 0;
}
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define NUM_THREADS 27

int M[9][9]= {
    
    {5, 3, 4, 6, 7, 8, 9, 1, 2},
    {6, 7, 2, 1, 9, 5, 3, 4, 8},
    {1, 9, 8, 3, 4, 2, 5, 6, 7},
    {8, 5, 9, 7, 6, 1, 4, 2, 3},
    {4, 2, 6, 8, 5, 3, 7, 9, 1},
    {7, 1, 3, 9, 2, 4, 8, 5, 6},
    {9, 6, 1, 5, 3, 7, 2, 8, 4},
    {2, 8, 7, 4, 1, 9, 6, 3, 5},
    {3, 4, 5, 2, 8, 6, 1, 7, 9}
};//Caso valido do sudoku



/*={ {1,2,3,4,5,6,7,8,9},
                {1,2,3,4,5,6,7,8,9},
                {1,2,3,4,5,6,7,8,9},
                {1,2,3,4,5,6,7,8,9},
                {1,2,3,4,5,6,7,8,9},
                {1,2,3,4,5,6,7,8,9},
                {1,2,3,4,5,6,7,8,9},
                {1,2,3,4,5,6,7,8,9},
                {1,2,3,4,5,6,7,8,9}
              };caso invalido de um sudoku*/
typedef struct {   
    int valor;
    int presente;
    
}vet_teste;/*Estrutura de elemento para o array 
que ira ser comparado a coluna,linha ou subgrade,
onde valor reprensenta os numeros possiveis do sudoku
 e presente se ele ja foi contabilizado */         

typedef struct {
    long tid;
    int *resposta;
}ThArg;/*Estrutura usado como referencia para o ponteiro transformado
 em void usado na funcao,onde tid e passado o id das threads
e resposta um ponteiro que aponta pra variavel valido*/ 


void *sudoku(void *args) {
    ThArg *threadArgs = (ThArg *)args;
    int i,j,k;
    long threadId=threadArgs->tid;
    int *valido=threadArgs->resposta;
    vet_teste *teste= (vet_teste *) malloc(9 * sizeof(vet_teste));
      //crio o array e prencho os valores e o estado de presença
        for(i=0; i<9; i++){
          teste[i].valor=i;
          teste[i].presente=0;
         
        }
    if(threadId<9){ 
        /*Separo as noves primeiras threads para percorrer as 9 linhas e testar 
        se existe numeros repetidos*/
            for(j=0;j<9;j++){
                for(k=0;k<9;k++){     
                     
                     if(M[threadId][j]==teste[k].valor&&teste[k].presente==0){
                        teste[k].presente=1;
                     }
                     else if(M[threadId][j]==teste[k].valor&&teste[k].presente==0){
                         *valido=1; //se a algum numero repetido mudo a variavel valido
                     }    
                }
            }
        }
    else if(threadId<18){
    //Separ as proximas 9 para testar as colunas
            for(i=0;i<9;i++){
                for(k=0;k<9;k++){     
                     
                     if(M[i][threadId-9]==teste[k].valor&&teste[k].presente==0){
                        teste[k].presente=1;
                     }
                     else if(M[i][threadId-9]==teste[k].valor&&teste[k].presente==1){
                         *valido=1; 
                     }    
                }
            }

        }
    else if(threadId<27){
    // e as restantes para testar as subgrades
          int indicei,indicej;
          if(threadId-18<3){
               indicei=0;}

          else if(threadId-18<6){
               indicei=3;}

          else{
               indicei=6;}
                      
          if((threadId-18)%3==0){
               indicej=0;}
          else if((threadId-18)%3==1){
               indicej=3;}   
          else{
               indicej=6;}
                      
          for(i=0;i<3;i++){
              for(j=0;j<3;j++){
                  
                  for(k=0;k<9;k++){     
                     if(M[indicei+i][indicej+j]==teste[k].valor&&teste[k].presente==0){
                        teste[k].presente=1;
                     }
                     else if(M[indicei+i][indicej+j]==teste[k].valor&&teste[k].presente==1){
                         *valido=1; 
                     }    
                }}}
        
          
        }    
free(teste);
}

int main (int argc, char *argv[]){
    pthread_t threads[NUM_THREADS];
    ThArg *args[NUM_THREADS];
    int i,j;
    int valido=0;
    for(i=0; i<NUM_THREADS; i++){  //crios as threads
        args[i] = (ThArg *)malloc(sizeof(ThArg));
        args[i]->tid = i;
        args[i]->resposta = &valido; 
        pthread_create(&threads[i],NULL,sudoku,(void *)args[i]);
}
    for(i=0; i<NUM_THREADS;i++) {//Espero todas as threads terminar
       pthread_join(threads[i], NULL);
    }
    for(i=0; i<NUM_THREADS;i++) {
       free(args[i]);  
    }
    if(valido==0)//printo a solucao
    {printf("A solucao e um sudoku");}
    else{printf("a solucao nao e u sudoku");}
    
    
  return 0;   
}  
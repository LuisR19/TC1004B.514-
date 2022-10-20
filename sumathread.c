#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#define NUM_THREADS 10


int saldo;
sem_t protecSaldo;

void *suma(void *arg){
    sem_wait(&protecSaldo); //decrementa
    printf("El saldo inicial es %d\n",saldo);
    saldo = saldo + 100;
    printf("El saldo despues es %d\n",saldo);
    sem_post(&protecSaldo); //incrementa
    pthread_exit(NULL);
    
}


int main(){
    saldo = 0;
    pthread_t threadsInfo[NUM_THREADS];
    sem_init(&protecSaldo,0,1);
    //ThreadData threadData[NUM_THREADS];
    for(int i = 0; i<NUM_THREADS; i++){
        //ThreadData myData;
        //pthread_t threadId;
        //myData.threadId = i;
        pthread_create(&threadsInfo[i],NULL, suma,NULL);
        
    }
    for(int i = 0; i<NUM_THREADS; i++){
        pthread_join(threadsInfo[i],NULL);
    }
     //donde guardas la info del hilo, 0, funcion, argumento de la funcion
    printf("El final es %d\n",saldo);
    pthread_exit(NULL); //espera a que todos los hilos terminen y luego termina el principal
    printf("NUNCA LLEGA");


}
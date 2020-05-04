#include <stdlib.h>
#include <stdio.h> 
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

#include <sys/types.h>
#include <sys/select.h>

#include <stdint.h> 
#include <pthread.h>

#include "myqueue.h"

myqueue queue;
pthread_mutex_t lock;

struct data{
    int value;
};

void* consumerMethode( void* arg ){
    struct data* dataArg = (struct data*) arg;
    int sum = 0;
    int topEntry = 0;
    while (1)
    {
        pthread_mutex_lock(&lock);
        topEntry = myqueue_pop(&queue);
        pthread_mutex_unlock(&lock);
        sum += topEntry;
        if(topEntry == 0){
            break;
        }
    }
    
    printf("Sum: %d \n",sum);
    dataArg->value = sum;
    pthread_exit(dataArg);
}

int main()
{
    pthread_t id[5];
    struct data* data[5];
    pthread_mutex_init(&lock,NULL);
    myqueue_init(&queue);

    //we want to lock this thread before creating another thread
    pthread_mutex_lock(&lock);
    for (int i = 0; i < 5; i++)
    {
        data[i] = malloc(sizeof(struct data));
        pthread_create(&id[i],NULL,&consumerMethode,data[i]);

    }

    for(int j = 0; j < 10000; j++){
        myqueue_push(&queue, 1);
    }

    for(int k = 0; k < 5; k++){
        myqueue_push(&queue, 0);
    }
    //finished inserting all elements inside the queue
    pthread_mutex_unlock(&lock);

    //main thread waits for all other threads
    for (int i = 0; i < 5; i++)
    {
        pthread_join(id[i],(void**) &data[i]);

    }

    //Printing results
    int sum = 0;
    for (int i = 0; i < 5; i++)
    {
        printf("Thread %d: %d\n",i,data[i]->value);
        sum += data[i]->value;
    }
    printf("Total Sum: %d",sum);
    

    pthread_mutex_destroy(&lock);
    return EXIT_SUCCESS;
}

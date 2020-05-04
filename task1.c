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
#define BASE_NAME "thread"

struct data{
    int nr;
};


void methodDelete(void* arg){
    char* fileName = (char*) arg;
    printf("[D] Deleting File %s \n",fileName);
    remove(fileName);
}


void* methodCreate(void* arg){
    //thread method

    struct data* dataArg = (struct data*) arg;

    //file name to str
    char number[12];
    sprintf(number, "%d", dataArg->nr);
    char* str = malloc(sizeof(BASE_NAME)+sizeof(number));
    strcat(str,BASE_NAME);
    strcat(str,number);
    strcat(str,".txt");

    pthread_cleanup_push(methodDelete,str);


    char number[12];
    sprintf(number, "%d", dataArg->nr);
    char* str = malloc(sizeof(BASE_NAME)+sizeof(number));
    strcat(str,BASE_NAME);
    strcat(str,number);
    strcat(str,".txt");

    printf("[C] Creating File %s \n",str);
    fopen(str,"w+");

    //sleep for 2 seconds, during this time the main thread coud send a cancel request.
    sleep(2);

    //otherwise we remove the cleanup method
    pthread_cleanup_pop(0);

    pthread_exit(arg);

}

int main()
{
    srand(time(0)); //new random seed each time

    pthread_t id[10];
    struct data* data[10];
    
    //create all threads
    for (int i = 0; i < 10; i++)
    {
        data[i] = malloc(sizeof(struct data));
        data[i]->nr = i;
        pthread_create(&id[i],NULL,&methodCreate,data[i]);
    }

    sleep(1);

    for (int i = 0; i < 10; i++)
    {
        if(rand()%2){
            printf("[Canc] Task %ld cancelled",id[i]);
            pthread_cancel(id[i]);
            continue;
        }
        printf("[NotCanc] Task %ld not cancelled",id[i]);
        
    }

    //main thread waits for all other threads
    for (int i = 0; i < 10; i++)
    {
        pthread_join(id[i],(void**) &data[i]);

    }
    

    return EXIT_SUCCESS;
}


#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define EAT_DURATION 500
#define NUM_PHILOSOPHER 5
#define EAT_ITERATIONS 1000
#define RIGHT_CHOPSTICK(n) (n)
#define LEFT_CHOPSTICK(n) (((n) + 1) % NUM_PHILOSOPHER)

pthread_t philosopher[NUM_PHILOSOPHER];
pthread_mutex_t chopstick[NUM_PHILOSOPHER];

void* dine(void* id) {
	int n = (int)(intptr_t)id;
    int i = 0;
	while(i < 1000)
    {
		pthread_mutex_lock(&chopstick[RIGHT_CHOPSTICK(n)]);
		if(pthread_mutex_trylock(&chopstick[LEFT_CHOPSTICK(n)]) != 0){
            pthread_mutex_unlock(&chopstick[RIGHT_CHOPSTICK(n)]);
            continue;
        }
        i++;
		usleep(EAT_DURATION);
		pthread_mutex_unlock(&chopstick[LEFT_CHOPSTICK(n)]);
		pthread_mutex_unlock(&chopstick[RIGHT_CHOPSTICK(n)]);
	}
	printf("[End %d] Philosopher %d is done eating!\n", n,n);

	return NULL;
}

int main() {
	for(int i = 0; i < NUM_PHILOSOPHER; ++i) {
		pthread_mutex_init(&chopstick[i], NULL);
	}

	for(int i = 0; i < NUM_PHILOSOPHER; ++i) {
		pthread_create(&philosopher[i], NULL, dine, (void*)(intptr_t)i);
	}

	for(int i = 0; i < NUM_PHILOSOPHER; ++i) {
		pthread_join(philosopher[i], NULL);
	}

	for(int i = 0; i < NUM_PHILOSOPHER; ++i) {
		pthread_mutex_destroy(&chopstick[i]);
	}

	return EXIT_SUCCESS;
}
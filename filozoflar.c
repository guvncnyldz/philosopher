#include<stdio.h>
#include<stdlib.h>
#include<semaphore.h>
#include<pthread.h>
#include<unistd.h>

#define PH_COUNT 5

sem_t chair;
sem_t chopstick[PH_COUNT];

void * philosopher(void *);
void initialize();
void eat(int ph_index);
void think(int ph_index);
void sit(int ph_index);

int main()
{
        initialize();
        
        int i;
        pthread_t philosophers[PH_COUNT];
        for(i=0;i<PH_COUNT;i++){
		pthread_create(&philosophers[i],NULL,philosopher,(void *) &i);
	}

        for(i=0;i<PH_COUNT;i++)
                pthread_join(philosophers[i],NULL);
}

void initialize()
{
        sem_init(&chair,0,PH_COUNT-1);
	
        int i;
	for(i=0;i<PH_COUNT;i++)
		sem_init(&chopstick[i],0,1);
}

void * philosopher(void * arg)
{
	int ph_index=*(int *)arg;

	sem_wait(&chair);
        sit(ph_index);
	sem_wait(&chopstick[ph_index]);
	sem_wait(&chopstick[(ph_index+1)%PH_COUNT]);

	eat(ph_index);
	sleep(2);

	sem_post(&chopstick[(ph_index+1)%PH_COUNT]);
	sem_post(&chopstick[ph_index]);
	sem_post(&chair);
        think(ph_index);
}

void sit(int ph_index)
{
       printf("\nPhilosopher %d has sat on chair",ph_index); 
}
void eat(int ph_index)
{
	printf("\nPhilosopher %d is eating",ph_index);
}

void think(int ph_index)
{
	printf("\nPhilosopher %d is thinking",ph_index);
}
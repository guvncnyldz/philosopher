#include<stdio.h>
#include<stdlib.h>
#include<semaphore.h>
#include<pthread.h>
#include<unistd.h>

#define PH_COUNT 5

sem_t unlocked_chair;
sem_t chopstick[PH_COUNT];

void * philosopher(void *);
void initialize();
void eat(int ph_index);
void think(int ph_index);
void take_stick(int ph_index, int stick_index, char dir[]);
void sit(int ph_index);

typedef struct
{
    int index;
    int is_special;
} philosopher_args;

int main()
{
    initialize();

    srand(time(NULL));
    int special_ph = rand() % PH_COUNT; 
    printf("\nPhilosopher %d will be special",special_ph);

    int i;
    pthread_t philosophers[PH_COUNT];
    for(i=0;i<PH_COUNT;i++){
        
        philosopher_args *philosopher_arg = malloc(sizeof(philosopher_args));
        philosopher_arg->index = i;
        if(i == special_ph)
        {
            philosopher_arg->is_special = 1;
        }
        else 
            philosopher_arg->is_special = 0;

		pthread_create(&philosophers[i],NULL,philosopher,(void *)philosopher_arg);
	    }

    for(i=0;i<PH_COUNT;i++)
        pthread_join(philosophers[i],NULL);
}

void initialize()
{
    sem_init(&unlocked_chair,0,PH_COUNT-1);
	
    int i;
	for(i=0;i<PH_COUNT;i++)
		sem_init(&chopstick[i],0,1);
}

void * philosopher(void * arg)
{
	philosopher_args philosopher_arg=*(philosopher_args *)arg;

    int left_stick = philosopher_arg.index;
    int right_stick = (philosopher_arg.index+1)%PH_COUNT;

	sem_wait(&unlocked_chair);
    sit(philosopher_arg.index);

    if(philosopher_arg.is_special==1)
    {
	    sem_wait(&chopstick[right_stick]);
        take_stick(philosopher_arg.index,right_stick,"Sağ");
	    sem_wait(&chopstick[left_stick]);
        take_stick(philosopher_arg.index,left_stick,"Sol");
    }
    else
    {
        sem_wait(&chopstick[left_stick]);
        take_stick(philosopher_arg.index,left_stick,"Sol");
        sem_wait(&chopstick[right_stick]);
        take_stick(philosopher_arg.index,right_stick,"Sağ");
    }
	
    eat(philosopher_arg.index);
	sleep(2);

	sem_post(&chopstick[(philosopher_arg.index+1)%PH_COUNT]);
	sem_post(&chopstick[philosopher_arg.index]);
	sem_post(&unlocked_chair);
    think(philosopher_arg.index);
}

void take_stick(int ph_index, int stick_index, char dir[])
{
    printf("\nPhilosopher %d has taken %d (%s) stick",ph_index,stick_index,dir); 
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
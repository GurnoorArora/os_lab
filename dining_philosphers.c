#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_PHILOSOPHERS 5

typedef struct {
    int index;
    sem_t* forkOnLeft;
    sem_t* forkOnRight;
} Philosopher;

volatile int running = 1;  // Volatile to prevent compiler optimizations for thread visibility

// Function prototypes
void* philosopher_thread(void* arg);
void dine(Philosopher* philosopher);
void dining(Philosopher* philosopher);

void* philosopher_thread(void* arg) {
    Philosopher* philosopher = (Philosopher*)arg;
    while (running) {
        sleep(rand() % 3 + 1); // Random thinking time between 1-3 seconds
        printf("Philosopher %d is hungry.\n", philosopher->index);
        dine(philosopher);
    }
    return NULL;
}

void dine(Philosopher* philosopher) {
    sem_t* fork1 = philosopher->forkOnLeft;
    sem_t* fork2 = philosopher->forkOnRight;

    while (running) {
        sem_wait(fork1);  // Pick up left fork
        if (sem_trywait(fork2) == 0) {  // Try to pick up right fork
            break;  // Both forks acquired, start eating
        }
        sem_post(fork1);  // Could not acquire both forks, release the left one
        printf("Philosopher %d swaps forks.\n", philosopher->index);
        // Swap the forks, trying the other fork first next time
        sem_t* temp = fork1;
        fork1 = fork2;
        fork2 = temp;
    }

    if (!running) {
        return;  // If the program is ending, exit without eating
    }

    dining(philosopher);  // Start eating

    // Release both forks after eating
    sem_post(fork2);
    sem_post(fork1);
}

void dining(Philosopher* philosopher) {
    printf("Philosopher %d starts eating.\n", philosopher->index);
    sleep(rand() % 3 + 1);  // Eating time is also randomized between 1-3 seconds
    printf("Philosopher %d finishes eating and leaves to think.\n", philosopher->index);
}

int main() {
    pthread_t threads[NUM_PHILOSOPHERS];
    sem_t forks[NUM_PHILOSOPHERS];
    Philosopher philosophers[NUM_PHILOSOPHERS];

    // Initialize semaphores for forks
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        sem_init(&forks[i], 0, 1);  // Each fork is available
    }

    // Initialize philosophers and their forks
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        philosophers[i].index = i;
        philosophers[i].forkOnLeft = &forks[i];
        philosophers[i].forkOnRight = &forks[(i + 1) % NUM_PHILOSOPHERS];  // Circular allocation
        pthread_create(&threads[i], NULL, philosopher_thread, &philosophers[i]);
    }

    sleep(10);  // Let the philosophers run for a while

    running = 0;  // Stop all philosophers
    printf("Now we're finishing.\n");

    // Join all threads to ensure clean termination
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Destroy the semaphores
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        sem_destroy(&forks[i]);
    }

    return 0;
}

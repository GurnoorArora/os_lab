#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

typedef struct {
    sem_t rd;
    sem_t wrt;
    int readCount;
} ReaderWriter;

ReaderWriter rw;

void* reader(void* arg) {
    while (1) {
        sem_wait(&rw.rd);
        rw.readCount++;
        if (rw.readCount == 1) {
            sem_wait(&rw.wrt);
        }
        sem_post(&rw.rd);

        printf("Reader %d is reading\n", rw.readCount);

        sem_wait(&rw.rd);
        rw.readCount--;
        if (rw.readCount == 0) {
            sem_post(&rw.wrt);
        }
        sem_post(&rw.rd);

        sleep(3);
    }
    return NULL;
}

void* writer(void* arg) {
    while (1) {
        sem_wait(&rw.wrt);
        printf("Writing data.....\n");
        printf("--------------------\n");
        sem_post(&rw.wrt);
        sleep(3);
    }
    return NULL;
}

int main() {
    pthread_t t1, t2, t3, t4, t5, t6;

    sem_init(&rw.rd, 0, 1);
    sem_init(&rw.wrt, 0, 1);
    rw.readCount = 0;

    pthread_create(&t1, NULL, reader, NULL);
    pthread_create(&t2, NULL, writer, NULL);
    pthread_create(&t3, NULL, reader, NULL);
    pthread_create(&t4, NULL, reader, NULL);
    pthread_create(&t5, NULL, reader, NULL);
    pthread_create(&t6, NULL, writer, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    pthread_join(t4, NULL);
    pthread_join(t5, NULL);
    pthread_join(t6, NULL);

    sem_destroy(&rw.rd);
    sem_destroy(&rw.wrt);

    return 0;
}
//
//  first_solution.c
//  ReadersWriters
//
//  Created by Lucas Oliveira on 29/09/23.
//

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

// MARK: Global constants
#define NUM_READERS 5
#define NUM_WRITERS 2

// MARK: Functions
void * reader_routine(void *num);
void * writer_routine(void *num);

// MARK: Variables
int readcount;

pthread_mutex_t mutex;
pthread_mutex_t w;

pthread_t readers[NUM_READERS];
pthread_t writers[NUM_WRITERS];

int main(int argc, const char * argv[]) {
    readcount = 0;
    
    pthread_mutex_init(&w, NULL);
    pthread_mutex_init(&mutex, NULL);
    
    for (int i = 0; i < NUM_READERS; i++)
        pthread_create(&readers[i], NULL, reader_routine, (void *)i);
    
    for (int i = 0; i < NUM_WRITERS; i++)
        pthread_create(&writers[i], NULL, writer_routine, (void *)i);
    
    for (int i = 0; i < NUM_READERS; i++)
        pthread_join(readers[i], NULL);
    
    for (int i = 0; i < NUM_WRITERS; i++)
        pthread_join(writers[i], NULL);
    
    return 0;
}

void * reader_routine(void *num) {
    int id = (int)num;
    
    pthread_mutex_lock(&mutex);
    readcount = readcount + 1;
    if (readcount == 1) {
        pthread_mutex_lock(&w);
    }
    
    pthread_mutex_unlock(&mutex);
    
    printf("Reader %d start reading the page\n", id);
    
    pthread_mutex_lock(&mutex);
    readcount = readcount - 1;
    
    if (readcount == 0) {
        pthread_mutex_unlock(&w);
    }
    pthread_mutex_unlock(&mutex);
    
    
    return 0;
}

void * writer_routine(void *num) {
    int id = (int)num;
    
    printf("Writer %d want to write\n", id);
    pthread_mutex_lock(&w);
    
    printf("Writer %d start writing\n", id);
    pthread_mutex_unlock(&w);
    
    printf("Writer %d stop writing\n", id);
    
    return 0;
}

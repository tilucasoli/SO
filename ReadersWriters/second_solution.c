//
//  main.c
//  ReadersWriters
//
//  Created by Lucas Oliveira on 29/09/23.
//

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

// MARK: Global constants
#define NUM_READERS 5
#define NUM_WRITERS 4

// MARK: Functions
void * reader_routine(void *num);
void * writer_routine(void *num);

// MARK: Variables
int readcount, writecount;

pthread_mutex_t mutex1, mutex2, mutex3, w, r;

pthread_t readers[NUM_READERS], writers[NUM_WRITERS];

int main(int argc, const char * argv[]) {
    readcount = 0;
    writecount = 0;
    
    pthread_mutex_init(&r, NULL);
    pthread_mutex_init(&w, NULL);
    pthread_mutex_init(&mutex1, NULL);
    pthread_mutex_init(&mutex2, NULL);
    pthread_mutex_init(&mutex3, NULL);
    
    
    
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
    while (1) {
        
        int id = (int)num;
        pthread_mutex_lock(&mutex3);
        pthread_mutex_lock(&r);
        pthread_mutex_lock(&mutex1);
        
        readcount += 1;
        if (readcount == 1) {
            pthread_mutex_lock(&w);
        }
        pthread_mutex_unlock(&mutex1);
        pthread_mutex_unlock(&r);
        pthread_mutex_unlock(&mutex3);
        
        printf("Reader is performed...\n");
        
        pthread_mutex_lock(&mutex1);
        readcount -= 1;
        if (readcount == 0) {
            pthread_mutex_unlock(&w);
        }
        pthread_mutex_unlock(&mutex1);
        
    }
    
    
    return 0;
}

void * writer_routine(void *num) {
    int id = (int)num;
    while (1) {
        pthread_mutex_lock(&mutex2);
        writecount += 1;
        if (writecount == 1) {
            pthread_mutex_lock(&r);
        }
        pthread_mutex_unlock(&mutex2);
        pthread_mutex_lock(&w);
        
        printf("Writter is performed...\n");
        
        pthread_mutex_unlock(&w);
        pthread_mutex_lock(&mutex2);
        writecount -= 1;
        if (writecount == 0) {
            pthread_mutex_unlock(&r);
        }
        pthread_mutex_unlock(&mutex2);
    }
    return 0;
}

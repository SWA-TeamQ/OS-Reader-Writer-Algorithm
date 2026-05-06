#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

int read_count = 0;
pthread_mutex_t counter_mutex;
sem_t writer_sem;
int execution_time[4]={5,8,3,6};

void* reader(void* arg){
    int id = *(int*)arg;

    pthread_mutex_lock(&counter_mutex);
    read_count++;
    if(read_count == 1){
        sem_wait(&writer_sem);
        printf("Blocking any Writing processes, Because ....\n");
    }
    pthread_mutex_unlock(&counter_mutex);

    printf("Reader %d is reading for %ds...\n", id, execution_time[id]);
    sleep(execution_time[id]);

    pthread_mutex_lock(&counter_mutex);
    read_count--;
    if(read_count == 0){
        sem_post(&writer_sem);
        printf("Notifying Writers to commit, Because .....\n");
    }
    pthread_mutex_unlock(&counter_mutex);

    printf("Reader %d has finished.\n", id);
    free(arg);
    return NULL;
}

void* writer(void* arg){
    int id= *(int*)arg;

    sem_wait(&writer_sem);

    printf("Writer %d is Writing for %ds...\n", id, execution_time[id]);
    sleep(execution_time[id]);

    sem_post(&writer_sem);
    printf("Writer %d has finished.\n", id);
    free(arg);
    return NULL;
}

int main(int argc, char* arg[]){
    pthread_t process_threads[4];
    pthread_mutex_init(&counter_mutex,NULL);
    sem_init(&writer_sem,0,1);
    printf("WE provided four process:\n three of which are readers and one is a writer \n and arrived at the same time at %ds\n", 0);
    for(int i=0;i<4;i++){
        int* id = malloc(sizeof(int));
            *id = i;
        if(i==3){
            pthread_create(&process_threads[i],NULL,writer,id);
        }
    else{
        pthread_create(&process_threads[i],NULL,reader,id);
    }
    }

    for(int i=0;i<4;i++){
        pthread_join(process_threads[i],NULL);
    }

    pthread_mutex_destroy(&counter_mutex);
    sem_destroy(&writer_sem);
    return 0;
}

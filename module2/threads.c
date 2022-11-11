#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_t ids[3];
int buf = 0;
pthread_mutex_t lock;

void *work()
{
    void *count = 0;
    while (1)
    {
        pthread_mutex_lock(&lock);

        if (buf < 15)
        {
            printf("TID: %lu, PID: %d, Buffer: %d\n", pthread_self(), getpid(), buf++);
            count++;

            pthread_mutex_unlock(&lock);
            sleep(0.01);
        }
        else
        {
            pthread_mutex_unlock(&lock);
            return (void *)count;
        }
    }
}

int main()
{
    int i;
    for (i = 0; i < 3; i++)
    {
        // printf("thread created %d\n", i);
        pthread_create(&(ids[i]), NULL, &work, NULL);
    }

    int count1;
    int count2;
    int count3;

    pthread_join(ids[0], (void **)&count1);
    pthread_join(ids[1], (void **)&count2);
    pthread_join(ids[2], (void **)&count3);

    printf("TID %lu worked on the buffer %d \033[0;32mtimes\033[0m\n", pthread_self(), count1);
    printf("TID %lu worked on the buffer %d \033[0;32mtimes\033[0m\n", pthread_self(), count2);
    printf("TID %lu worked on the buffer %d \033[0;32mtimes\033[0m\n", pthread_self(), count3);

    printf("Total buffer accesses: %d\n", buf);

    return 0;
}
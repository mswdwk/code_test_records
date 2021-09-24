#include "queue.h"
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <assert.h>

#define THREAD_NUMBER 4//开启的线程数，电脑是4核，所以用4

//sem_t queue_sem;//信号量
pthread_mutex_t mutex;//互斥锁

void *thread_push(void *arg);
void *thread_pop(void *arg);

int main()
{
    LinkQueue que;
    init_Queue(&que);

    /*初始化二进制信号量 初始值为1 代表每一次只有1个线程可以访问 
    本来更加应该用互斥量 比较贴合情景 但是不太熟 就用了信号量
    */
    //int res = sem_init(&queue_sem, 0, 1);
    //assert(res != -1);

    int i;
    pthread_t threadArr[THREAD_NUMBER];
    for (i = 0; i < THREAD_NUMBER; ++i)
    {
        pthread_create(&threadArr[i], NULL, thread_push, (void *)&que);
    }

    for (i = 0; i < THREAD_NUMBER; ++i)
    {
        pthread_join(threadArr[i], NULL);
    }

    show(&que);

    for (i = 0; i < THREAD_NUMBER; ++i)
    {
        pthread_create(&threadArr[i], NULL, thread_pop, (void *)&que);
    }

    for (i = 0; i < THREAD_NUMBER; ++i)
    {
        pthread_join(threadArr[i], NULL);
    }

    //sem_destroy(&queue_sem);

    exit(EXIT_SUCCESS);
}

void *thread_push(void *arg)
{
    printf("start push\n");
    LinkQueue * quePtr = (LinkQueue *)arg;
    int i;
    for (i = 0; i < 20; ++i)
    {
        //sem_wait(&queue_sem);
        pthread_mutex_lock(&mutex);
        push_Queue(quePtr, i);
        pthread_mutex_unlock(&mutex);
        //sem_post(&queue_sem);
    }
    printf("finish push\n");
    pthread_exit(NULL);
}

void *thread_pop(void *arg)
{
    printf("start pop\n");
    LinkQueue * quePtr = (LinkQueue *)arg;
    int tmp;
    int res;
    while (1)
    {
        //sem_wait(&queue_sem);
        pthread_mutex_lock(&mutex);
        res = pop_Queue(quePtr, &tmp);
        pthread_mutex_unlock(&mutex);
        //sem_post(&queue_sem);
        if (!res)
        {
            break;
        }
        printf("%d ", tmp);
    }
    printf("finish pop\n");
    pthread_exit(NULL);
}

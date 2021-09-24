#include "queue.h"
#include <pthread.h>
#include <unistd.h>
#include <assert.h>

#define THREAD_NUMBER 4//开启的线程数，电脑是4核，所以用4

void *thread_push(void *arg);
void *thread_pop(void *arg);

/*
初始化空队列

为了模拟线程对资源的抢占
开启4个线程 每个线程push 20个元素 0~19
等待4个线程结束
打印队列元素 验证push
开启四个线程 每个线程都对队列进行 pop操作
*/
int main()
{
    LinkQueue que;
    init_Queue(&que);

    int i;
    /*
    创造四个新线程 每个线程都执行 thread_push(&que)
    */
    pthread_t threadArr[THREAD_NUMBER];
    for (i = 0; i < THREAD_NUMBER; ++i)
    {
        pthread_create(&threadArr[i], NULL, thread_push, (void *)&que);
    }

    /*
    等待四个线程都执行完
    要不然主线程一下子就跑完了 程序就结束了
    还有就是 为了show函数 可以验证元素是不是都push进去了
    */
    for (i = 0; i < THREAD_NUMBER; ++i)
    {
        pthread_join(threadArr[i], NULL);
    }

    show(&que);

    /*
    创造四个新线程 每个线程都执行 thread_pop(&que)
    */
    for (i = 0; i < THREAD_NUMBER; ++i)
    {
        pthread_create(&threadArr[i], NULL, thread_pop, (void *)&que);
    }

    for (i = 0; i < THREAD_NUMBER; ++i)
    {
        pthread_join(threadArr[i], NULL);
    }

    exit(EXIT_SUCCESS);
}

void *thread_push(void *arg)
{
    printf("start push\n");
    LinkQueue * quePtr = (LinkQueue *)arg;
    int i;
    for (i = 0; i < 20; ++i)
    {
        cas_push(quePtr, i);
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
        res = cas_pop(quePtr, &tmp);
        if (!res)
        {
            break;
        }
        printf("%d ", tmp);
        //sleep(1);
    }
    printf("finish pop\n");
    pthread_exit(NULL);
}

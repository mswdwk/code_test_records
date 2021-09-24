#include "queue.h"

/*
初始化
为队列构建一个头结点
让front和rear都指向这个头结点
*/
void init_Queue(LinkQueue *q)
{
    q->front = q->rear = (QNode *)malloc(sizeof(QNode));
    q->front->next = NULL;
}

/*
普通的入队操作
*/
void push_Queue(LinkQueue *q, int e)
{
    QueuePtr newNode = (QueuePtr)malloc(sizeof(QNode));
    newNode->data = e;
    newNode->next = NULL;
    q->rear->next = newNode;
    q->rear = newNode;
}

/*
cas的入队操作
和普通的入队操作一样
新建节点后
要将新节点挂在队尾时需要进行cas操作
因为官方文档：The definition given in the Intel documentation allows only for the use of the types int, long, long long as well as their unsigned counterparts
只能用 int, long, long long
所以要把指针类型 QueuePtr 变成 long
用long的另一个原因就是：屏蔽32位和64位的差异 long在32位是4字节 64位是8字节
*/
void cas_push(LinkQueue *q, int e)
{
    QueuePtr newNode = (QueuePtr)malloc(sizeof(QNode));
    newNode->data = e;
    newNode->next = NULL;

    QueuePtr tmp;
    do
    {
        tmp = q->rear;
    }while (!__sync_bool_compare_and_swap((long *)(&(tmp->next)), NULL, (long)newNode));

    q->rear = newNode;
}

/*
以前的判空是 q->front == q->rear
但是这样子会增加出队的操作 当出的是最后一个元素时, q->rear需要指向 q->front
我把这一步省了 暂时没有发现有什么副作用
所以我改成了 q->front->next == NULL
*/
int is_Empty(LinkQueue *q)
{
    if (q->front->next == NULL)
    {
        return(1);
    }
    return(0);
}

/*
普通的出队操作
如果队空 返回0 也就是false
e作为接受元素的缓冲
*/
int pop_Queue(LinkQueue *q, int *e)
{
    if (is_Empty(q))
    {
        return(0);
    }
    QueuePtr tmp;
    tmp = q->front->next;
    q->front->next = tmp->next;

    *e = tmp->data;
    free(tmp);
    return(1);
}

/*
cas的出队操作
每一次都要判断这个队列是不是空
然后执行cas的出队操作：
（1）tmp = q->rear 把旧的队头存起来
（2）执行原子操作：看 旧的队头 是否等于 现在的队头 tmp == *(&(q->front)) 如果相等执行 *(&(q->front)) = tmp->next 返回true 
    否则，即执行这一步原子操作的时候，别的线程修改了队列，导致队尾指向改变了，返回false ,while(!false)回到第一步重新执行
*/
int cas_pop(LinkQueue *q, int *e)
{
    QueuePtr tmp;
    do {
        if (is_Empty(q))
        {
            return(0);
        }
        //printf("cas_pop...\n");
        tmp = q->front->next;
    } while (!__sync_bool_compare_and_swap((long *)(&(q->front->next)), (long)tmp, (long)tmp->next));

    *e = tmp->data;
    free(tmp);
    return(1);
}

/*
遍历队列 打印里面的元素 为了求证队列里面的元素
*/
void show(LinkQueue *q)
{
    printf("void show(LinkQueue *q)\n");
    QueuePtr tmp = q->front->next;
    while (tmp)
    {
        printf("%d ", tmp->data);
        tmp = tmp->next;
    }
    printf("\n");
}

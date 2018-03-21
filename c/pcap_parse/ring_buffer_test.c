/**@brief ring buffer测试程序，创建两个线程，一个生产者，一个消费者。
 * 生产者每隔1秒向buffer中投入数据，消费者每隔2秒去取数据。
 *@atuher Anker  date:2013-12-18
 * */
#include "ring_buffer.h"
#include <pthread.h>
#include <time.h>
#include <unistd.h> // function sleep header file

#include "dump_print.h"

typedef struct student_info
{
    uint64_t stu_id;
    uint32_t age;
    uint32_t score;
}student_info;
int produce_count = 0,consume_count = 0;

void print_student_info(const student_info *stu_info)
{
    assert(stu_info);
    printf("id:%lu\t",stu_info->stu_id);
    printf("age:%u\t",stu_info->age);
    printf("score:%u\n",stu_info->score);
}

student_info * get_student_info(time_t timer)
{
    student_info *stu_info = (student_info *)malloc(sizeof(student_info));
    if (!stu_info)
    {
        fprintf(stderr, "Failed to malloc memory.\n");
        return NULL;
    }
    srand(timer);
	static int i = 0;
    stu_info->stu_id =0x5670 +i ;//10000 + rand() % 9999;
    stu_info->age = 12 ;//rand() % 30;
    stu_info->score = 80 + i;//rand() % 101;
    print_student_info(stu_info);
    i++;
    return stu_info;
}

void * consumer_proc(void *arg)
{
    struct ring_buffer *ring_buf = (struct ring_buffer *)arg;
    student_info stu_info; 
    while(ring_buf != NULL)
    {
        sleep(2);
        //printf("------------------------------------------\n");
        //printf("get a student info from ring buffer.\n");
        ring_buffer_get(ring_buf, (void *)&stu_info, sizeof(student_info));
        //printf("ring buffer length: %u\n", ring_buffer_len(ring_buf));
        //print_student_info(&stu_info);
        //printf("------------------------------------------\n");
        consume_count++;
    }
    return (void *)ring_buf;
}

void * producer_proc(void *arg)
{
    time_t cur_time;
    struct ring_buffer *ring_buf = (struct ring_buffer *)arg;
    while(ring_buf != NULL)
    {
        time(&cur_time);
        srand(cur_time);
        int seed = rand() % 11111;
        //printf("******************************************\n");
        student_info *stu_info = get_student_info(cur_time + seed);
        //printf("put a student info to ring buffer.\n");
        ring_buffer_put(ring_buf, (void *)stu_info, sizeof(student_info));
        //printf("ring buffer length: %u\n", ring_buffer_len(ring_buf));
        //printf("******************************************\n");
        produce_count++;
        sleep(1);
    }
    return (void *)ring_buf;
}

pthread_t consumer_thread(void *arg)
{
    int err;
    pthread_t tid;
    err = pthread_create(&tid, NULL, consumer_proc, arg);
    if (err != 0)
    {
        fprintf(stderr, "Failed to create consumer thread.errno:%u, reason:%s\n",
            errno, strerror(errno));
        return -1;
    }
    printf("consumer thread id %zu\n",tid);
    return tid;
}

pthread_t producer_thread(void *arg)
{
    int err;
    pthread_t tid;
    err = pthread_create(&tid, NULL, producer_proc, arg);
    if (err != 0)
    {
        fprintf(stderr, "Failed to create consumer thread.errno:%u, reason:%s\n",
            errno, strerror(errno));
        return -1;
    }
    printf("producer thread id %zu\n",tid);
    return tid;
}

int main()
{
    void * buffer = NULL;
    uint32_t size = 0;
    struct ring_buffer *ring_buf = NULL;
    pthread_t consume_pid, produce_pid;

    buffer = (void *)malloc(RING_BUFFER_SIZE);
    if (!buffer)
    {
        fprintf(stderr, "Failed to malloc memory.\n");
        return -1;
    }
    size = RING_BUFFER_SIZE;
    ring_buf = ring_buffer_init(buffer, size);
    if (!ring_buf)
    {
        fprintf(stderr, "Failed to init ring buffer.\n");
        return -1;
    }
#if 0
    student_info *stu_info = get_student_info(638946124);
    ring_buffer_put(ring_buf, (void *)stu_info, sizeof(student_info));
    stu_info = get_student_info(976686464);
    ring_buffer_put(ring_buf, (void *)stu_info, sizeof(student_info));
    ring_buffer_get(ring_buf, (void *)stu_info, sizeof(student_info));
    print_student_info(stu_info);
#endif
    printf("multi thread test.......\n");
    produce_pid  = producer_thread((void*)ring_buf);
    consume_pid  = consumer_thread((void*)ring_buf);
	//consume_pid  = consumer_thread((void*)ring_buf);
    printf("Ring_buffer_len    In   Out Total_produce_number Total_consume_number\n");
    for(;;){
        printf("%15d  %4d  %4d %20d %21d\r",ring_buffer_len(ring_buf),
            ring_buf->in,ring_buf->out,produce_count,consume_count);
        fflush(stdout);
        sleep(1);
    }
    
    pthread_join(produce_pid, NULL);
    pthread_join(consume_pid, NULL);
    
    ring_buffer_free(ring_buf);
    //free(f_lock);
    return 0;
}



#include <boost/thread/thread.hpp> //包含boost头文件
#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <sched.h>
#include <sys/types.h>  
#include <sys/sysinfo.h>  
#include <unistd.h>

#define MAX_CPU_CORE_NUM 256

using namespace std;

volatile bool isRuning = true;

int get_cpu_affinity()
{
   cpu_set_t get;
   CPU_ZERO(&get);  
   if (sched_getaffinity(0, sizeof(get), &get) == -1)//获取线程CPU亲和力  
   {  
	printf("warning: cound not get thread affinity, continuing...\n");  
	return -1;
   }  
   int i;  

   for (i = 0; i < MAX_CPU_CORE_NUM ; i++)  
   {  
	if (CPU_ISSET(i, &get))//判断线程与哪个CPU有亲和力  
	{  
			// printf("this thread is running processor : %d\n",i); 
			return i; 
	}  
   }  
   return -1;
}
void func1()
{
    static int cnt1 = 0;
    while(isRuning)
    {
	int core_id = get_cpu_affinity();
	cout<<__func__<<" "<<cnt1++<<" cpu id "<<core_id<<endl;
        sleep(1);
    }
}

void func2()
{
    static int cnt2 = 0;
    while(isRuning)
    {
	int core_id = get_cpu_affinity();
	cout<<__func__<<" "<<cnt2++<<" cpu id "<<core_id<<endl;
        sleep(2);
    }
}

int main()
{
    boost::thread thread1(&func1);
    boost::thread thread2(&func2);

    system("read");
    isRuning = false;

    thread2.join();
    thread1.join();
    cout << "exit" << endl;
    return 0;
}

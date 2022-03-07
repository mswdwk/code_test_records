// https://www.cnblogs.com/gtarcoder/p/4856704.html
#include<iostream>
#include<thread>
#include<future>
#include<utility>
int main(){
    std::future<int> f1 = std::async(std::launch::async, [](){
        return 8;
    });
 
    std::cout << f1.get() << std::endl;
 
    std::future<void> f2 = std::async(std::launch::async, [](){
        std::cout << 8 << std::endl;
    });
 
    f2.wait();
 
    std::future<int> future = std::async(std::launch::async, [](){
        std::this_thread::sleep_for(std::chrono::seconds(3));
        return 8;
    });
 
    std::cout << "waiting ... " << std::endl;
    std::future_status status;
 
    do{
        status = future.wait_for(std::chrono::seconds(1));
        if (status == std::future_status::deferred){
            std::cout << "deferred..\n";
        }else if(status == std::future_status::timeout){
            std::cout << "time out...\n";
        }
        else if (status == std::future_status::ready){
            std::cout << "ready!\n";
        }
    } while (status != std::future_status::ready);
    std::cout << "result is " << future.get() << std::endl;
    return 0;
}

package org.example;

import org.junit.Test;
import org.springframework.scheduling.concurrent.ThreadPoolTaskExecutor;

import java.util.concurrent.*;

public class FutureTest {
    // https://zhuanlan.zhihu.com/p/623036273
    // CompletableFuture实现异步编排全面分析和总结
    public ThreadPoolTaskExecutor asyncThreadPoolExecutor;
    public ThreadPoolTaskExecutor asyncThreadPoolExecutor() {
        if(asyncThreadPoolExecutor != null)
            return asyncThreadPoolExecutor;

        ThreadPoolTaskExecutor asyncThreadPoolExecutor = new ThreadPoolTaskExecutor();
        asyncThreadPoolExecutor.setCorePoolSize(5);
        asyncThreadPoolExecutor.setMaxPoolSize(10);
        asyncThreadPoolExecutor.setKeepAliveSeconds(60);
        asyncThreadPoolExecutor.setQueueCapacity(2048);
        asyncThreadPoolExecutor.setWaitForTasksToCompleteOnShutdown(true);
        asyncThreadPoolExecutor.setThreadNamePrefix("customAsyncTaskExecutor-");
        asyncThreadPoolExecutor.setRejectedExecutionHandler(new ThreadPoolExecutor.CallerRunsPolicy());
        asyncThreadPoolExecutor.initialize();
        return asyncThreadPoolExecutor;
    }

    @Test
    public void multiTaskTest() {
        ThreadPoolExecutor threadPoolExecutor = new ThreadPoolExecutor(10, 10, 60L, TimeUnit.SECONDS,
                new ArrayBlockingQueue<>(10000), new ThreadPoolExecutor.CallerRunsPolicy());

        CompletableFuture[] futures = new CompletableFuture[10] ;
        for(int i =0;i < 10; i++){
            int id = i;
            CompletableFuture<String> future = CompletableFuture.supplyAsync(() -> {
                System.out.println("thread name:" + Thread.currentThread().getName() + " 执行future"+id+"开始...");
                try {
                    Thread.sleep(100);
                } catch (InterruptedException e) {
                    throw new RuntimeException(e);
                }
                return "Hello " + String.valueOf(id)+" ";
            }, asyncThreadPoolExecutor());
            futures[id]=future;
        }
        CompletableFuture.allOf(futures).thenAccept(v->{
            int j =0;
            try {
                for(int i =0;i<10;i++) {
                    String r = (String) futures[i].get();
                    System.out.println("result: "+r+", currentTimeMills: "+System.currentTimeMillis());
                }
            } catch (InterruptedException e) {
                throw new RuntimeException(e);
            } catch (ExecutionException e) {
                throw new RuntimeException(e);
            }
        }).join();
    }
}

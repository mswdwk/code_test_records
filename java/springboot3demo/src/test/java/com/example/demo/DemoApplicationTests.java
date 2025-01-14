package com.example.demo;

import org.junit.jupiter.api.Test;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.scheduling.concurrent.ThreadPoolTaskExecutor;

import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.CompletableFuture;
import java.util.concurrent.ThreadPoolExecutor;

import java.util.concurrent.TimeUnit;

/* thenRun：【执行】直接开启一个异步线程执行任务，不接收任何参数，回调方法没有返回值；
* thenApply：【提供】可以提供返回值，接收上一个任务的执行结果，作为下一个Future的入参，回调方法是有返回值的；
* thenAccept：【接收】可以接收上一个任务的执行结果，作为下一个Future的入参，回调方法是没有返回值的；
* thenCombine：【结合】可以结合不同的Future的返回值，做为下一个Future的入参，回调方法是有返回值的；
* thenCompose：【组成】将上一个Future实例结果传递给下一个实例中。
* */
@SpringBootTest
class DemoApplicationTests {

	@Test
	void contextLoads() {
	}
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


	/* Note：thenCombine() VS thenCompose()，两者之间的区别
     * thenCombine[结合] 的两个CompletableFuture没有依赖关系，且第二个CompletableFuture不需要等第一个CompletableFuture执行完成才开始。
     * thenCompose[组成] 可以两个 CompletableFuture 对象，并将前一个任务的返回结果作为下一个任务的参数，它们之间存在着先后顺序。
     * thenCombine[结合] 会在两个任务都执行完成后，把两个任务的结果合并。两个任务是并行执行的，它们之间并没有先后依赖顺序。
	 * */

	/*
	* 1) thenAccept()和thenApply()的用法实际上基本上一致，区别在于thenAccept()回调方法是没有返回值的，而thenApply()回调的带返回值的。
	* 2) thenApply()和thenAccept()请求线程池在不指定的情况下，两者的不同表现，thenApply()在不指定线程池的情况下，
	* 会沿用上一个Future指定的线程池 customAsyncTaskExecutor，而thenAccept()在不指定线程池的情况，并没有复用上一个Future设置的线程池，
	* 而是重新创建了新的线程来实现异步调用
	* */
	@Test
	public void thenCombine() {
		ThreadPoolExecutor threadPoolExecutor = new ThreadPoolExecutor(10, 10, 60L, TimeUnit.SECONDS,
				new ArrayBlockingQueue<>(10000), new ThreadPoolExecutor.CallerRunsPolicy());

		CompletableFuture<String> future1 = CompletableFuture.supplyAsync(() -> {
			System.out.println("thread name:" + Thread.currentThread().getName() + " 执行future1开始...");
            try {
                Thread.sleep(100);
            } catch (InterruptedException e) {
                throw new RuntimeException(e);
            }
            return "Hello";
		}, asyncThreadPoolExecutor());
		CompletableFuture<String> future2 = CompletableFuture.supplyAsync(() -> {
			System.out.println("thread name:" + Thread.currentThread().getName() + " 执行future2开始...");
			try {
				Thread.sleep(300);
			} catch (InterruptedException e) {
				throw new RuntimeException(e);
			}
			return "World";
		}, asyncThreadPoolExecutor());
		future1.thenCombine(future2, (result1, result2) -> {
			String result = result1 + " " + result2;
			System.out.println("thread name:" + Thread.currentThread().getName() + " 获取到future1、future2聚合结果：" + result);
			return result;
		}).thenAccept(result -> System.out.println(result)).join();
	}

	/* 先有future1，然后和future2组成一个链：future1 -> future2，
	* 然后又组合了future3，形成链：future1 -> future2 -> future3。
	* 这里有个隐藏的点：future1、future2、future3它们完全没有数据依赖关系，我们只不过是聚合了它们的结果
	* */
	@Test
	public void thenCompose() {
		CompletableFuture.supplyAsync(() -> {
			// 第一个Future实例结果
			System.out.println("thread name:" + Thread.currentThread().getName() + " 执行future1开始...");
			return "Hello";
		}, asyncThreadPoolExecutor()).thenCompose(result1 -> CompletableFuture.supplyAsync(() -> {
			// 将上一个Future实例结果传到这里
			System.out.println("thread name:" + Thread.currentThread().getName() + " 执行future2开始..., 第一个实例结果：" + result1);
			return result1 + " World";
		})).thenCompose(result12 -> CompletableFuture.supplyAsync(() -> {
			// 将第一个和第二个实例结果传到这里
			System.out.println("thread name:" + Thread.currentThread().getName() + " 执行future3开始..., 第一第二个实现聚合结果：" + result12);
			String targetResult = result12 + ", I am austin!";
			System.out.println("最终输出结果：" + targetResult);
			return targetResult;
		}));
	}

}

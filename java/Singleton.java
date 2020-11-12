//package org.vincent;

import java.util.concurrent.Callable;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;

/**
 * 单例模式实现，静态私有类实现。
 * @author  pengrong
 *
 */
public class Singleton {
    private Singleton() {
	init();	
	System.out.println("Singleton init");
    };
    public static Singleton getInstance() {
        return Holder.instance;
    }
    private static class Holder{
        private volatile static Singleton instance = new Singleton();
    }
	public static int hello(int i){
		System.out.println("hello "+i);
		return i;
	}
	public static void init (){
		System.out.println("init lib");
	}

    public static void main(String[] args) throws InterruptedException, ExecutionException { 
        ExecutorService executorService=Executors.newCachedThreadPool();
        for (int i = 0; i < 10; i++) {
            Future<Integer> future=executorService.submit(new Callable<Integer>() {

                @Override
                public Integer call() throws Exception {
                    Singleton.hello(100);
                    return Singleton.getInstance().hashCode();
                }
            });
            System.out.println("第 "+(i+1)+" 个"+future.get());;
        }
        executorService.shutdown();
    }
}  

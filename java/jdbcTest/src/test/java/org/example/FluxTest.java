package org.example;

import org.junit.Test;
import reactor.core.publisher.Flux;
import reactor.core.publisher.Mono;
import reactor.core.scheduler.Scheduler;
import reactor.core.scheduler.Schedulers;

/*  R2DBC test */
public class FluxTest {
    @Test
    public void test1() {
        Flux<Integer> flux = Flux.range(1, 10)
                .log()
                .take(5);
        flux.subscribe(System.out::println);
    }
    @Test
    public  void test2() {
        Mono<Integer> mono = Mono.just(123);
        int a = mono.block();
        System.out.println("block res "+a);
    }

    @Test
    public void test3() {
        Scheduler s = Schedulers.newParallel("parallel-scheduler", 3);
        final Flux<String> flux = Flux
                .range(1, 8)
                .map(i -> 10 + i)
                .publishOn(s)
                .map(i -> "value " + i)
                .take(7);
        flux.subscribe(System.out::println);
    }
}

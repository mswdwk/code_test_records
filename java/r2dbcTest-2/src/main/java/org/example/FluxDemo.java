package org.example;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import reactor.core.publisher.Flux;
import reactor.core.publisher.Mono;

public class FluxDemo {
    private static Logger log = LoggerFactory.getLogger(FluxDemo.class);
    public static void test1() {
        Flux<Integer> flux = Flux.range(1, 1)
                .log("test1")
                .take(3);
        flux.subscribe(a -> System.out.println(a));
    }

    public static void test2() {
        Mono<Integer> mono = Mono.just(123);
        int a = mono.block();
        System.out.println("block res "+a);
    }
}

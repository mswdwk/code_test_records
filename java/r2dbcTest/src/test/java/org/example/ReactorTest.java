package org.example;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.junit.Test;
import reactor.core.publisher.Flux;
import reactor.core.publisher.Mono;

import java.util.List;

import static org.junit.Assert.assertEquals;

public class ReactorTest {
    private static Logger log = LogManager.getLogger();
    @Test
    public void test1() {
        Flux<Integer> flux = Flux.range(1, 1)
                .log("test1")
                .take(3);
        flux.subscribe(a -> System.out.println(a));
    }

    @Test
    public void test2() {
        Mono<Integer> mono = Mono.just(123);
        int a = mono.block();
        System.out.println("block res "+a);
    }

    @Test
    public void test_mono_block(){
        Mono<Integer> mono = Mono.just(123);
        assertEquals(123, (int) mono.block());
    }

    @Test
    public void test_flux_block(){
        Flux<Integer> flux = Flux.just(123,234,345);
        assertEquals(123, (int) flux.blockFirst());
        assertEquals(345, (int) flux.blockLast());
        List<Integer> r = flux.collectList().block();
        System.out.println("test "+r);
    }
}

package org.example;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

import org.junit.Test;
import reactor.core.publisher.Flux;
import reactor.core.publisher.Mono;
import java.lang.*;
import java.util.List;

/**
 * Unit test for simple App.
 */
public class AppTest
{
    /**
     * Rigorous Test :-)
     */
    @Test
    public void shouldAnswerWithTrue()
    {
        assertTrue( true );
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
package org.example;

/**
 * Hello world!
 *
 */
import reactor.core.publisher.Flux;

public class App 
{
    private static void testFlux(){
        System.out.println( "testFlux" );
        Flux<Integer> ints;
        ints = Flux.range(1, 3);
        ints.subscribe(i -> System.out.println(i));
    }
    private static void testFlux2()
    {
        System.out.println( "testFlux 2, deal error" );
        Flux<Integer> ints = Flux.range(1, 4)
                .map(i -> {
                    if (i <= 3) return i;
                    throw new RuntimeException("Got to 4");
                });
        ints.subscribe(i -> System.out.println(i),
                error -> System.err.println("Error: " + error));
    }
    private static void testFlux3()
    {
        System.out.println( "testFlux 3, deal done,print done" );
        Flux<Integer> ints = Flux.range(1, 4);
        ints.subscribe(i -> System.out.println(i),
                error -> System.err.println("Error " + error),
                () -> System.out.println("Done"));
    }

    private static void testFlux4()
    {
        System.out.println( "testFlux 4, deal exception/done" );
        Flux<Integer> ints = Flux.range(1, 4)
                .map(i->{
                    if(i<=2 || i >3) return i;
                    throw new RuntimeException("Got to 3");
                });
        ints.subscribe(i -> System.out.println(i),
                error -> System.err.println("Error " + error),
                () -> System.out.println("Done"));
    }
    private static void testFlux5()  // want 10 datas
    {
        System.out.println( "testFlux 5, deal subscribe" );
        Flux<Integer> ints = Flux.range(1, 4);
        ints.subscribe(i -> System.out.println(i),
                error -> System.err.println("Error " + error),
                () -> System.out.println("Done"),
                sub -> sub.request(10));
    }
    private static void testMono(){
        System.out.println( "testMono" );
        Flux<Integer> ints;
        ints = Flux.range(1, 3);
        ints.subscribe(i -> System.out.println(i));
    }

    public static void main( String[] args )
    {
        System.out.println( "Hello World!" );
        testFlux();
        testFlux2();
        testFlux3();
        testFlux4();
        testFlux5();
        testMono();
    }
}

package org.example;

import reactor.core.publisher.BaseSubscriber;
import org.reactivestreams.Subscription;
//public class SampleSubscriber { }

public class SampleSubscriber<T> extends BaseSubscriber<T> {

    public void hookOnSubscribe(Subscription subscription) {
        System.out.println("Subscribed");
        request(1);
    }

    public void hookOnNext(T value) {
        System.out.println("hookOnNext:"+value);
        request(1);
    }
}
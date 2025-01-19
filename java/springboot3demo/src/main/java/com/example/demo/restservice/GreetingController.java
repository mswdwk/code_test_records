package com.example.demo.restservice;

import java.util.concurrent.atomic.AtomicLong;

import com.example.demo.model.Greeting;
import com.example.demo.model.Greeting2;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;

@RestController
public class GreetingController {

    private static final String template = "Hello, %s!";
    private final AtomicLong counter = new AtomicLong();

    @GetMapping("/greeting")
    public Greeting greeting(@RequestParam(value = "name", defaultValue = "World") String name) {
        return new Greeting(counter.incrementAndGet(), String.format(template, name));
    }

    @GetMapping("/greeting2")
    public Greeting2 greeting2(@RequestParam(value = "name", defaultValue = "World") String name) {
        return new Greeting2(counter.incrementAndGet(), String.format(template, name));
    }
}
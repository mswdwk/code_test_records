package com.example.demo.model;

import lombok.Data;

@Data
public class Greeting2 {
    public long id;
    public String content;
    public Greeting2(long id, String content){
        this.id = id;
        this.content = content;
    }
}

package edu.bistu.show.entity;

import lombok.Data;

@Data
public class Info {
    private int id;
    private String area;
    private int cumulative;
    private int cure;
    private int death;
    private int increase;
    private String createtime;
//    private String starTime;
//    private String endTime;

    public Info() {
    }
}
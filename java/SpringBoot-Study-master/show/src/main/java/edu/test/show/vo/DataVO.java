package edu.bistu.show.vo;

import lombok.Data;

import java.util.List;

@Data
public class DataVO<T> {

    private Integer code;
    private String msg;
    private Integer count;
    private List<T> data;



}

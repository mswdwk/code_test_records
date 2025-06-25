package com.example.demo.model;

import lombok.Data;

import java.util.Date;

@Data
public class OneEsIndex {
    String name;
    Date cratTmtp;
    int esDocCount;
    int dbRowCount;
}

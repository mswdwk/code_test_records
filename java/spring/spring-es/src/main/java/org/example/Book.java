package org.example;

import lombok.Data;

@Data
public class Book{
    String title;
    String description;
    String author;
    Integer year;
    String publisher;
    Float ratings;
}

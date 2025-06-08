package org.example.model;

// 假设的User类
public class User {
    private Long id;
    private String name;
    private Integer age;

    // getters and setters...

    @Override
    public String toString() {
        return "User{id=" + id + ", name='" + name + "', age=" + age + "}";
    }
}
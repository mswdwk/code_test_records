package com.example.demo.model;

import java.io.Serializable;


public class User implements Serializable {

    private static final long serialVersionUID = 8104961490512068314L;
    private int id;
    private String name;
    private String dept;
    private String phone;
    private String website;
    private String py;

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getDept() {
        return dept;
    }

    public void setDept(String dept) {
        this.dept = dept;
    }

    public String getPhone() {
        return phone;
    }

    public void setPhone(String phone) {
        this.phone = phone;
    }

    public String getWebsite() {
        return website;
    }

    public void setWebsite(String website) {
        this.website = website;
    }

    public String getPy() {
        return py;
    }

    public void setPy(String py) {
        this.py = py;
    }

    @Override
    public String toString() {
        return "User{" +
                "id=" + id +
                ", name='" + name + '\'' +
                ", dept='" + dept + '\'' +
                ", phone='" + phone + '\'' +
                ", website='" + website + '\'' +
                ", py='" + py + '\'' +
                '}';
    }

}

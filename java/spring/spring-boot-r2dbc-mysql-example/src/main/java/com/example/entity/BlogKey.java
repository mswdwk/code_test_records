package com.example.entity;

import java.io.Serializable;
import java.util.Date;

public class BlogKey implements Serializable {
    private Integer id;

    private Date crtDt;

    private static final long serialVersionUID = 1L;

    public Integer getId() {
        return id;
    }

    public void setId(Integer id) {
        this.id = id;
    }

    public Date getCrtDt() {
        return crtDt;
    }

    public void setCrtDt(Date crtDt) {
        this.crtDt = crtDt;
    }
}
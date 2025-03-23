package com.example.entity;

import java.io.Serializable;
import java.util.Date;

public class Blog extends BlogKey implements Serializable {
    private Date updDt;

    private String title;

    private String vers;

    private Integer viewed;

    private Integer forward;

    private Integer praise;

    private Integer trample;

    private Integer favorite;

    private Short delFlg;

    private String cont;

    private static final long serialVersionUID = 1L;

    public Date getUpdDt() {
        return updDt;
    }

    public void setUpdDt(Date updDt) {
        this.updDt = updDt;
    }

    public String getTitle() {
        return title;
    }

    public void setTitle(String title) {
        this.title = title == null ? null : title.trim();
    }

    public String getVers() {
        return vers;
    }

    public void setVers(String vers) {
        this.vers = vers == null ? null : vers.trim();
    }

    public Integer getViewed() {
        return viewed;
    }

    public void setViewed(Integer viewed) {
        this.viewed = viewed;
    }

    public Integer getForward() {
        return forward;
    }

    public void setForward(Integer forward) {
        this.forward = forward;
    }

    public Integer getPraise() {
        return praise;
    }

    public void setPraise(Integer praise) {
        this.praise = praise;
    }

    public Integer getTrample() {
        return trample;
    }

    public void setTrample(Integer trample) {
        this.trample = trample;
    }

    public Integer getFavorite() {
        return favorite;
    }

    public void setFavorite(Integer favorite) {
        this.favorite = favorite;
    }

    public Short getDelFlg() {
        return delFlg;
    }

    public void setDelFlg(Short delFlg) {
        this.delFlg = delFlg;
    }

    public String getCont() {
        return cont;
    }

    public void setCont(String cont) {
        this.cont = cont == null ? null : cont.trim();
    }
}
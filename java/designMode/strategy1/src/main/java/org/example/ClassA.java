package org.example;

// 类A：实现共同接口
public class ClassA implements HasPropertyC {
    private int c; // 共同属性c

    public ClassA(int c) {
        this.c = c;
    }

    @Override
    public int getC() {
        return c;
    }

    @Override
    public void setC(int c) {
        this.c = c;
    }
}
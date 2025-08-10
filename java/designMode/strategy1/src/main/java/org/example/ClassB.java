package org.example;

// 类B：实现共同接口
public class ClassB implements HasPropertyC {
    private int c; // 共同属性c

    public ClassB(int c) {
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
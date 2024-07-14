package org.example;

import org.junit.Test;

public class XaTest {

    @Test
    public void test1() {

        XaDemo x = new XaDemo();
        try {
            x.test();
        } catch (Exception e) {
            System.out.println("xatest test err:" + e.toString());
        }
    }
}
package org.example;

// 策略接口：定义修改属性c的策略
@FunctionalInterface // Java 8函数式接口，简化策略实现
public interface CModifyStrategy {
    // 入参：实现了HasPropertyC的对象（类A/类B/新增类）
    // 返回值：是否修改成功
    boolean modify(HasPropertyC obj);
}
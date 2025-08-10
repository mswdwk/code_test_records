package org.example;

// 处理器：统一处理所有实现HasPropertyC的对象
public class PropertyCHandler {
    // 执行修改：传入对象和策略
    public static boolean handle(HasPropertyC obj, CModifyStrategy strategy) {
        return strategy.modify(obj);
    }
}
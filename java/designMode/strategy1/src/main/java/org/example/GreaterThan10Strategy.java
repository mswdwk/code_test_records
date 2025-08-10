package org.example;

// 策略1：当c > 10时，将c改为0
public class GreaterThan10Strategy implements CModifyStrategy {
    @Override
    public boolean modify(HasPropertyC obj) {
        if (obj.getC() > 10) {
            obj.setC(0);
            return true;
        }
        return false;
    }
}
package org.example;

// 策略2：当c < 0时，将c改为100
public class LessThan0Strategy implements CModifyStrategy {
    @Override
    public boolean modify(HasPropertyC obj) {
        if (obj.getC() < 0) {
            obj.setC(100);
            return true;
        }
        return false;
    }
}
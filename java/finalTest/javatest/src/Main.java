public class Main {
    public static void main(String[] argv) {
        new Main().doTheWork();
    }
    public void doTheWork() {
        Object o = null;
        for (int i=0; i<5; i++) {
            try {
                o = makeObj(i);
            }
            catch (IllegalArgumentException e) {
                System.err.println
                        ("Error: ("+ e.getMessage()+").");
                return;
            }
            finally {
                System.err.println("都已执行完毕 "+i);
                if (o==null)
                    System.exit(0);
            }
            System.err.println("i= "+i+", "+o);
            //  System.out.println("i= "+i+", "+o);
        }
    }
    public Object makeObj(int type)
            throws IllegalArgumentException {
        if (type == 1)
            throw new IllegalArgumentException
                    ("不是指定的类型： " + type);
        return new Object();
    }
}
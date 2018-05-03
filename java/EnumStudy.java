public class EnumStudy {
    public static void main(String [] args){
        WeekDay weekDay=WeekDay.SUN;
        System.out.println("对象本身："+weekDay);//对象本身：SUN
        System.out.println("枚举name方法"+weekDay.name());//枚举name方法SUN
        System.out.println("枚举ordinal方法：序号"+weekDay.ordinal());//枚举ordinal方法：序号0
        System.out.println("枚举toString方法"+weekDay.toString());//枚举toString方法SUN
        //valuesOf()方法：将String转化成枚举对象
        System.out.println(WeekDay.valueOf("FRI"));
        //values()方法：返回所有的枚举值
        System.out.println(WeekDay.values().length);
        for(WeekDay wd:WeekDay.values()){
            System.out.println(wd);
        }
        //*************枚举高级应用***************\\
        System.out.println("-------------------------");
        //枚举的每个值都是一个对象，他们都有构造方法，我们可给它重构一些构造方法
        //每次第一个使用枚举值时，它会为所有枚举值都实例化一次
        System.out.println(WeekDay2.SUN);
        System.out.println(WeekDay2.TUE);
        System.out.println("-------------------------");
        //枚举的每个值都是一个对象，我我们也可以为它们添加一些属性和方法
        TranfficLamp lamp = TranfficLamp.GREEN;   
        // TranfficLamp枚举添加的特有的属性   
        System.out.println(lamp.time);   
        // TranfficLamp枚举添加的特有的方法   
        System.out.println(lamp.nextLamp());       
    }
    //普通枚举类
    public enum WeekDay{
        SUN,MON,TUE,WED,THI,FRI,SAT;
    }
    //有多个构造方法的枚举
    public enum WeekDay2{
        SUN(0),MON(1),TUE(),WED,THI,FRI,SAT;
        //所有的方法或其它必须放在枚举值的下面
        //构造器只能私有private，绝对不允许有public构造器。
        //这样可以保证外部代码无法新构造枚举类的实例。
        //这也是完全符合情理的，因为我们知道枚举值是public static final的常量而已。 
        //但枚举类的方法和数据域可以允许外部访问。
        private WeekDay2(){
            System.out.println("调用第一个构造方法");
        }
        private WeekDay2(int day){
            System.out.println("调用第二个构造方法");
        }
    }
    //有抽象方法和属性的枚举
    public enum TranfficLamp{
        RED(30){
            public TranfficLamp nextLamp(){
                return GREEN;
            }
        },
        GREEN(45) {   
            public TranfficLamp nextLamp() {   
                return YELLOW;   
            }   
        },   
        YELLOW(5) {   
            public TranfficLamp nextLamp() {   
                return RED;   
            }   
        };   
        // 该枚举有一个抽象方法   
        public abstract TranfficLamp nextLamp();   
        // 该枚举有一个属性   
        public int time;   
        // 该枚举的构造方法必须要有一个int类型参数   
        private TranfficLamp(int time) {   
            this.time = time;   
        }   
    }   
}
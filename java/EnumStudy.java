public class EnumStudy {
    public static void main(String [] args){
        WeekDay weekDay=WeekDay.SUN;
        System.out.println("������"+weekDay);//������SUN
        System.out.println("ö��name����"+weekDay.name());//ö��name����SUN
        System.out.println("ö��ordinal���������"+weekDay.ordinal());//ö��ordinal���������0
        System.out.println("ö��toString����"+weekDay.toString());//ö��toString����SUN
        //valuesOf()��������Stringת����ö�ٶ���
        System.out.println(WeekDay.valueOf("FRI"));
        //values()�������������е�ö��ֵ
        System.out.println(WeekDay.values().length);
        for(WeekDay wd:WeekDay.values()){
            System.out.println(wd);
        }
        //*************ö�ٸ߼�Ӧ��***************\\
        System.out.println("-------------------------");
        //ö�ٵ�ÿ��ֵ����һ���������Ƕ��й��췽�������ǿɸ����ع�һЩ���췽��
        //ÿ�ε�һ��ʹ��ö��ֵʱ������Ϊ����ö��ֵ��ʵ����һ��
        System.out.println(WeekDay2.SUN);
        System.out.println(WeekDay2.TUE);
        System.out.println("-------------------------");
        //ö�ٵ�ÿ��ֵ����һ������������Ҳ����Ϊ�������һЩ���Ժͷ���
        TranfficLamp lamp = TranfficLamp.GREEN;   
        // TranfficLampö����ӵ����е�����   
        System.out.println(lamp.time);   
        // TranfficLampö����ӵ����еķ���   
        System.out.println(lamp.nextLamp());       
    }
    //��ͨö����
    public enum WeekDay{
        SUN,MON,TUE,WED,THI,FRI,SAT;
    }
    //�ж�����췽����ö��
    public enum WeekDay2{
        SUN(0),MON(1),TUE(),WED,THI,FRI,SAT;
        //���еķ����������������ö��ֵ������
        //������ֻ��˽��private�����Բ�������public��������
        //�������Ա�֤�ⲿ�����޷��¹���ö�����ʵ����
        //��Ҳ����ȫ��������ģ���Ϊ����֪��ö��ֵ��public static final�ĳ������ѡ� 
        //��ö����ķ�������������������ⲿ���ʡ�
        private WeekDay2(){
            System.out.println("���õ�һ�����췽��");
        }
        private WeekDay2(int day){
            System.out.println("���õڶ������췽��");
        }
    }
    //�г��󷽷������Ե�ö��
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
        // ��ö����һ�����󷽷�   
        public abstract TranfficLamp nextLamp();   
        // ��ö����һ������   
        public int time;   
        // ��ö�ٵĹ��췽������Ҫ��һ��int���Ͳ���   
        private TranfficLamp(int time) {   
            this.time = time;   
        }   
    }   
}
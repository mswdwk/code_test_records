package com.alibaba.otter;
import kafka.javaapi.producer.Producer;
import kafka.producer.KeyedMessage;
import kafka.producer.ProducerConfig;
import kafka.serializer.StringEncoder;
import java.util.Properties;
 
public class MyKafkaProducer {
    private String topic;
    public MyKafkaProducer(String topic){
        super();
        this.topic = topic ;
    }
 
    public static void sendMsg(String topic , String sendKey , String data){
        Producer producer = createProducer();
        producer.send(new KeyedMessage<String, String>(topic,sendKey,data));
    }
 
    public static Producer<Integer,String> createProducer(){
        Properties properties = new Properties();
        properties.put("zookeeper.connect" , "localhost:2181");
        properties.put("metadata.broker.list" , "localhost:9092");
        properties.put("serializer.class" , StringEncoder.class.getName());
        final ProducerConfig producerConfig = new ProducerConfig(properties);
        return new Producer<Integer,String>(producerConfig);
    }
}

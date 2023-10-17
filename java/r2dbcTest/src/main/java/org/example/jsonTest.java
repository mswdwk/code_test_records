package org.example;

import com.alibaba.fastjson.JSON;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

public class jsonTest {
    public static void objToJsonString(){
        HashMap<String,Object> job = new HashMap<>();
        HashMap<String ,Object> job2= new HashMap<>();
        job.put("key1",1);
        job.put("key2","12");
        job.put("key3","_abc123");
        job2.put("key",1);
        job2.put("key2",1.0);
        job2.put("key3","key3");
        job.put("key4",job2);

        List<Object> job3 = new ArrayList<>();
        job3.add("123");
        job3.add("bac_");
        job3.add(123.00);
        job.put("job3",job3);

        //复杂java类转json字符串

        String userGroupJson = JSON.toJSONString(job);
        System.out.println("json="+userGroupJson);

        String jsonStr1 = "{'password':'123456','username':'dmego'}";
        User user = JSON.parseObject(jsonStr1, User.class);
        System.out.println("json字符串转简单java对象:"+user.toString());
    }
}
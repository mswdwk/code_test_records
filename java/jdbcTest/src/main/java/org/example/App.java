package org.example;

import com.alibaba.fastjson.JSONObject;
import org.apache.http.HttpEntity;
import org.apache.http.HttpHost;
import org.apache.http.entity.ContentType;
import org.apache.http.entity.StringEntity;
import org.apache.http.nio.entity.NStringEntity;
import org.apache.http.util.EntityUtils;
import org.elasticsearch.action.search.SearchRequest;
import org.elasticsearch.client.RestClient;
import org.elasticsearch.client.RestHighLevelClient;
import org.elasticsearch.script.ScriptType;
import org.elasticsearch.script.mustache.SearchTemplateRequest;

import java.sql.*;
import java.time.Duration;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import com.alibaba.fastjson.JSON;
import reactor.core.publisher.Flux;
import reactor.core.publisher.Mono;
import reactor.core.scheduler.Scheduler;
import reactor.core.scheduler.Schedulers;

/**
 * Hello world!
 *
 */
public class App 
{
    public static void es() throws  Exception{
        RestClient restClient = RestClient.builder(
                new HttpHost("localhost", 9200, "http"),
                new HttpHost("localhost", 9201, "http")).build();
        String index = "";
        // restClient.performRequest();
        // HttpEntity entity = new StringEntity("\"这一个字符串实体", "UTF-8");
        HttpEntity entity = new NStringEntity("\"hello\":1", ContentType.APPLICATION_JSON);
        //内容类型
        System.out.println(entity.getContentType());
        //内容的编码格式
        System.out.println(entity.getContentEncoding());
        //内容的长度
        System.out.println(entity.getContentLength());
        //把内容转成字符串
        System.out.println(EntityUtils.toString(entity));
        //内容转成字节数组
        System.out.println(EntityUtils.toByteArray(entity).length);

    }
    public static void high_level(){
        RestHighLevelClient client = new RestHighLevelClient(
                RestClient.builder(
                        new HttpHost("localhost", 9200, "http"),
                        new HttpHost("localhost", 9201, "http")));


        SearchTemplateRequest request = new SearchTemplateRequest();
        request.setRequest(new SearchRequest("posts"));

        request.setScriptType(ScriptType.INLINE);
        request.setScript(
                "{" +
                        "  \"query\": { \"match\" : { \"{{field}}\" : \"{{value}}\" } }," +
                        "  \"size\" : \"{{size}}\"" +
                        "}");

        Map<String, Object> scriptParams = new HashMap<>();
        scriptParams.put("field", "title");
        scriptParams.put("value", "elasticsearch");
        scriptParams.put("size", 5);
        request.setScriptParams(scriptParams);

        System.out.println("query= "+request.getScriptParams());
    }

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
        // User user = JSON.parseObject(jsonStr1, User.class);
        // System.out.println("json字符串转简单java对象:"+user.toString());
    }

    public static void main( String[] args ) {
        // objToJsonString()

        Scheduler s = Schedulers.newParallel("parallel-scheduler", 4);

        final Flux<String> flux = Flux
                .range(1, 2)
                .map(i -> 10 + i)
                .subscribeOn(s)
                .map(i -> "value " + i);
        flux.subscribe(System.out::println);

        Dbconnect dbconnect = new Dbconnect();
        dbconnect.test_r2dbc_mysql();
//        try {
//            high_level();
//           // es();
//        }catch (Exception e){
//            System.out.println(e.toString());
//        }

    }


}

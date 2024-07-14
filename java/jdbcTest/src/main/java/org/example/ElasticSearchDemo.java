package org.example;

import org.apache.http.HttpEntity;
import org.apache.http.HttpHost;
import org.apache.http.entity.ContentType;
import org.apache.http.nio.entity.NStringEntity;
import org.apache.http.util.EntityUtils;
import org.elasticsearch.action.search.SearchRequest;
import org.elasticsearch.client.RestClient;
import org.elasticsearch.client.RestHighLevelClient;
import org.elasticsearch.script.ScriptType;
import org.elasticsearch.script.mustache.SearchTemplateRequest;

import java.util.HashMap;
import java.util.Map;

public class ElasticSearchDemo {
    public static void es() throws Exception {
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

    public static void high_level() {
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

        System.out.println("query= " + request.getScriptParams());
    }

    /* ES test*/
    public void es_test() {
        try
        {
            high_level();
            // es();
        }catch(Exception e)
        {
            System.out.println(e.toString());
        }
    }
}

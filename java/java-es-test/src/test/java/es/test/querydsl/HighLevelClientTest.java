package es.test.querydsl;

import org.apache.http.HttpHost;
import org.elasticsearch.action.search.SearchRequest;
import org.elasticsearch.action.search.SearchResponse;
import org.elasticsearch.client.RequestOptions;
import org.elasticsearch.client.RestClient;
import org.elasticsearch.client.RestClientBuilder;
import org.elasticsearch.client.RestHighLevelClient;
import org.elasticsearch.index.query.QueryBuilders;
import org.elasticsearch.search.builder.SearchSourceBuilder;
import org.elasticsearch.search.collapse.CollapseBuilder;
import org.junit.Test;

public class HighLevelClientTest {
    @Test
    public void t2(){
        // 初始化Elasticsearch客户端
        try  {
            RestHighLevelClient client = new RestHighLevelClient(
                    RestClient.builder(
                            new HttpHost("localhost", 9200, "http")
                            ));
            // 创建查询请求并设置索引
            SearchRequest searchRequest = new SearchRequest("test");

            // 构建查询条件
            SearchSourceBuilder searchSourceBuilder = new SearchSourceBuilder();
            searchSourceBuilder.query(QueryBuilders.matchAllQuery()); // 使用所有文档匹配查询作为示例

            // 设置分组字段并去重
            CollapseBuilder collapseBuilder = new CollapseBuilder("price"); // "group_field" 是你想根据其进行去重的字段
            searchSourceBuilder.collapse(collapseBuilder);

            // 设置排序
            searchSourceBuilder.sort("_id"); // 根据_id排序确保稳定性

            // 将查询条件放入请求
            searchRequest.source(searchSourceBuilder);
            System.out.println("search: "+searchSourceBuilder.toString());

            // 执行查询
            SearchResponse searchResponse = client.search(searchRequest, RequestOptions.DEFAULT);
            System.out.println("resp: "+searchResponse.toString());
            // 处理查询结果
            // ...
        }catch (Exception e) {
            System.out.println("error: "+e);
        }
    }
}

package es.test.querydsl;

import es.test.BaseTest;
import org.apache.http.HttpHost;
import org.elasticsearch.action.search.SearchRequest;
import org.elasticsearch.action.search.SearchResponse;
import org.elasticsearch.client.RequestOptions;
import org.elasticsearch.client.RestClient;
import org.elasticsearch.client.RestClientBuilder;
import org.elasticsearch.client.RestHighLevelClient;
import org.elasticsearch.client.indices.CreateIndexRequest;
import org.elasticsearch.index.query.QueryBuilders;
import org.elasticsearch.search.SearchHit;
import org.elasticsearch.search.SearchHits;
import org.elasticsearch.search.aggregations.Aggregation;
import org.elasticsearch.search.aggregations.AggregationBuilder;
import org.elasticsearch.search.aggregations.bucket.terms.IncludeExclude;
import org.elasticsearch.search.aggregations.metrics.ParsedTopHits;
import org.elasticsearch.search.builder.SearchSourceBuilder;
import org.elasticsearch.search.collapse.CollapseBuilder;

import org.elasticsearch.search.aggregations.AggregationBuilders;
import org.elasticsearch.search.aggregations.Aggregations;
import org.elasticsearch.search.aggregations.bucket.terms.Terms;

import org.elasticsearch.xcontent.XContentType;
import org.junit.Before;
import org.junit.Test;

import java.io.IOException;
import java.util.Arrays;

public class HighLevelClientTest extends BaseTest {
    private RestHighLevelClient client;

    @Before
    public void setUp() throws IOException {
        RestClientBuilder builder = RestClient.builder(new HttpHost("localhost", 9200, "http"));
        client = new RestHighLevelClient(builder);
    }

    @Test
    public void createIndex() throws  Exception {
        CreateIndexRequest req = new CreateIndexRequest(indexName);
        req.source(indexMapping, XContentType.JSON);
        client.indices().create(req,RequestOptions.DEFAULT);
    }

    @Test
    public void t2(){
        // 初始化Elasticsearch客户端
        try  {
            RestHighLevelClient client = new RestHighLevelClient(
                    RestClient.builder(
                            new HttpHost("localhost", 9200, "http")
                            ));
            // 创建查询请求并设置索引
            SearchRequest searchRequest = new SearchRequest(indexName);

            // 构建查询条件
            SearchSourceBuilder searchSourceBuilder = new SearchSourceBuilder();
            searchSourceBuilder.query(QueryBuilders.matchAllQuery()); // 使用所有文档匹配查询作为示例

            // 设置分组字段并去重
            CollapseBuilder collapseBuilder = new CollapseBuilder("city"); // "group_field" 是你想根据其进行去重的字段

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

    @Test
    public  void  t3() throws IOException {
        // 初始化Elasticsearch客户端

        // 创建查询请求并设置索引
        SearchRequest searchRequest = new SearchRequest(indexName);

        // 构建查询条件
        SearchSourceBuilder searchSourceBuilder = new SearchSourceBuilder();
        searchSourceBuilder.query(QueryBuilders.matchAllQuery()); // 使用所有文档进行测试

        String includes[] = {"price","product","city","province"};
        IncludeExclude includeExclude = new IncludeExclude(includes,null);

        AggregationBuilder aggregationBuilder = AggregationBuilders
                .terms("group_by_city").field("city")
                .subAggregation(AggregationBuilders.topHits("top_1").size(1).fetchSource(includes,null));
        // 添加分组聚合
        searchSourceBuilder.aggregation(aggregationBuilder);

        searchRequest.source(searchSourceBuilder);

        // 执行查询
        SearchResponse searchResponse = null;

        searchResponse = client.search(searchRequest, RequestOptions.DEFAULT);


        // 获取分组结果
        Aggregations aggregations = searchResponse.getAggregations();
        Terms groupAggregation = aggregations.get("group_by_city");

        for (Terms.Bucket bucket : groupAggregation.getBuckets()) {
            String key = bucket.getKeyAsString(); // 分组的键
            Aggregation p = bucket.getAggregations().get("top_1");
            ParsedTopHits tph = (ParsedTopHits)p;
            SearchHits shts = tph.getHits();
            SearchHit[] hitArry = shts.getHits();
            Arrays.stream(hitArry).forEach(
                    hit -> {
                        System.out.println("source: "+hit.getSourceAsString());
                    }
            );


            long docCount = bucket.getDocCount(); // 分组中的文档数量
            System.out.println(key + " : " + docCount+", p: "+p.toString());
        }

        // 关闭客户端
        client.close();
    }
}

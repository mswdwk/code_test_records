package es.test.querydsl;

import es.test.BaseTest;
import lombok.extern.slf4j.Slf4j;
import org.elasticsearch.action.admin.indices.delete.DeleteIndexRequest;
import org.elasticsearch.action.delete.DeleteRequest;
import org.elasticsearch.action.index.IndexRequest;
import org.elasticsearch.action.search.SearchRequest;
import org.elasticsearch.action.search.SearchResponse;
import org.elasticsearch.client.RestHighLevelClient;
import org.elasticsearch.client.indices.CreateIndexRequest;
import org.elasticsearch.index.query.ConstantScoreQueryBuilder;
import org.elasticsearch.index.query.QueryBuilders;
import org.elasticsearch.index.reindex.DeleteByQueryAction;
import org.elasticsearch.search.builder.SearchSourceBuilder;
import org.junit.After;
import org.junit.Assert;
import org.junit.Before;
import org.junit.Test;

import java.io.IOException;
import java.util.HashMap;

// import static org.elasticsearch.common.xcontent.XContentFactory.jsonBuilder;
import static org.elasticsearch.index.query.QueryBuilders.constantScoreQuery;
import static org.elasticsearch.index.query.QueryBuilders.termQuery;
import static org.elasticsearch.xcontent.XContentFactory.jsonBuilder;

@Slf4j
public class CompoundQueryTest extends BaseTest {

  @Before
  public void setUp() throws IOException {

    /*CreateIndexRequest req = new CreateIndexRequest(indexName);
    client.admin().indices().prepareCreate(indexName);*/


    //初始化测试数据
    bulkProcessor.add(new IndexRequest(indexName ).source(jsonBuilder().startObject().field("name", "王百万").endObject()));
    bulkProcessor.add(new IndexRequest(indexName).source(jsonBuilder().startObject().field("name", "王大力").endObject()));
    bulkProcessor.add(new IndexRequest(indexName).source(jsonBuilder().startObject().field("name", "赵海洋").endObject()));
    bulkProcessor.add(new IndexRequest(indexName).source(jsonBuilder().startObject().field("name", "丁建国").endObject()));
    bulkProcessor.flush();
    bulkProcessor.close();
    client.admin().indices().prepareRefresh().get();

  }

  @After
  public void tearDown() {
    //删除测试数据
    /*DeleteByQueryAction.INSTANCE.newRequestBuilder(client)
            .filter(QueryBuilders.matchAllQuery())
            .source("test")
            .get();*/
    DeleteIndexRequest req = new DeleteIndexRequest( );
    req.indices(indexName);
    client.admin().indices().delete(req);
  }

  @Test
  public void constantScore() {
    ConstantScoreQueryBuilder builder =
            constantScoreQuery(
                    termQuery("name", "王百万"))
                    .boost(1.0f);
    // log.info(builder.toString());
    SearchResponse searchResponse = client.prepareSearch().setQuery(builder).get();
    // log.info(searchResponse.toString());
    //Assert.assertEquals(1, searchResponse.getHits().getTotalHits());
  }


}

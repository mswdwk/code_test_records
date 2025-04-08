package es.test;

import org.elasticsearch.action.admin.indices.create.CreateIndexRequest;
import org.elasticsearch.action.admin.indices.delete.DeleteIndexRequest;
import org.elasticsearch.action.admin.indices.exists.indices.IndicesExistsRequest;
import org.elasticsearch.action.bulk.BulkProcessor;
import org.elasticsearch.action.bulk.BulkRequest;
import org.elasticsearch.action.bulk.BulkResponse;
import org.elasticsearch.client.Client;
import org.elasticsearch.client.transport.TransportClient;
import org.elasticsearch.common.settings.Settings;
import org.elasticsearch.common.transport.TransportAddress;
import org.elasticsearch.transport.client.PreBuiltTransportClient;
import org.elasticsearch.xcontent.XContentType;

import java.net.InetAddress;
import java.net.UnknownHostException;

/**
 * 提取client和bulkProcessor到公共类，避免每个测试类都要初始化客户端。
 * 因为每个类所测试的数据不一定相同，所以测试数据需要在单独的测试类中初始化。
 */
public class BaseTest {

    protected Client client;
    protected BulkProcessor bulkProcessor;
    public String indexName = "idx_test_1";
    public String indexMapping = """
            {
             "settings": {
                     "number_of_shards": 3,
                     "number_of_replicas": 1
              },
              "mappings": {
                "properties": {
                  "province": {
                    "type": "keyword"
                  },
                  "city": {
                    "type": "keyword"
                  },
                  "company": {
                    "type": "keyword"
                  },
                  "product": {
                    "type": "keyword"
                  },
                  "amount": {
                    "type": "long"
                  },
                  "price": {
                    "type": "double"
                  },
                  "description": {
                    "type": "text"
                  }
                }
              }
            }
           """;

    public BaseTest() {
        //初始化客户端
        Settings settings = Settings.builder()
                .put("cluster.name", "elasticsearch").build();
        TransportClient client = null;
        try {
            client =
                    //默认集群名就是elasticsearch，所以也可以传入默认参数Settings.EMPTY
                    new PreBuiltTransportClient(settings)
                            .addTransportAddress(new TransportAddress(InetAddress.getByName("localhost"), 9300));
        } catch (UnknownHostException e) {
            e.printStackTrace();
        }
        this.client = client;

        assert client != null;
        this.bulkProcessor = BulkProcessor.builder(client, new BulkProcessor.Listener() {
                    public void beforeBulk(long l, BulkRequest bulkRequest) {
                        //批处理之前会经过这里
                    }

                    public void afterBulk(long l, BulkRequest bulkRequest, BulkResponse bulkResponse) {
                        //批处理之后会经过这里
                    }

                    public void afterBulk(long l, BulkRequest bulkRequest, Throwable throwable) {
                        //批处理出错会经过这里
                    }
                })
                //由于单元测试不支持多线程操作，所以需要设置并发数为1
                .setConcurrentRequests(0)
                .build();

        this.createEsIndex();
    }

    public void createEsIndex() {
        try {
            CreateIndexRequest req = new CreateIndexRequest(indexName);
            // org.elasticsearch.action.admin.indices.create.CreateIndexRequest req2 = new org.elasticsearch.action.admin.indices.create.CreateIndexRequest(indexName);
            req.source(indexMapping, XContentType.JSON);
            IndicesExistsRequest existsRequest = new IndicesExistsRequest(indexName);
            boolean r = false;
            if (client.admin().indices().exists(existsRequest).actionGet().isExists()) {
                System.out.println("delete exist index : "+indexName);
                 r = client.admin().indices().delete(new DeleteIndexRequest(indexName)).actionGet().isAcknowledged();
            }

             r = client.admin().indices().create(req).actionGet().isAcknowledged();
            System.out.println("create es index : "+indexName);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }


}

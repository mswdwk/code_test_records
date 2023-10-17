package org.example;

import dev.miku.r2dbc.mysql.MySqlConnection;
import io.r2dbc.spi.*;
import org.reactivestreams.Publisher;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import reactor.core.publisher.Flux;
import reactor.core.publisher.Mono;

import java.sql.*;
import java.sql.Connection;
import java.sql.Statement;
import java.time.Duration;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.stream.Collectors;
import java.util.stream.IntStream;

import static io.r2dbc.spi.ConnectionFactoryOptions.*;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertThat;
import static reactor.core.publisher.Signal.complete;

public class Dbconnect {
    private String driverName = "mysql";
    private String host="";
    private int port = 0;
    private String user="";
    private String password="";
    private Duration connect_timeout =  Duration.ofSeconds(3);
    private String database = "testdb";
    private static Logger log = LoggerFactory.getLogger(Dbconnect.class);
    Dbconnect(String[] args) throws Exception {
        if( args.length < 4) {
            Exception e = new Exception("arguments less than 4!");
            // e.fillInStackTrace();
            throw e;
        }
        this.host = args[0];
        this.port = Integer.decode(args[1]).intValue();
        this.user = args[2];
        this.password = args[3];
    }
    Dbconnect(MyConfig myConfig) throws Exception {
        this.host = myConfig.host;
        this.port = myConfig.port;
        this.user = myConfig.user;
        this.password = myConfig.password;
    }
    Dbconnect(String host,String user,String password, int port){
        this.host = host;
        this.port = port;
        this.user = user;
        this.password = password;
    }
    public ConnectionFactoryOptions create_option(){
        ConnectionFactoryOptions options = ConnectionFactoryOptions.builder()
                .option(DRIVER, driverName)
                .option(HOST, host)
                .option(USER, user)
                .option(PORT, port)  // optional, default 3306
                .option(PASSWORD, password) // optional, default null, null means has no password
                .option(DATABASE, database) // optional, default null, null means not specifying the database
                .option(CONNECT_TIMEOUT,connect_timeout) // optional, default null, null means no timeout
                .option(SSL, false) // optional, default sslMode is "preferred", it will be ignore if sslMode is set
                /*.option(Option.valueOf("sslMode"), "verify_identity") // optional, default "preferred"
                .option(Option.valueOf("sslCa"), "/path/to/mysql/ca.pem") // required when sslMode is verify_ca or verify_identity, default null, null means has no server CA cert
                .option(Option.valueOf("sslCert"), "/path/to/mysql/client-cert.pem") // optional, default null, null means has no client cert
                .option(Option.valueOf("sslKey"), "/path/to/mysql/client-key.pem") // optional, default null, null means has no client key
                .option(Option.valueOf("sslKeyPassword"), "key-pem-password-in-here") // optional, default null, null means has no password for client key (i.e. "sslKey")
                .option(Option.valueOf("tlsVersion"), "TLSv1.3,TLSv1.2,TLSv1.1") // optional, default is auto-selected by the server
                .option(Option.valueOf("sslHostnameVerifier"), "com.example.demo.MyVerifier") // optional, default is null, null means use standard verifier
                .option(Option.valueOf("sslContextBuilderCustomizer"), "com.example.demo.MyCustomizer") // optional, default is no-op customizer
                .option(Option.valueOf("zeroDate"), "use_null") // optional, default "use_null"
                .option(Option.valueOf("useServerPrepareStatement"), true) // optional, default false
                .option(Option.valueOf("tcpKeepAlive"), true) // optional, default false
                .option(Option.valueOf("tcpNoDelay"), true) // optional, default false
                .option(Option.valueOf("autodetectExtensions"), false) // optional, default false
                */.build();
                System.out.println("create db option success");
                return options;
    }
    public void test_r2dbc_create_database(){
        ConnectionFactory connectionFactory = ConnectionFactories.get(create_option());
        System.out.println("get connection factory "+connectionFactory);
        Mono.from(connectionFactory.create())
                .doOnError(e->System.err.println("got connection error: "+e.toString()))
                .doOnSuccess(c -> System.out.println("get connection success"))
                .flatMapMany(connection -> connection
                        .createStatement("create database if not exists testdb ")
                        // .bind("$1", 100)
                        .execute())
                .doOnError(e -> System.err.println("create database error: "+e.toString()))
                .flatMap(result -> result.getRowsUpdated())
                .doOnNext( a -> System.out.println("OnNext: "+a))
                .subscribe( rowUpdated -> System.out.println("getRowUpdated= " +rowUpdated.intValue()),
                        error -> {
                            System.err.println("got error 3: " + error);
                            error.printStackTrace();},
                        () -> System.out.println("Done"));
    }

    public void test_r2dbc_create_database_table(){
        ArrayList<String> Sqls = new ArrayList(Arrays.asList("drop database if exists newdb1",
                "create database if not exists newdb1","use newdb1",
                "create table t1(id int primary key,name varchar(128)) engined=innodb charset=utf8mb4"));
        Sqls.forEach(sql -> System.out.println(sql));
        ConnectionFactory connectionFactory = ConnectionFactories.get(create_option());
        io.r2dbc.spi.Connection connection = Mono.from(connectionFactory.create()).block();

        String tdl = "CREATE TEMPORARY TABLE test(id INT PRIMARY KEY AUTO_INCREMENT," +
                "email VARCHAR(190),password VARCHAR(190),updated_at DATETIME,created_at DATETIME)";

        Flux.from(connection.createStatement(tdl).execute())
                .flatMap(result -> result.getRowsUpdated())
                .doOnNext( r -> log.info("create table update rows="+r))
                .thenMany(Flux.range(0, 10))
                .flatMap(it -> Flux.from(connection.createStatement("INSERT INTO test VALUES(DEFAULT,?,?,NOW(),NOW())")
                        .bind(0, String.format("integration-test%d@mail.com", it))
                        .bind(1, "* "+it.toString())
                        .execute()))
                .flatMap(Result::getRowsUpdated)
                .reduce(Math::addExact)
                .doOnNext(it -> assertEquals(it.longValue(),10))
                .then(Mono.from(connection.createStatement("SELECT email FROM test").execute()))
                .flatMapMany(result -> result.map((row, metadata) -> row.get(0, String.class)))
                .collectList()
                .doOnNext(it -> assertEquals(it,IntStream.range(0, 10)
                        .mapToObj(i -> String.format("integration-test%d@mail.com", i))
                        .collect(Collectors.toList())))
                .subscribe(r -> log.info("result:"+r),e -> log.error("final error:"+e),() -> log.info("Done"))
        ;
    }

    public void test_r2dbc_connect(){
        //ConnectionFactory factory = ConnectionFactories.get(options);
        ConnectionFactory connectionFactory = ConnectionFactories.get(create_option());
        /*Mono.from(connectionFactory.create())
                .flatMapMany(connection -> {
                    System.out.println("get connection:"+connection);
                    Publisher<? extends Result> res = connection
                            .createStatement("SELECT * FROM mysql.user ")
                            // .bind("$1", 42)
                            .execute();
                    return res;
                })
                .flatMap(result -> result
                        .map((row, rowMetadata) -> row.get("user", String.class)))
                .doOnNext(System.out::println)
                .subscribe();*/
        System.out.println("connectionFactory meta Name: "+connectionFactory.getMetadata().getName());
        Mono<? extends io.r2dbc.spi.Connection> conn = Mono.from(connectionFactory.create());
        System.out.println("start map conn:"+conn.toString());
        conn.doOnError(e->System.err.println("got connection error: "+e.toString()))
                .flatMapMany(connection -> {
                    System.out.println("get connection:" + connection);
                    Publisher<? extends Result> res = connection
                            .createStatement("SELECT * FROM mysql.user ")
                            .execute();
                    return res;
                })
                .flatMap(result -> result
                        .map((row, rowMetadata) -> row.get("user", String.class)))
                .doOnNext(System.out::println)
                .subscribe(i -> System.out.println("this is:" + i));
    }
    public void test_r2dbc_mysql(){
        ConnectionFactory connectionFactory = ConnectionFactories.get(create_option());
        System.out.println("get connection factory "+connectionFactory);
        Mono.from(connectionFactory.create())
                .doOnError(e->System.err.println("got connection error: "+e.toString()))
                .doOnSuccess(c -> System.out.println("get connection success"))
                .flatMapMany(connection -> connection
                        .createStatement("insert into testdb.t1 value() ")
                        // .bind("$1", 100)
                        .execute())
                .doOnError(e -> System.err.println("select error: "+e.toString()))
                .flatMap(result -> result
                        .map((row, rowMetadata) -> {
                            Integer c1 = row.get("id",Integer.class);
                            // Long c1 =  (Long)row.get("id");
                            System.out.println("id = " +c1);
                            log.info("id : " + c1.intValue());
                            log.debug("debug");
                            return c1;
                        }))
                .doOnNext( a -> System.out.println("OnNext: "+a))
                .subscribe( i -> System.err.println("i= " +i.intValue()),
                        error -> {
                                System.err.println("got error 3: " + error);
                                error.printStackTrace();},
                        () -> System.out.println("Done"));

        /*Uni.createFrom().publisher(connectionFactory.create())
                .onItem().transformToMulti(connection -> connection
                .createStatement("SELECT firstname FROM PERSON WHERE age > $1")
                .bind("$1", 42)
                .execute())
                .onItem().transform(result -> result
                .map((row, rowMetadata) -> row.get("firstname", String.class)))
                .subscribe().with(System.out::println);
        */

    }

    public void test_r2dbc_mysql_2(){
        // Creating a Mono using Project Reactor
        ConnectionFactory connectionFactory = ConnectionFactories.get(create_option());
        io.r2dbc.spi.Connection conn = Mono.from(connectionFactory.create()).block();

        conn
                .createStatement("SELECT user,host FROM mysql.user WHERE user = $1")
            .bind("$1", "root")
            .execute();
           // .subscribe();
//        result.map(
//                    (row, rowMetadata) -> {
//                        Integer c1 = row.get("c1", Integer.class);
//                        System.err.println("c1 = " +c1);
//                        log.info("c1 : " + c1);
//                        log.debug("debug");
//                        return c1;
//                    );

    }

    public void test_2(){


        ConnectionFactory connectionFactory = ConnectionFactories
                .get("r2dbc:h2:mem:///testdb");
/*
        Uni.createFrom().publisher(connectionFactory.create())
                .onItem().transformToMulti(connection -> connection
                .createStatement("SELECT firstname FROM PERSON WHERE age > $1")
                .bind("$1", 42)
                .execute())
                .onItem().transform(result -> result
                .map((row, rowMetadata) -> row.get("firstname", String.class)))
                .subscribe().with(System.out::println);
*/
    }

    public  void test_for_jdbc_login_timeout() {
        try {
            Class.forName("com.mysql.jdbc.Driver");
            System.out.println("数据库驱动加载成功");
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        }
        try {
            DriverManager.setLoginTimeout(1);
            Connection con = DriverManager.getConnection("jdbc:mysql://192.168.79.133:3310/mysql?characterEncoding=UTF-8&connectTimeout=3000", "root", "123");
            // Connection con = DriverManager.getConnection("jdbc:mysql://192.168.79.132:3306?characterEncoding=UTF-8","user","123");
            // stmt = con.createStatement();
            PreparedStatement stmt = con.prepareStatement("select now()");
            stmt = con.prepareStatement("select sleep(1)");
            ResultSet rs = stmt.executeQuery();
            //con.close();
            rs = stmt.executeQuery();
            System.out.println("1 sleep over.");
            Statement st = con.createStatement();
            long cur_ns = System.nanoTime();
            st.execute("select sleep(1)");
            long cost_ms = (System.nanoTime() - cur_ns) / 1000 / 1000;
            System.out.println("cost_ms= " + cost_ms);
            st.close();
            con.close();
            System.out.println("数据库连接成功");
        } catch (SQLException e) {
            System.out.println("error:" + e.toString());
            e.printStackTrace();
        }
    }
}

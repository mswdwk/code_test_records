package org.example;

import io.r2dbc.spi.*;
import org.reactivestreams.Publisher;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import reactor.core.publisher.Mono;

import java.sql.*;
import java.sql.Connection;
import java.sql.Statement;
import java.time.Duration;

import static io.r2dbc.spi.ConnectionFactoryOptions.*;

public class Dbconnect {
    private static Logger log = LoggerFactory.getLogger(Dbconnect.class);
    public static ConnectionFactoryOptions create_option(){
        ConnectionFactoryOptions options = ConnectionFactoryOptions.builder()
                .option(DRIVER, "mysql")
                .option(HOST, "192.168.79.133")
                .option(USER, "root")
                .option(PORT, 3310)  // optional, default 3306
                .option(PASSWORD, "123") // optional, default null, null means has no password
                .option(DATABASE, "testdb") // optional, default null, null means not specifying the database
                .option(CONNECT_TIMEOUT, Duration.ofSeconds(3)) // optional, default null, null means no timeout
                /*.option(SSL, true) // optional, default sslMode is "preferred", it will be ignore if sslMode is set
                .option(Option.valueOf("sslMode"), "verify_identity") // optional, default "preferred"
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
                System.out.println("create option");
                return options;
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
        Mono<? extends io.r2dbc.spi.Connection> conn = Mono.from(connectionFactory.create());
        System.out.println("start map conn:"+conn.toString());
        conn.flatMapMany(connection -> {
            System.out.println("get connection:"+connection);
            Publisher<? extends Result> res = connection
                    .createStatement("SELECT * FROM mysql.user ")
                    .execute();
            return res;
        })
        .flatMap(result -> result
                .map((row, rowMetadata) -> row.get("user", String.class)))
        .doOnNext(System.out::println)
        .subscribe(i-> System.out.println("this is:"+i));
    }
    public void test_r2dbc_mysql(){
        ConnectionFactory connectionFactory = ConnectionFactories.get(create_option());
        System.out.println("get connection factory "+connectionFactory);
        Mono.from(connectionFactory.create())
                .flatMapMany(connection -> connection
                        .createStatement("SELECT * FROM testdb.t1 WHERE id = 1")
                        // .bind("$1", 100)
                        .execute())
                .flatMap(result -> result
                        .map((row, rowMetadata) -> {
                            Integer c1 = row.get("id",Integer.class);
                            System.err.println("id = " +c1);
                            log.info("id : " + c1.intValue());
                            log.debug("debug");
                            return c1;
                        }))
                .subscribe( i -> System.err.println("i= " +i.intValue()),
                        error -> System.err.println("got error: " + error),
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

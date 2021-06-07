package org.example;

import io.r2dbc.spi.ConnectionFactories;
import io.r2dbc.spi.ConnectionFactory;
import io.r2dbc.spi.ConnectionFactoryOptions;
import io.r2dbc.spi.Option;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import reactor.core.publisher.Mono;
import reactor.core.publisher.UnicastProcessor;
import java.sql.*;
import java.time.Duration;

import static io.r2dbc.spi.ConnectionFactoryOptions.*;

public class Dbconnect {
    private static Logger log = LoggerFactory.getLogger(Dbconnect.class);

    public void test_r2dbc_mysql(){
        ConnectionFactoryOptions options = ConnectionFactoryOptions.builder()
                .option(DRIVER, "mysql")
                .option(HOST, "192.168.79.132")
                .option(USER, "root")
                .option(PORT, 3310)  // optional, default 3306
                .option(PASSWORD, "Roo123.") // optional, default null, null means has no password
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
            ConnectionFactory connectionFactory = ConnectionFactories.get(options);
        System.out.println("get connection factory "+connectionFactory);
        // Creating a Mono using Project Reactor
        // Mono<Connection> connectionMono = Mono.from(connectionFactory.create());
        Mono.from(connectionFactory.create())
                .flatMapMany(connection -> connection
                        .createStatement("SELECT user,host FROM mysql.user WHERE user = $1")
                        .bind("$1", "root")
                        .execute())
                .flatMap(result -> result
                        .map((row, rowMetadata) -> {
                            String host = row.get("host", String.class);
                            System.err.println("host = " +host);
                            log.info("host : " + host);
                            log.debug("debug");
                            return host;
                        }))
                .doOnNext(System.out::println)
                .subscribe();

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

    public  void test1() {
        try {
            Class.forName("com.mysql.jdbc.Driver");
            System.out.println("数据库驱动加载成功");
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        }
        try {
            DriverManager.setLoginTimeout(1);
            Connection con = DriverManager.getConnection("jdbc:mysql://192.168.79.132:3310/mysql?characterEncoding=UTF-8&connectTimeout=3000", "root", "Roo123.");
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
            con.close();
            System.out.println("数据库连接成功");
        } catch (SQLException e) {
            System.out.println("error:" + e.toString());
            e.printStackTrace();
        }
    }
}

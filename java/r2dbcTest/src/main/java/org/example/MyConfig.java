package org.example;

import java.io.Reader;
import java.util.Properties;

import org.apache.ibatis.io.Resources;
import org.apache.ibatis.javassist.compiler.ast.Variable;
import org.apache.ibatis.session.SqlSession;
import org.apache.ibatis.session.SqlSessionFactory;
import org.apache.ibatis.session.SqlSessionFactoryBuilder;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

public class MyConfig {
    private static final Logger log = LogManager.getLogger();
    public   String databaseurl="";
    public String host="";
    public int port = 0;
    public String user="";
    public String password="";
    MyConfig(String mybatis_config_file){
        init(mybatis_config_file);
    }
    public SqlSessionFactory init(String mybatis_config_file ) { // "mybatis-config.xml"
        SqlSessionFactory sessionFactory=null;
        try {
            //使用MyBatis提供的Resources类加载mybatis的配置文件
            Reader reader = Resources.getResourceAsReader(mybatis_config_file);
            //构建sqlSession的工厂
            sessionFactory = new SqlSessionFactoryBuilder().build(reader);
            Properties vars = sessionFactory.getConfiguration().getVariables();

            this.databaseurl = (String) vars.get("jdbc.databaseurl");
            this.host = (String) vars.get("jdbc.host");
            this.port = Integer.parseInt((String) vars.get("jdbc.port"));
            this.user = (String) vars.get("jdbc.username");
            this.password = (String) vars.get("jdbc.password");
            // System.out.println("host:"+sessionFactory.getConfiguration().getVariables().get("jdbc.databaseurl"));
            log.info("databaseurl = "+databaseurl);
            log.info("user = "+user);
            log.info("password = "+password);
            log.info("host = "+host);
            log.info("port="+port);
        } catch (Exception e){
            e.printStackTrace();
        }
        return sessionFactory;
    }
}

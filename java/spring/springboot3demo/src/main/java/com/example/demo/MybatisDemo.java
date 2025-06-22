package com.example.demo;

import com.example.demo.mapper.datasource1.UserMapper;
import com.example.demo.model.User;
import org.apache.ibatis.io.Resources;
import org.apache.ibatis.session.SqlSession;
import org.apache.ibatis.session.SqlSessionFactory;
import org.apache.ibatis.session.SqlSessionFactoryBuilder;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.io.FileReader;
import java.io.Reader;
import java.sql.Statement;
import java.util.Objects;

public class MybatisDemo {
    private static final Logger log = LogManager.getLogger();
    private static SqlSessionFactory sessionFactory;
    private static SqlSession session;

    public MybatisDemo() {
        try {
            // 使用MyBatis提供的Resources类加载mybatis的配置文件
            Reader reader = Resources.getResourceAsReader("mybatis-config.xml");
            // 构建sqlSession的工厂
            sessionFactory = new SqlSessionFactoryBuilder().build(reader);
            sessionFactory.getConfiguration().getParameterMapNames();
            session = sessionFactory.openSession();

            Statement st = session.getConnection().createStatement();
            // Charset.forName("utf8")
            FileReader fr = new FileReader("src/main/resources/table_init.sql");
            char[] sql = new char[2048];
            int sql_len = fr.read(sql);
            sql[sql_len] = 0;
            log.debug("encoding {}, sql = {}, len {}", fr.getEncoding(), sql, sql_len);
            // create table user1
            // st.execute(sql.toString());
            st.close();
        } catch (Exception e) {
            e.printStackTrace();
            log.info("error: " + e);
        }
    }

    public User getUser(int userId) {
        User user = new User();
        try {
            UserMapper mapper = session.getMapper(UserMapper.class);
            user = mapper.GetUserByID(userId);
            if (!Objects.isNull(user))
                log.info(String.valueOf(user));
            else
                log.info("can not find user id: " + userId);
            session.commit();
        } catch (Exception e) {
            e.printStackTrace();
            log.info("error: " + e);
        }
        return user;
    }

    public User addUser(String name,String dept) {
        User user = new User();
        try {
            UserMapper mapper = session.getMapper(UserMapper.class);
            int ret = mapper.addUser(name,dept);
            if ( 1 == ret)
                log.info("add user success: name {} dept {}",name,dept);
            else
                log.info("can not add  user: {} {} ",name,dept);
            session.commit();
        } catch (Exception e) {
            e.printStackTrace();
            log.info("error: " + e);
        }
        return user;
    }
}

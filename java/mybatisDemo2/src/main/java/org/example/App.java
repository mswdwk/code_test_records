package org.example;

import org.apache.ibatis.io.Resources;
import org.apache.ibatis.session.SqlSession;
import org.apache.ibatis.session.SqlSessionFactory;
import org.apache.ibatis.session.SqlSessionFactoryBuilder;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.io.Reader;
import java.sql.Statement;
import java.util.ArrayList;
import java.util.List;

/**
 * Hello world!
 */
public class App {
    private final static Logger log = LogManager.getLogger();

    public static void main(String[] args) {
        log.info("Hello World!");
        try {
            // 使用MyBatis提供的Resources类加载mybatis的配置文件
            Reader reader = Resources.getResourceAsReader("mybatis-config.xml");
            // 构建sqlSession的工厂
            SqlSessionFactory sessionFactory = new SqlSessionFactoryBuilder().build(reader);
            sessionFactory.getConfiguration().getParameterMapNames();

            SqlSession session = sessionFactory.openSession();
            Statement st = session.getConnection().createStatement();
            //  Charset.forName("utf8")
//            FileReader fr = new FileReader("src/main/resources/table_init.sql");
//            char[] sql_char = new char[2048];
//            int sql_len = fr.read(sql_char);
//            sql_char[sql_len] = 0;
//            String sql_str = String.valueOf(sql_char, 0, sql_len);
//            log.info("encoding {}, sql = {}, len {}", fr.getEncoding(), sql_str, sql_len);

            List<String> sqls = readSqlFile("src/main/resources/table_init.sql");
            for (String sql : sqls) {
                log.info("execute: sql = {}, len {}", sql, sql.length());
                st.execute(sql);
            }

            UserMapper mapper = session.getMapper(UserMapper.class);
            int userId = 1;
            User user = mapper.GetUserByID(userId);
            if (user != null)
                log.info(String.valueOf(user));
            else
                log.info("no user id: " + userId);

            session.commit();
            session.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private static List<String> readSqlFile(String filePath) {
        List<String> sql_statements = new ArrayList<>();
        StringBuilder statementBuilder = new StringBuilder();
        try (BufferedReader reader = new BufferedReader(new FileReader(filePath))) {
            String line;
            while ((line = reader.readLine()) != null) {
                line = line.trim();
                // Ignore empty lines and comments
                if (line.isEmpty() || line.startsWith("--")) {
                    continue;
                }
                statementBuilder.append(line).append(" ");
                if (line.endsWith(";")) { // Statement ends with a semicolon
                    sql_statements.add(statementBuilder.toString());
                    statementBuilder = new StringBuilder(); // Reset for the next statement
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
        return sql_statements;
    }
}

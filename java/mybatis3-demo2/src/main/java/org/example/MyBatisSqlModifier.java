package org.example;

import org.apache.ibatis.builder.xml.XMLMapperBuilder;
import org.apache.ibatis.mapping.BoundSql;
import org.apache.ibatis.mapping.MappedStatement;
import org.apache.ibatis.mapping.ParameterMap;
import org.apache.ibatis.mapping.SqlSource;
import org.apache.ibatis.parsing.XNode;
import org.apache.ibatis.session.Configuration;
import org.apache.ibatis.session.SqlSession;
import org.apache.ibatis.session.SqlSessionFactory;
import org.apache.ibatis.session.SqlSessionFactoryBuilder;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.example.model.User;

import java.io.InputStream;
import java.sql.ResultSet;
import java.sql.Statement;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class MyBatisSqlModifier {
    private final static Logger log = LogManager.getLogger();

    public static void tmain(String[] args) throws Exception {
        // 1. 加载MyBatis配置
        InputStream configStream = MyBatisSqlModifier.class.getClassLoader()
                .getResourceAsStream("mybatis-config.xml");
        SqlSessionFactory sqlSessionFactory = new SqlSessionFactoryBuilder().build(configStream);
        Configuration configuration = sqlSessionFactory.getConfiguration();

        // 2. 加载Mapper XML文件
        InputStream mapperStream = MyBatisSqlModifier.class.getClassLoader()
                .getResourceAsStream("org/example/mapper/UserMapper.xml");
        System.out.println("current dir: " + System.getProperty("user.dir") + ", mapperStream " + mapperStream);
        XMLMapperBuilder mapperParser = new XMLMapperBuilder(
                mapperStream,
                configuration,
                "org/example/mapper/UserMapper.xml",
                configuration.getSqlFragments());
        mapperParser.parse();

        // 3. 获取原始MappedStatement
        String statementId = "org.example.mapper.UserMapper.selectByAge";
        MappedStatement originalMappedStatement = configuration.getMappedStatement(statementId);

        MappedStatement statement_by_id = configuration.getMappedStatement("org.example.mapper.UserMapper.selectById");

        // 4. 获取原始SQL
        BoundSql originalBoundSql = originalMappedStatement.getBoundSql(null);
        String originalSql = originalBoundSql.getSql();
        System.out.println("原始SQL: " + originalSql);

        // 5. 加工SQL - 这里添加WHERE条件
        String modifiedSql = originalSql + " WHERE age > #{age}";
        System.out.println("修改后SQL: " + modifiedSql);

        // 6. 创建新的SqlSource
        SqlSource newSqlSource = new CustomSqlSource(configuration, modifiedSql);

        // 7. 创建新的MappedStatement并添加到配置中
        MappedStatement.Builder builder = new MappedStatement.Builder(
                configuration,
                statementId + "_modified",
                newSqlSource,
                originalMappedStatement.getSqlCommandType());

        // 复制原始MappedStatement的其他属性
        builder.resource(originalMappedStatement.getResource())
                .fetchSize(originalMappedStatement.getFetchSize())
                .statementType(originalMappedStatement.getStatementType())
                .keyGenerator(originalMappedStatement.getKeyGenerator())
                .keyProperty(originalMappedStatement.getKeyProperties() == null ? null :
                        String.join(",", originalMappedStatement.getKeyProperties()))
                .timeout(originalMappedStatement.getTimeout())
                .parameterMap(originalMappedStatement.getParameterMap())
                .resultMaps(originalMappedStatement.getResultMaps())
                .resultSetType(originalMappedStatement.getResultSetType())
                .cache(originalMappedStatement.getCache())
                .flushCacheRequired(originalMappedStatement.isFlushCacheRequired())
                .useCache(originalMappedStatement.isUseCache());
        ParameterMap origin = originalMappedStatement.getParameterMap();
        log.info("originalMappedStatement param: {}", origin.getParameterMappings());


        MappedStatement modifiedMappedStatement = builder.build();
        configuration.addMappedStatement(modifiedMappedStatement);

        log.info("modifiedMappedStatement param: " + modifiedMappedStatement.getParameterMap().toString() + ", " + modifiedMappedStatement.getParameterMap().getParameterMappings());

        // 8. 执行修改后的SQL
        try (SqlSession session = sqlSessionFactory.openSession()) {
            Map<String, Object> params = new HashMap<>();
            params.put("age", 18);
            params.put("id", 1);

            log.info("statement_by_id {}", statement_by_id.getParameterMap().getParameterMappings());
            // 使用未修改的statementId执行
            List<User> users = session.selectList("org.example.mapper.UserMapper.selectById", new Long(1));
            // 处理结果
            users.forEach(System.out::println);

            // 使用未修改的statementId执行
            users = session.selectList("org.example.mapper.UserMapper.selectById2", params);
            // 处理结果
            users.forEach(System.out::println);

            // 使用修改后的statementId执行
            // TODO:
           // users = session.selectList(statementId + "_modified", params);


            XNode m = configuration.getSqlFragments().get("org.example.mapper.UserMapper.mv_table");

            System.out.println("原始SQL 2: " + m.getStringBody());
            Statement st = session.getConnection().createStatement();
            String sql2 = "select count(*) from " +m.getStringBody().trim();
            log.info("sql2 "+sql2);
            ResultSet rs = st.executeQuery(sql2);
            if(rs.next()){
                int count = rs.getInt(1);
                log.info("count = "+count);
            }

            // 处理结果
            users.forEach(System.out::println);


        }
    }

    // 自定义SqlSource实现
    static class CustomSqlSource implements SqlSource {
        private final Configuration configuration;
        private final String sql;

        public CustomSqlSource(Configuration configuration, String sql) {
            this.configuration = configuration;
            this.sql = sql;
        }

        @Override
        public BoundSql getBoundSql(Object parameterObject) {
            return new BoundSql(configuration, sql, new ArrayList<>(), parameterObject);
        }
    }
}
package org.example;

import org.apache.ibatis.builder.xml.XMLMapperBuilder;
import org.apache.ibatis.mapping.BoundSql;
import org.apache.ibatis.mapping.MappedStatement;
import org.apache.ibatis.mapping.SqlSource;
import org.apache.ibatis.session.Configuration;
import org.apache.ibatis.session.SqlSession;
import org.apache.ibatis.session.SqlSessionFactory;
import org.apache.ibatis.session.SqlSessionFactoryBuilder;
import org.example.model.User;

import java.io.InputStream;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class MyBatisSqlModifier {

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
        String statementId = "org.example.mapper.UserMapper.selectAll";
        MappedStatement originalMappedStatement = configuration.getMappedStatement(statementId);

        // 4. 获取原始SQL
        BoundSql originalBoundSql = originalMappedStatement.getBoundSql(null);
        String originalSql = originalBoundSql.getSql();
        System.out.println("原始SQL: " + originalSql);

        // 5. 加工SQL - 这里添加WHERE条件
        String modifiedSql = originalSql + " WHERE age > #{minAge}";
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

        MappedStatement modifiedMappedStatement = builder.build();
        configuration.addMappedStatement(modifiedMappedStatement);

        // 8. 执行修改后的SQL
        try (SqlSession session = sqlSessionFactory.openSession()) {
            Map<String, Object> params = new HashMap<>();
            params.put("minAge", 18);

            // 使用修改后的statementId执行
            List<User> users = session.selectList(statementId + "_modified", params);

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
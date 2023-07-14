package org.example;

import org.apache.ibatis.io.Resources;
import org.apache.ibatis.session.SqlSession;
import org.apache.ibatis.session.SqlSessionFactory;
import org.apache.ibatis.session.SqlSessionFactoryBuilder;

import java.io.Reader;

/**
 * Hello world!
 *
 */
public class App 
{
    public static void main( String[] args )
    {
        System.out.println( "Hello World!" );
        try {
            //使用MyBatis提供的Resources类加载mybatis的配置文件
            Reader reader = Resources.getResourceAsReader("mybatis-config.xml");
            //构建sqlSession的工厂
            SqlSessionFactory sessionFactory = new SqlSessionFactoryBuilder().build(reader);
            sessionFactory.getConfiguration().getParameterMapNames("");

            SqlSession session = sessionFactory.openSession();
            UserMapper mapper = session.getMapper(UserMapper.class);
            int userId = 1;
            User user= mapper.GetUserByID(userId);
            if(user != null)
                System.out.println(user.toString());
            else
                System.out.println("no user id: "+userId);

            session.commit();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}

package com.example.demo.mapper.datasource1;

import com.example.demo.model.User;
import org.apache.ibatis.session.RowBounds;

import java.util.List;

// @Mapper
public interface UserMapper {

    User GetUserByID(Integer id) throws Exception;

    Integer addUser(String name, String dept) throws Exception;

    //@Select("select * from sys_user")
    List<User> findAll();

    List<User> findAll(RowBounds rowBounds);
}
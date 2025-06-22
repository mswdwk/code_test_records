package com.example.demo.mapper.manager;

import com.example.demo.model.User;
import org.apache.ibatis.session.RowBounds;

import java.util.List;

// @Mapper
public interface UserMapper3 {

    User GetUserByID(Integer id) throws Exception;

    Integer addUser(String name, String dept) throws Exception;

    //@Select("select * from sys_user")
    List<User> findAll();

    List<User> findAll(RowBounds rowBounds);
}
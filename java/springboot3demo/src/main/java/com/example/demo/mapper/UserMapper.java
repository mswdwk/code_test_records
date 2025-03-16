package com.example.demo.mapper;

import com.example.demo.model.User;
import jakarta.annotation.sql.DataSourceDefinition;
import org.apache.ibatis.annotations.Mapper;
import org.apache.ibatis.annotations.Select;
import org.apache.ibatis.session.RowBounds;
import org.springframework.stereotype.Repository;

import java.util.List;

@Mapper
public interface UserMapper {

    User GetUserByID(Integer id) throws Exception;
    Integer addUser(String name,String dept) throws Exception;

    //@Select("select * from sys_user")
    List<User> findAll();

    List<User> findAll(RowBounds rowBounds);
}
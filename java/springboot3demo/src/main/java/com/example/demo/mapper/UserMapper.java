package com.example.demo.mapper;

import com.example.demo.model.User;

public interface UserMapper {

    User GetUserByID(Integer id) throws Exception;
    Integer addUser(String name,String dept) throws Exception;
}
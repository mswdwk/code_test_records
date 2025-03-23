package com.example.demo.service;

import com.example.demo.mapper.UserMapper;
import com.example.demo.model.User;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.List;

@Service
public class UserService {
    @Autowired
    private UserMapper userMapperDb1;

    @Autowired
    private UserMapper userMapperDb2;

    public List<User> getAllUserForDb1() {
        return userMapperDb1.findAll();
    }

    public List<User> getAllUserForDb2() {
        return userMapperDb2.findAll();
    }
}
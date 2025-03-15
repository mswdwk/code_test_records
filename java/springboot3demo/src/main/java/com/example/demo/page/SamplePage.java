package com.example.demo.page;

import com.example.demo.model.User;
import com.github.pagehelper.Page;
import com.github.pagehelper.PageHelper;
import com.github.pagehelper.PageRowBounds;
import com.example.demo.mapper.UserMapper;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.annotation.Bean;
import org.springframework.stereotype.Component;

import java.util.List;

@Component
public class SamplePage {
    private static final Logger log = LogManager.getLogger();

    @Autowired
    UserMapper userMapper;

    public List<User> demo() throws Exception {
        PageHelper.startPage(1, 20).disableAsyncCount();
        List<User> users = userMapper.findAll();
        System.out.println("Total: " + ((Page) users).getTotal());
        for (User user : users) {
            System.out.println("Name: " + user.getName());
        }

        PageHelper.orderBy("id desc");
        users = userMapper.findAll();
        System.out.println("Total: " + ((Page) users).getTotal());
        for (User user : users) {
            System.out.println("Name: " + user.getName());
        }

       /* PageRowBounds rowBounds = new PageRowBounds(3, 5);
        users = userMapper.findAll(rowBounds);
        System.out.println("Total: " + rowBounds.getTotal());
        for (User user : users) {
            System.out.println("Name: " + user.getName());
        }*/
        return  users;
    }
}

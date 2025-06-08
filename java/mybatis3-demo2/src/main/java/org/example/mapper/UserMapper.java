package org.example.mapper;


import org.example.model.User;
import java.util.List;

public interface UserMapper {
    List<User> selectAll();
    User selectById(Long id);
    void insert(User user);
    void update(User user);
    void delete(Long id);
}
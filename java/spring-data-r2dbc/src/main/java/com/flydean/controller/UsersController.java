package com.flydean.controller;

import com.flydean.dao.UsersDao;
import com.flydean.entity.Users;
import lombok.RequiredArgsConstructor;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RestController;
import reactor.core.publisher.Flux;
import reactor.core.publisher.Mono;

/**
 * @author wayne
 * @version UsersController,  2020/9/13
 */
@RestController
@RequiredArgsConstructor
public class UsersController {

    private final UsersDao usersDao;

    @GetMapping("/users")
    public Flux<Users> findAll() {
        return usersDao.findAll();
    }


 /*   @GetMapping("/adduser")
    public Flux<Integer> addUser(@RequestBody Users user) {
        return usersDao.saveAll();
    }*/

    @Transactional
    @GetMapping("/adduser")
    public Mono<Users> save(Users user) {
        return usersDao.save(user).map(it -> {
            if (it.getFirstname().equals("flydean")) {
                throw new IllegalStateException();
            } else {
                return it;
            }
        });
    }

    @Transactional
    @GetMapping("/deluser")
    public Mono<Void> delete(Integer id) {
        return usersDao.deleteById(id.longValue());
    }
}

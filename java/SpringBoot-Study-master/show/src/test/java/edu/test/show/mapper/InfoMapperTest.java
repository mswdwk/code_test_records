package edu.bistu.show.mapper;

import org.junit.jupiter.api.Test;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;

import static org.junit.jupiter.api.Assertions.*;

@SpringBootTest
class InfoMapperTest {

    @Autowired
    private InfoMapper infoMapper;

    @Test
    void test(){
        infoMapper.selectList(null).forEach(System.out::println);
    }

}
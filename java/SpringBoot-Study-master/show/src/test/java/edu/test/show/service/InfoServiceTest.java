package edu.bistu.show.service;

import edu.bistu.show.vo.DataVO;
import org.junit.jupiter.api.Test;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;

import static org.junit.jupiter.api.Assertions.*;
@SpringBootTest
class InfoServiceTest {

    @Autowired
    private  InfoService infoService;

    @Test
    void findData(){
        DataVO dataVO= infoService.findData();
        int i =0;
    }

}
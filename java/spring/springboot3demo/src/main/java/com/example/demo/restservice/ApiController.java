package com.example.demo.restservice;

import com.example.demo.model.MyReq;
import com.example.demo.model.MyRsp;
import com.example.demo.model.OneEsIndex;
import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.ObjectMapper;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.util.ArrayList;
import java.util.Date;
import java.util.List;
import java.util.concurrent.atomic.AtomicLong;

@RestController
@RequestMapping("/esindex")
public class ApiController {
    private static final Logger log = LogManager.getLogger();
    private final AtomicLong counter = new AtomicLong();
    @Autowired
    ObjectMapper objectMapper;

    @PostMapping("/hi")
    public ResponseEntity<String> post(@RequestBody MyReq req) {
        log.info(" req =  " + req);
        MyRsp rsp = new MyRsp(" hi " + counter.getAndIncrement(), new ArrayList<>());
        log.info(" req = {} rsp = {} ", req, rsp);
        return ResponseEntity.ok(rsp.toString());
    }

    @GetMapping("/getall")
    public ResponseEntity<List<OneEsIndex>> getall(@RequestParam(value = "env", defaultValue = "dev") String env) throws JsonProcessingException {
        log.info(" getall env =  " + env);
        List<OneEsIndex> rsp = getEsIndex();
        log.info(" env = {} rsp = {} ", env, rsp);
        log.info(" rsp json= " + objectMapper.writeValueAsString(rsp));
        return ResponseEntity.ok(rsp);
    }

    @PostMapping("/getall")
    public ResponseEntity<List<OneEsIndex>> getall2(@RequestBody MyReq req) throws JsonProcessingException {
        log.info(" getall2 req =  " + req);
        List<OneEsIndex> rsp = getEsIndex();
        log.info(" req = {} rsp = {} ", req, rsp);
        return ResponseEntity.ok(rsp);
    }

    private List<OneEsIndex> getEsIndex() {
        List<OneEsIndex> rsp = new ArrayList<>();

        OneEsIndex oneEsIndex = new OneEsIndex();
        oneEsIndex.setName("index_test_name_1");
        oneEsIndex.setCratTmtp(new Date());
        oneEsIndex.setDbRowCount(12345);
        oneEsIndex.setEsDocCount(12340);
        rsp.add(oneEsIndex);

        oneEsIndex = new OneEsIndex();
        oneEsIndex.setName("index_test_name_2");
        oneEsIndex.setCratTmtp(new Date());
        oneEsIndex.setDbRowCount(1234501);
        oneEsIndex.setEsDocCount(1234002);
        rsp.add(oneEsIndex);

        return rsp;
    }
}

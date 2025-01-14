package edu.bistu.show.controller;

import edu.bistu.show.service.InfoService;
import edu.bistu.show.vo.DataVO;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
public class indexController {
 //   @RequestMapping("/")
//    public String index(){
//        return "detail";
//    }
    @Autowired
    InfoService infoService;

    @RequestMapping("/list")
    public DataVO list(){
        return infoService.findData();
    }


}

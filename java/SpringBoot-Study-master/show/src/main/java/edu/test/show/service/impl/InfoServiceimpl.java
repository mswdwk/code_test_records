package edu.bistu.show.service.impl;

import edu.bistu.show.entity.Info;
import edu.bistu.show.mapper.InfoMapper;
import edu.bistu.show.service.InfoService;
import edu.bistu.show.vo.DataVO;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.List;

@Service
public class InfoServiceimpl implements InfoService {

    @Autowired
    private InfoMapper infoMapper;

    @Override
    public DataVO<Info> findData() {

        DataVO dataVO= new DataVO();
        dataVO.setCode(0);
        dataVO.setMsg("");
        dataVO.setCount(infoMapper.selectCount(null));
        List<Info> InfoList= infoMapper.selectList(null);
        dataVO.setData(InfoList);
        return dataVO;
    }
}

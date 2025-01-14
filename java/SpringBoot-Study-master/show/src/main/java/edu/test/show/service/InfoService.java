package edu.bistu.show.service;

import edu.bistu.show.entity.Info;
import edu.bistu.show.vo.DataVO;

public interface InfoService {
    public DataVO<Info> findData();
}

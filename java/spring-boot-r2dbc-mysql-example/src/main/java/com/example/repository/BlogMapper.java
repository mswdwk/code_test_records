package com.example.repository;

import com.example.entity.Blog;
import com.example.entity.BlogKey;

public interface BlogMapper {
    int deleteByPrimaryKey(BlogKey key);

    int insert(Blog record);

    int insertSelective(Blog record);

    Blog selectByPrimaryKey(BlogKey key);

    int updateByPrimaryKeySelective(Blog record);

    int updateByPrimaryKeyWithBLOBs(Blog record);

    int updateByPrimaryKey(Blog record);
}
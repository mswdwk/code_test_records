DELIMITER ;;
CREATE PROCEDURE insert_person()   /* 创建一个存储过程*/
begin      /* 存储过程开始*/
    declare c_id integer default 1;   /* 声明一个int型c_id，默认值为1*/
    while c_id<=100000 do   /* while循环 cid<=10000 */
    insert into person values(c_id, concat('name',c_id), c_id+100, date_sub(NOW(), interval c_id second));
    set c_id=c_id+1;
    end while;
end    /*存储过程开始*/
;;
DELIMITER ;

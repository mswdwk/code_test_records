CREATE DATABASE IF NOT EXISTS testdb;
USE testdb;
CREATE TABLE IF NOT EXISTS user1(id INT UNSIGNED AUTO_INCREMENT PRIMARY KEY, name VARCHAR(128) NOT NULL DEFAULT '', \
    dept VARCHAR(128) NOT NULL DEFAULT '',website VARCHAR(128) NOT NULL DEFAULT '') \
    ENGINE=INNODB CHARSET=UTF8MB4 COLLATE=UTF8MB4_BIN;

INSERT INTO user1 values (1,'name 1','depentment 1','website 1');
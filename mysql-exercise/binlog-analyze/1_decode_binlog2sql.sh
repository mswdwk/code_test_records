mysqlbinlog --no-defaults --base64-output='decode-rows' -v  mysql-bin.000009 > test.sql

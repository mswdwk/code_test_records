# example 1
mysqlslap --delimiter=";" --create="CREATE TABLE a (b int);INSERT INTO a VALUES (23)" --query="SELECT * FROM a" --concurrency=50 --iterations=200
# example 2
mysqlslap --concurrency=5 --iterations=20 --number-int-cols=2 --number-char-cols=3 --auto-generate-sql
# example 3, please prepare query.sql and create.sql
mysqlslap --concurrency=5 --iterations=20 ---query=query.sql --create=create.sql --delimiter=";" --number-of-queries=1000
# example 4
mysqlslap --delimiter=";" --create="CREATE TABLE a (b int);INSERT INTO a VALUES (23)" --query="SELECT * FROM a" --concurrency=50 --iterations=100 --number-of-queries=100
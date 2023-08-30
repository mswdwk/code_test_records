#!/usr/bin/python
import sys

field_str=""
field_num=200
fields_name=""
start_str="  CREATE TABLE test12 ("

if __name__ == "__main__":
	print "argv=",sys.argv
	if len(sys.argv) >  1:
		field_num = int(sys.argv[1])
	print "field_num=",field_num

for i in range(0,field_num):
	fields_name+="C"+str(i)+" text,"	

#end_str="c char(255))ENGINE=InnoDB ROW_FORMAT=COMPACT DEFAULT CHARSET utf8mb4;"
end_str="c char(255))ENGINE=InnoDB ROW_FORMAT=COMPACT DEFAULT CHARSET utf8mb4;"

create_sql=start_str+fields_name+end_str
print "create ddl sql is:\n",create_sql


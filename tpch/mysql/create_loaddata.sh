
load data local infile 'region.tbl' into table region fields terminated by '|' lines terminated by '|\n';
load data local infile 'nation.tbl' into table nation fields terminated by '|' lines terminated by '|\n';  
load data local infile 'part.tbl' into table part fields terminated by '|' lines terminated by '|\n'; 
load data local infile 'supplier.tbl' into table supplier fields terminated by '|' lines terminated by '|\n';
load data local infile 'partsupp.tbl' into table partsupp fields terminated by '|' lines terminated by '|\n'; 
load data local infile 'customer.tbl' into table customer fields terminated by '|' lines terminated by '|\n';
load data local infile 'orders.tbl' into table orders fields terminated by '|' lines terminated by '|\n';  
load data local infile 'lineitem.tbl' into table lineitem fields terminated by '|' lines terminated by '|\n';

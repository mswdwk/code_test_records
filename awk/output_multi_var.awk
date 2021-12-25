{
	
	if( match($0,/Slave_IO_Running:(.+)/,a))
		 sir=a[1];
	else if( match($0,/Slave_SQL_Running:(.+)/,b))
		ssr=b[1];
	else if( match($0,/Seconds_Behind_Master:(.+)/,b))
		sbm=b[1];
}
END{
	print sir," ",ssr," ",sbm
}

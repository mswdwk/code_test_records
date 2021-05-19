BEGIN{
	PROCINFO["sorted_in"]="@val_num_desc"
}
{
	
	if( match($0,/this is (.+) .+/,a))
		 suma[a[1]]+=1;
	else if (match($0,/hello (.+)/,b))
		sumb[b[1]]+=1;
}
END{
	count=0
	for(i in suma) { 
		count++
		if ( count < qian4) print i,"=",suma[i]
	}
	
	count=0
	for(i in sumb){ 
		count++
		if ( count < qian3) print i,"=",sumb[i]
	}
	#asort(sum)
	print "--------------"
}

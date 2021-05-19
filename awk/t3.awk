BEGIN{
	PROCINFO["sorted_in"]="@val_num_desc"
}
{
	
	if($0 ~ /^this is (.+) .+/)
		suma[$3]+=1;
	else if ($0 ~ /^hello (.+)/)
		sumb[$2]+=1;
}
END{
	count=0
	for(i in suma) { 
		count++
		if ( count < 3) print i,"=",suma[i]
	}
	
	count=0
	for(i in sumb){ 
		count++
		if ( count < 3) print i,"=",sumb[i]
	}
	#asort(sum)
	print "--------------"
}

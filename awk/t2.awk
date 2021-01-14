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
	for(i in suma) print i,"=",suma[i]
	for(i in sumb) print i,"=",sumb[i]
	#asort(sum)
	#for(i in sum) print i,"=",sum[i]
	print "--------------"
}

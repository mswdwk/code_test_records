BEGIN{
PROCINFO["sorted_in"]="@val_num_desc"
}
{
	
	if match($0,/this is (.+) .+/,a)
		 suma[a[1]]+=1;
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

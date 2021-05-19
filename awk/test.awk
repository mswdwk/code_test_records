BEGIN{
}
{
	suma;match($0,/this is (.+) .+/,a); suma[a[1]]+=1;
	sumb;match($0,/hello (.+)/,b);  sumb[b[1]]+=1;
}
END{
	print suma
}

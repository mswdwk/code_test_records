#include <stdio.h>

int main()
{
FILE *fp;
fp = fopen("t1.txt", "rb");
if(fp != NULL)
{
	char buf[1024]={0};
	fread(buf,1,1,fp);
  if(feof(fp)) printf("EOF\n");
  else printf("not EOF\n");
  fclose(fp);
}
else printf("open file error!\n");
return 1;
}

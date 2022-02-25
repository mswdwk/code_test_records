#include <stdio.h>
#include <errno.h>

int main()
{
FILE *fp;
int i;
char filename[64];
for(i=0;i<12000;i++){
		sprintf(filename,"f_%04d",i);
		fp = fopen(filename, "w+");
		if(fp != NULL)
		{
			printf("%d\r",i);
		 // fclose(fp);
		}
		else {
			printf("\nopen file error:file=%s i=%d, errno=%u!\n",filename,i,errno);
			break;
		}
}
return 0;
}

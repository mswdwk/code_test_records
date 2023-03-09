#include<stdio.h>

int main(int argc,char*argv[])
{
	char line[1024*64];	
	FILE*fp;
	if(argc < 2){
		printf("usage: ./program input_filename\n");
		return 1;
	}
	char *filename = argv[1];
	
	fp = fopen(filename,"r");
	if(fp == NULL){
		printf("fp %p \n",fp);
		return 1;
	}
	int line_id = 0;
	while( NULL != fgets(line,sizeof(line),fp)){
		line_id++;
		printf("%5d:%s\n",line_id,line);
	}
	return 0;
}

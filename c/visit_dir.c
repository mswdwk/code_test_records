#include <sys/types.h>
#include <string.h>
#include <ulimit.h>
#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdlib.h>

#define MAX_PATH 1024

void deal(char*dir,struct dirent *dp){
	printf("%s/%s type: %u,ino %ld\n", dir, dp->d_name,dp->d_type,dp->d_ino);
}

void dirwalk(char *dir, void (*fcn)(char *,struct dirent*))
{
	char name[MAX_PATH];
	struct dirent *dp;
	DIR *dfd;
	if ((dfd = opendir(dir)) == NULL) {
		fprintf(stderr, "dirwalk: can't open %s\n", dir);
		return;
	}
	while ((dp = readdir(dfd)) != NULL) {
		if (strcmp(dp->d_name, ".") == 0|| strcmp(dp->d_name, "..") ==0 )
			continue; /* skip self and parent */
		if ( 4 == dp->d_type ){
			if ( strlen(dir)+strlen(dp->d_name)+2 > MAX_PATH ) {
				printf("dir file name path too long : max is %d",MAX_PATH);
			} else {
				sprintf(name,"%s/%s", dir, dp->d_name);
				dirwalk(name,deal);
			}
		} else if ( 8 == dp->d_type ) {
			(*fcn)(dir,dp);
		} else {
			printf("name %s inode %ld unknown d_type %d\n",dp->d_name,dp->d_ino,dp->d_type);
			break;
		}
	}
	closedir(dfd);
}

int main(int argc,char*argv[])
{
	if(argc < 2){
		return 0;
	}
	printf("argc %d \n",argc);	
	dirwalk(argv[1],deal);
	return 0;
}

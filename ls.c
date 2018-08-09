#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include<time.h>
#include<pwd.h>
#include<grp.h>


int main(int argc,char *argv[]){
	struct dirent *read;
	struct stat sb;
	struct tm *tme;
	struct passwd *u;
	struct group *g;
	char cwd[2048] ;
	DIR *dir;
	int opt;
	int longl=0;
	int hidden=0;
	
	dir = opendir(getcwd(cwd,sizeof(cwd)));
	if (dir == -1){
		printf("Error:Directory does not exist\n");
		return 0;
	}

	while ((opt = getopt(argc, argv, "la ")) != -1) {
		switch (opt) {

			case 'a':
				hidden = 1;
				break;

			case 'l':
				longl=1;
				break;

			default:
				break;

		}
	}
	argc -= optind;
	argv += optind;

	if(longl==1){
		while((read = readdir(dir))!=NULL){
			if((stat(read->d_name,&sb))==-1)printf("Failed to get stat");
			if (hidden==1){
				printf( (S_ISDIR(sb.st_mode)) ? "d" : "-");
				printf( (sb.st_mode & S_IRUSR) ? "r" : "-");
				printf( (sb.st_mode & S_IWUSR) ? "w" : "-");
				printf( (sb.st_mode & S_IXUSR) ? "x" : "-");
				printf( (sb.st_mode & S_IRGRP) ? "r" : "-");
				printf( (sb.st_mode & S_IWGRP) ? "w" : "-");
				printf( (sb.st_mode & S_IXGRP) ? "x" : "-");
				printf( (sb.st_mode & S_IROTH) ? "r" : "-");
				printf( (sb.st_mode & S_IWOTH) ? "w" : "-");
				printf( (sb.st_mode & S_IXOTH) ? "x" : "-");
				printf(" %lo ",sb.st_nlink);
				u = getpwuid(sb.st_uid);
				g = getgrgid(sb.st_gid);
				printf("%s ",u->pw_name);
				printf("%s ",g->gr_name);
				printf("%ld ",sb.st_size);
				tme = gmtime(&sb.st_mtime);
				switch(tme->tm_mon){
					case 0:
						printf("Jan");
						break;
					case 1:
						printf("Feb");
						break;
					case 2:
						printf("Mar");
						break;
					case 3:
						printf("Apr");
						break;
					case 4:
						printf("May");
						break;
					case 5:
						printf("Jun");
						break;
					case 6:
						printf("Jul");
						break;
					case 7:
						printf("Au");
						break;
					case 8:
						printf("Sep");
						break;
					case 9:
						printf("Oct");
						break;
					case 10:
						printf("Nov");
						break;
					case 11:
						printf("Dec");
						break;
				}
				printf(" ");
				printf("%d ",tme->tm_mday);
				printf("%d:",tme->tm_hour);
				printf("%d ",tme->tm_min);
				printf("%s\n",read->d_name);
			}
			else{
				if (read->d_name[0]!='.'){
					printf( (S_ISDIR(sb.st_mode)) ? "d" : "-");
					printf( (sb.st_mode & S_IRUSR) ? "r" : "-");
					printf( (sb.st_mode & S_IWUSR) ? "w" : "-");
					printf( (sb.st_mode & S_IXUSR) ? "x" : "-");
					printf( (sb.st_mode & S_IRGRP) ? "r" : "-");
					printf( (sb.st_mode & S_IWGRP) ? "w" : "-");
					printf( (sb.st_mode & S_IXGRP) ? "x" : "-");
					printf( (sb.st_mode & S_IROTH) ? "r" : "-");
					printf( (sb.st_mode & S_IWOTH) ? "w" : "-");
					printf( (sb.st_mode & S_IXOTH) ? "x" : "-");
					printf(" %lo ",sb.st_nlink);
					u = getpwuid(sb.st_uid);
					g = getgrgid(sb.st_gid);
					printf("%s ",u->pw_name);
					printf("%s ",g->gr_name);
					printf("%ld ",sb.st_size);
					tme = gmtime(&sb.st_mtime);
					switch(tme->tm_mon){
						case 0:
							printf("Jan");
							break;
						case 1:
							printf("Feb");
							break;
						case 2:
							printf("Mar");
							break;
						case 3:
							printf("Apr");
							break;
						case 4:
							printf("May");
							break;
						case 5:
							printf("Jun");
							break;
						case 6:
							printf("Jul");
							break;
						case 7:
							printf("Au");
							break;
						case 8:
							printf("Sep");
							break;
						case 9:
							printf("Oct");
							break;
						case 10:
							printf("Nov");
							break;
						case 11:
							printf("Dec");
							break;
					}
					printf(" ");
					printf("%d ",tme->tm_mday);
					printf("%d:",tme->tm_hour);
					printf("%d ",tme->tm_min);
					printf("%s\n",read->d_name);
				}
			}
		}
	}
			else if(hidden==0){
				while((read = readdir(dir))!=NULL){
					if (read->d_name[0] !='.'){
						printf("%s ",read->d_name);
					}}
				printf("\n");
				}
			else if(hidden==1){	
				while((read = readdir(dir))!=NULL){
					printf("%s ",read->d_name);
				}
				printf("\n");
			}

				// else if(hidden==0){
				// 	while((read = readdir(dir))!=NULL){
				// 		if (read->d_name[0] !='.'){
				// 			printf("%s ",read->d_name);
				// 		}}}
				// else if(hidden==1){	
				// 	while((read = readdir(dir))!=NULL){
				// 		printf("%s ",read->d_name);
				// 	}}



//printf("\n");
return 0;
}

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main(){
char cwd[2048];
getcwd(cwd,sizeof(cwd));
printf("%s\n",cwd);
return 0;
}

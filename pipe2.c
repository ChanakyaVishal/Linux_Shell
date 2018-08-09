#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/utsname.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include "fileio.h"
#include "pipe.h"
#define cmd_buff_size 1024

char *trimwhitespace(char *str){
  char *end;

  // Trim leading space
  while(isspace((unsigned char)*str)) {str++;}

  if(*str == 0)  // All spaces?
    return str;

  // Trim trailing space
  end = str + strlen(str) - 1;
  while(end > str && isspace((unsigned char)*end)) {end--;}

  // Write new null terminator
  *(end+1) = 0;

  return str;
}

int pipe_cmd(char** arg, int cmd_num, char* redirectFile[][2]){
    char cwd[2048];
    getcwd(cwd,sizeof(cwd));

    int i=0, j=0, curr_num=-1, stat;
    char* cmd[cmd_buff_size];
    int filedes1[2], filedes2[2];
    pid_t pid;

    while(arg[i] != NULL){
        j = 0;
        while(strcmp(arg[j],"|") != 0 && arg[j] != NULL){
            cmd[j++] = arg[i++];
        }
        curr_num++;
        cmd[++j] = NULL;

        if(i%2){
            if(pipe(filedes1) != 0){
                perror("Pipe Failed");
            }
        }
        else{
            if(pipe(filedes2) != 0){
                perror("Pipe Failed");
            }
        }
        trimwhitespace(cmd);
        int flag = 0,in_flag =0,out_flag=0;

        if(cmd[strlen(cmd) - 1] == '&')flag = 1;
        if(strcmp(redirectFile[curr_num][0],"")!=0)in_flag=1;
        if(strcmp(redirectFile[curr_num][1],"")!=0)out_flag=1;

        pid = fork();
        if(pid<0){
            perror("Error. Could not fork child");
        }
        else if(pid == 0){
            if(i==0){
                dup2(filedes1[1],1);
            }
            else if(i == cmd_num -1){
                if(i%2){
                    dup2(filedes1[0], 0);
                }
                else{
                    dup2(filedes2[0], 0);
                }
            }
            else{
                if(i%2){
                    dup2(filedes1[1], 1);
                    dup2(filedes2[0], 0);
                }
                else{
                    dup2(filedes1[0], 0);
                    dup2(filedes2[1], 1);
                }
            }


            char* builtin = (char*)malloc(cmd_buff_size);
            if(strcmp(cmd[0],"ls")==0)strcpy(builtin,strcat(cwd,"/ls"));
            else if(strcmp(cmd[0],"pinfo")==0)strcpy(builtin,strcat(cwd,"/pinfo"));
            else if(strcmp(cmd[0],"cd")==0){
                char* temp = (char*)malloc(cmd_buff_size);
                if(cmd[1] == "~"){
                    strcpy(temp,cwd);
                }
                else
                strcpy(temp,cmd[1]);

                if(chdir(temp) == -1)printf("bash: cd: %s: No such file or directory\n",temp);
            }
            else if(strcmp(cmd[0],"pwd")==0)strcpy(builtin,strcat(cwd,"/pwd"));
            else if(strcmp(cmd[0],"echo")==0)strcpy(builtin,strcat(cwd,"/echo"));
            else strcpy(builtin,cmd[0]);

            if(in_flag && out_flag){
                redirectIO_IO(cmd,redirectFile[curr_num][0],redirectFile[curr_num][1],flag,builtin);
            }
            else if(in_flag){
                redirectIO_I(cmd,redirectFile[curr_num][0],flag,builtin);
            }
            else if(out_flag){
                redirectIO_O(cmd,redirectFile[curr_num][1],flag,builtin);
            }
            else{
                if(execvp(builtin,arg) == -1){
                    kill(getpid(), SIGTERM);
                }
                else {
                    exit(EXIT_SUCCESS);
                }
            }
        }

        if (i == 0){
            close(filedes2[1]);
        }
        else if (i == cmd_num - 1){
            if (cmd_num % 2){
                close(filedes1[0]);
            }else{
                close(filedes2[0]);
            }
        }
        else{
            if (i % 2){
                close(filedes2[0]);
                close(filedes1[1]);
            }else{
                close(filedes1[0]);
                close(filedes2[1]);
            }
        }
        if(flag==0)waitpid(pid,&stat,0);

        i++;
    }
    return 1;
}

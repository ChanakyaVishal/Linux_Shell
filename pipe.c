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

int pipe_cmd(char** args, int num_cmds, char* redirectFile[][2]){
    int filedes1[2], filedes2[2], curr_num = -1;
	char *command[256];
    pid_t pid;
    char* builtin = (char*)malloc(cmd_buff_size);

	int end = 0, i = 0, j = 0, k = 0, l = 0;

	while (args[j] != NULL && end != 1){
        char cwd[2048];
        getcwd(cwd,sizeof(cwd));

		k = 0;
		while (strcmp(args[j],"|") != 0){
			command[k] = args[j];
			j++;	
			if (args[j] == NULL){
				end = 1;
				k++;
				break;
			}
			k++;
        }
        curr_num++;
		command[k] = NULL;
        j++;
        
        if(strcmp(command[0],"ls")==0){
            strcpy(builtin,strcat(cwd,"/ls"));
        }
        else if(strcmp(command[0],"pinfo")==0){
            strcpy(builtin,strcat(cwd,"/pinfo"));
        }
        else if(strcmp(command[0],"cd")==0){
            char* temp = (char*)malloc(cmd_buff_size);
            if(command[1] == "~"){
                strcpy(temp,cwd);
            }
            else
            strcpy(temp,command[1]);
            if(chdir(temp) == -1)printf("bash: cd: %s: No such file or directory\n",temp);
        }
        else if(strcmp(command[0],"pwd")==0){
            strcpy(builtin,strcat(cwd,"/pwd"));
        }
        else if(strcmp(command[0],"echo")==0){
            strcpy(builtin,strcat(cwd,"/echo"));
        }
        else {
            strcpy(builtin,command[0]);
        }

        printf("comm:%s\n", builtin);
        int in_flag=0,out_flag=0,flag=0;
        // printf("%d\n",curr_num);
        if(redirectFile[curr_num][0] != NULL && strlen(redirectFile[curr_num][0]) > 0)in_flag=1;
        // printf("wtf\n");
        if(redirectFile[curr_num][1] != NULL && strlen(redirectFile[curr_num][1]) > 0)out_flag=1;
        printf("in:%d, out:%d\n",in_flag,out_flag);

		if (i % 2 != 0){
			pipe(filedes1);
		}else{
			pipe(filedes2);
		}
		
        pid=fork();
        
        if(pid==-1){			
			if (i != num_cmds - 1){
				if (i % 2 != 0){
					close(filedes1[1]);
				}else{
					close(filedes2[1]);
				} 
			}			
			printf("Child process could not be created\n");
			return;
		}
		if(pid==0){
			if (i == 0){
				dup2(filedes2[1], 1);
			}
			else if (i == num_cmds - 1){
				if (num_cmds % 2 != 0){
					dup2(filedes1[0],0);
                }
                else{
					dup2(filedes2[0],0);
				}

            }
            else{
				if (i % 2 != 0){
					dup2(filedes2[0],0); 
					dup2(filedes1[1],1);
                }
                else{
					dup2(filedes1[0],0); 
					dup2(filedes2[1],1);					
				} 
			}

    
            if(in_flag && out_flag){
                // printf("1\n");
                redirectIO_IO(command,redirectFile[curr_num][0],redirectFile[curr_num][1],flag,builtin);
            }
            else if(in_flag){
                // printf("3\n");
                redirectIO_I(command,redirectFile[curr_num][0],flag,builtin);
            }
            else if(out_flag){
                // printf("4\n");
                redirectIO_O(command,redirectFile[curr_num][1],flag,builtin);
            }
            else{
                if(execvp(command[0],command) == -1){
                    // printf("5\n");
                    kill(getpid(), SIGTERM);
                }
                else {
                    // printf("6\n");                    
                    exit(EXIT_SUCCESS);
                }
            }
		}
				
		if (i == 0){
			close(filedes2[1]);
		}
		else if (i == num_cmds - 1){
			if (num_cmds % 2 != 0){					
				close(filedes1[0]);
			}else{					
				close(filedes2[0]);
			}
		}else{
			if (i % 2 != 0){					
				close(filedes2[0]);
				close(filedes1[1]);
			}else{					
				close(filedes1[0]);
				close(filedes2[1]);
			}
		}
				
		waitpid(pid,&stat,0);
		i++;	
    }
    return 1;
}

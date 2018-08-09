#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include <sys/utsname.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include <pwd.h>
#include "parser.h"
#include "exec.h"
#include "fileio.h"
#include "pipe.h"
#define buffer_size 1024
#define parse_buffer_size 128


// char *trimwhitespace(char *str){
//   char *end;
//
//   // Trim leading space
//   while(isspace((unsigned char)*str)) {str++;}
//
//   if(*str == 0)  // All spaces?
//     return str;
//
//   // Trim trailing space
//   end = str + strlen(str) - 1;
//   while(end > str && isspace((unsigned char)*end)) {end--;}
//
//   // Write new null terminator
//   *(end+1) = 0;
//
//   return str;
// }

int main(){
    char hostname[2048];
    char cwd[2048];

    gethostname(hostname,2048);
    struct utsname unameData;
    uname(&unameData);

    char *Os = unameData.sysname;
    char path[2048];
    char og[2048];

    getcwd(cwd,sizeof(cwd));
    strcpy(path,cwd);
    strcpy(og,cwd);

    while(1){
        strcpy(path,og);
        char* redirectFile[1024][2];
        char* buffer = (char*)malloc(buffer_size);
        char* inFile = (char*)malloc(buffer_size);
        char* outFile = (char*)malloc(buffer_size);
        char** parsed_buffer = (char**)malloc(buffer_size);
        memset(inFile,0,strlen(inFile));
        memset(outFile,0,strlen(outFile));
        int i;
        for(i=0;i<1024;i++){
            redirectFile[i][0]=0;
            redirectFile[i][1]=0;
        }
    //Read Line
        getcwd(cwd,sizeof(cwd));
        printf("<%s-%s:%s>-",hostname,Os,cwd);
        fflush(stdout);

        int pos = 0, c, status = 0, temp_i_pos = 0, temp_o_pos = 0, cmd_num = 0;
        int redirect_o_flag = 0, redirect_i_flag = 0,temp_i =0, temp_o=0;
        while((c = getchar()) != '\n'){
            if(c == '<'){
                // printf("<%c\n",(char)c);
                redirect_i_flag = 1;
                temp_i=1;
                temp_o=0;
                continue;
            }
            else if(c == '>'){
                // printf(">%c\n",(char)c);
                redirect_o_flag = 1;
                temp_i=0;
                temp_o=1;
                continue;
            }
            else if(c == EOF){
                break;
            }
            else{
                if(c == '|'){
                    // printf("|%c\n",(char)c);
                    // inFile[temp_i_pos] = '\0';
                    // outFile[temp_o_pos] = '\0';
                    redirectFile[cmd_num][0] = inFile;
                    redirectFile[cmd_num][1] = outFile;
                    cmd_num++;
                    memset(inFile,0,strlen(inFile));
                    memset(outFile,0,strlen(outFile));
                    temp_i = 0;
                    temp_o = 0;
                    temp_i_pos = 0;
                    temp_o_pos = 0;
                    // printf("%d\n",cmd_num);
                }
                // printf("%c\n",(char)c);
                if(temp_i == 0 && temp_o == 0)
                    buffer[pos++] = (char)c;
                else if(temp_i == 1 && temp_o == 0)
                    inFile[temp_i_pos++] = (char)c;
                else
                    outFile[temp_o_pos++] = (char)c;
            }
        }
        cmd_num++;
        buffer = trimwhitespace(buffer);
        inFile = trimwhitespace(inFile);
        outFile = trimwhitespace(outFile);

        pos = strlen(buffer);
        // printf("%s\n",buffer);
    //Parse Line
        // for(i=0;i<5;i++){
        //     printf("id:%d ",i);
        //     printf("in:%s  ",redirectFile[i][0]);
        //     printf("out:%s\n",redirectFile[i][1]);
        // }
        int flag = 0;
        if(buffer[pos-1] == '&'){
            flag = 1;
            buffer[pos-1]=NULL;
        }
        char ex[4] = {'e', 'x', 'i', 't'};
        if(strcmp(buffer,ex) == 0)
            break;

        parsed_buffer = parse_line(buffer);
        if(strcmp(parsed_buffer[0],"quit") == 0){
          break;
        }
        else if(cmd_num > 1){
            pipe_cmd(parsed_buffer, cmd_num, redirectFile);
        }
        else if(strcmp(parsed_buffer[0],"ls") == 0){
            if(redirect_o_flag && redirect_i_flag)redirectIO_IO(parsed_buffer,inFile,outFile,flag,strcat(path,"/ls"));
            else if(redirect_o_flag)redirectIO_O(parsed_buffer,outFile,flag,strcat(path,"/ls"));
            else if(redirect_i_flag)redirectIO_I(parsed_buffer,inFile,flag,strcat(path,"/ls"));
            else execute_builtin(parsed_buffer,flag,strcat(path,"/ls"));
        }

        else if(strcmp(parsed_buffer[0],"pwd") == 0){
            if(redirect_o_flag && redirect_i_flag)redirectIO_IO(parsed_buffer,inFile,outFile,flag,strcat(path,"/pwd"));
            else if(redirect_o_flag)redirectIO_O(parsed_buffer,outFile,flag,strcat(path,"/pwd"));
            else if(redirect_i_flag)redirectIO_I(parsed_buffer,inFile,flag,strcat(path,"/pwd"));
            else execute_builtin(parsed_buffer,flag,strcat(path,"/pwd"));
        }

        else if(strcmp(parsed_buffer[0],"cd") == 0){
            char* temp = (char*)malloc(buffer_size);
            if(pos == 2 ||parsed_buffer[1][0] == '~'){
                strcpy(temp,og);
            }
            else
            strcpy(temp,parsed_buffer[1]);

            if(chdir(temp) == -1)printf("bash: cd: %s: No such file or directory\n",temp);
        }

        else if(strcmp(parsed_buffer[0],"echo") == 0){
            if(redirect_o_flag && redirect_i_flag)redirectIO_IO(parsed_buffer,inFile,outFile,flag,strcat(path,"/echo"));
            else if(redirect_o_flag)redirectIO_O(parsed_buffer,outFile,flag,strcat(path,"/echo"));
            else if(redirect_i_flag)redirectIO_I(parsed_buffer,inFile,flag,strcat(path,"/echo"));
            else execute_builtin(parsed_buffer,flag,strcat(path,"/echo"));
        }

        else if(strcmp(parsed_buffer[0],"pinfo") == 0){
            if(redirect_o_flag && redirect_i_flag)redirectIO_IO(parsed_buffer,inFile,outFile,flag,strcat(path,"/pinfo"));
            else if(redirect_o_flag)redirectIO_O(parsed_buffer,outFile,flag,strcat(path,"/pinfo"));
            else if(redirect_i_flag)redirectIO_I(parsed_buffer,inFile,flag,strcat(path,"/pinfo"));
            else execute_builtin(parsed_buffer,flag,strcat(path,"/pinfo"));
        }

        else if(strcmp(parsed_buffer[0],"setenv") == 0){
            manageEnviron(parsed_buffer,1);
        }
        else if(strcmp(parsed_buffer[0],"unsetenv") == 0){
            manageEnviron(parsed_buffer,2);
        }
             
        else if (strcmp(parsed_buffer[0],"env") == 0){
            printf("%s\n",getenv(parsed_buffer[1]));
        } 
    //Execute Line
        else {
            if(redirect_o_flag && redirect_i_flag)redirectIO_IO(parsed_buffer,inFile,outFile,flag,parsed_buffer[0]);
            else if(redirect_o_flag)status = redirectIO_O(parsed_buffer,outFile,flag,parsed_buffer[0]);
            else if(redirect_i_flag)status = redirectIO_I(parsed_buffer,inFile,flag,parsed_buffer[0]);
            else status = execute(parsed_buffer, flag);
        }
    }
    return 0;
}

int manageEnviron(char * argv[], int flag){
	char **env_aux;
	if(flag ==1){
			if((argv[1] == NULL) && argv[2] == NULL){
				printf("%s","Not enought input arguments\n");
				return -1;
			}
			if(getenv(argv[1]) != NULL){
				printf("%s", "The variable has been overwritten\n");
			}else{
				printf("%s", "The variable has been created\n");
			}

			if (argv[2] == NULL){
				setenv(argv[1], "", 1);
			}else{
				setenv(argv[1], argv[2], 1);
			}
		}
	else if (flag==2){
			if(argv[1] == NULL){
				printf("%s","Not enought input arguments\n");
				return -1;
			}
			if(getenv(argv[1]) != NULL){
				unsetenv(argv[1]);
				printf("%s", "The variable has been erased\n");
			}else{
				printf("%s", "The variable does not exist\n");
			}
	}
return 0;

}
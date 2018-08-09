#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include <sys/utsname.h>
#include <sys/types.h>
#include "exec.h"
int execute_builtin(char** arg, int flag, char* builtin){
    pid_t pid, wait_pid;
    int stat;
    // printf("%s\n",arg[0]);
    // printf("%s\n",builtin);
    pid = fork();
    if(pid < 0){
        printf("Could not fork process. Exiting...");
        exit(EXIT_FAILURE);
    }
    else if(pid == 0){
    //Child process
        if(execvp(builtin,arg) == -1){
            printf("Error occured. Could not run process\n");
            // return 0;
            exit(EXIT_FAILURE);
        }
        exit(EXIT_SUCCESS);
    }
    // int len = sizeof(arg)/sizeof(arg[0]);
    if(flag==0)waitpid(pid, &stat, 0);
    return 1;
}

int execute(char** arg, int flag){
    pid_t pid, wait_pid;
    int stat;

    pid = fork();
    if(pid < 0){
        printf("Could not fork process. Exiting...");
        exit(EXIT_FAILURE);
        // return 0;
    }
    else if(pid == 0){
    //Child process
        if(execvp(arg[0],arg) == -1){
            printf("Error occured. Could not run process\n");
            exit(EXIT_FAILURE);
            // return 0;
        }
        exit(EXIT_SUCCESS);
    }
    if(flag==0)waitpid(pid, &stat, 0);
    return 1;
}

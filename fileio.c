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

int redirectIO_I(char** arg, char* inFile, int flag, char* builtin){
      int fd, status;
      pid_t pid;

      struct stat buffer;
      int exist = stat(inFile,&buffer);

      if(exist != 0){
        printf("bash: %s: No such file or directory", inFile);
        return 0;
      }

      pid = fork();

      if(pid == 0){
        fd = open(inFile, O_RDONLY, 0600);
        if(fd == -1){
          perror("Failed to open input file");
          return 0;
        }
        dup2(fd, 0);
        close(fd);

        if(execvp(builtin,arg) == -1){
            printf("Error occured. Could not run process\n");
            // return 0;
            exit(EXIT_FAILURE);
        }
        exit(EXIT_SUCCESS);
      }
      if(flag==0)waitpid(pid, &status, 0);
      return 1;
  }


  int redirectIO_O(char** arg, char* outFile, int flag, char* builtin){
      int fd, status;
      pid_t pid;

      pid = fork();

      if(pid == 0){
        fd = open(outFile, O_CREAT | O_WRONLY | O_TRUNC, 0600);
        if(fd == -1){
          perror("error: Failed to open output file\n");
          return 0;
        }
        dup2(fd, 1);
        close(fd);

        if(execvp(builtin,arg) == -1){
            printf("error: Error occured. Could not run process\n");
            // return 0;
            exit(EXIT_FAILURE);
        }
        exit(EXIT_SUCCESS);
      }
      if(flag==0)waitpid(pid, &status, 0);
      return 1;
}

int redirectIO_IO(char** arg, char* inFile, char* outFile, int flag, char* builtin){
    int fd1, fd2;
    pid_t pid;

    struct stat buffer;
    int exist2 = stat(inFile,&buffer);
    int status;

    if(exist2 != 0){
      printf("bash: %s: No such file or directory", inFile);
      return 0;
    }

    pid = fork();

    if(pid == 0){
      fd1 = open(outFile, O_CREAT | O_WRONLY | O_TRUNC, 0644);
      if(fd1 == -1){
        perror("error: Failed to open input file");
        return 0;
      }
      dup2(fd1, 1);
      close(fd1);

      fd2 = open(inFile, O_RDONLY, 0644);
      if(fd2 == -1){
        perror("error: Failed to open output file");
        return 0;
      }
      dup2(fd2, 0);
      close(fd2);

      if(execvp(builtin,arg) == -1){
          perror("error: Error occured. Could not run process\n");
          // return 0;
          exit(EXIT_FAILURE);
      }
      exit(EXIT_SUCCESS);
    }

    if(flag==0)waitpid(pid, &status, 0);
    return 1;
}

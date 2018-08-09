#include<stdio.h>
#include<stdlib.h>
#include "parser.h"
#define parse_buffer_size 128

char** parse_line(char* line){
    char** tokens = (char**)malloc(parse_buffer_size * sizeof(char*));
    char* token;
    int pos = 0;
    if(!tokens){
        printf("Memory allocation error!");
        exit(EXIT_FAILURE);
    }

    // const char* delim = " ;";
    token = strtok(line, " ");
    
    while(token!=NULL){
        tokens[pos++] = token;
        token = strtok(NULL, " ");
    }
    tokens[pos] = NULL;
    return tokens;
}
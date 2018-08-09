#ifndef pipe_flag
#define pipe_flag 1

char *trimwhitespace(char* str);
int pipe_cmd(char** arg, int cmd_num, char* redirectFile[][2]);

#endif

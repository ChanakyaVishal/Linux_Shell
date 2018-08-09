#ifndef fileio_flag
#define fileio_flag 1

int redirectIO_I(char** arg, char* inFile, int flag, char* builtin);
int redirectIO_O(char** arg, char* outFile, int flag, char* builtin);
int redirectIO_IO(char** arg, char* inFile, char* outFile, int flag, char* builtin);

#endif

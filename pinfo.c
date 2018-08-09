#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <ctype.h>

#define buf_size 1024

int main(int argc, char **argv){
	int pid;
	int i;
	long long int size;
	
	char buf[buf_size] = {0};
	char buf2[PATH_MAX] = {0};
	char state;
	
	if (argc == 1){
		pid = getppid();
	}

	else{
		sscanf(argv[1], "%d", &pid);
	}


	sprintf(buf, "/proc/%d/status", pid);
	FILE *file = fopen(buf, "r");
	do {
	fgets(buf, buf_size, file);
	buf[5] = '\0';
	}while (strcmp(buf, "State") != 0);
	
	for (i=6;!isalpha(buf[i]);i++);
	state = buf[i];
	
	do{
		fgets(buf, buf_size, file);
		buf[6] = '\0';
	}while (strcmp(buf, "VmSize") != 0);

	i = 0;
	while (!isdigit(buf[i])) i++;

	sscanf(buf + i, "%lld", &size);
	sprintf(buf, "/proc/%d/exe", pid);
	realpath(buf, buf2);

	printf("PID : %d\nProcess State : %c\nMemory : %lld KB\nExecutable Path : %s\n", pid, state, size, buf2);

	return 0;
}


shell: main.c parser.o exec.o fileio.o pipe.o ls pwd echo pinfo
	gcc -g -Wall main.c parser.o exec.o fileio.o pipe.o -o shell

exec.o: exec.c exec.h
	gcc -c exec.c -o exec.o
parser.o: parser.c parser.h
	gcc -c parser.c -o parser.o
fileio.o: fileio.c fileio.h
	gcc -c fileio.c -o fileio.o
pipe.o: pipe.c pipe.h
	gcc -c pipe.c -o pipe.o

ls: ls.c
	gcc -Wall ls.c -o ls
echo: echo.c
	gcc -Wall echo.c -o echo
pwd: pwd.c
	gcc -Wall pwd.c -o pwd
pinfo: pinfo.c
	gcc -Wall pinfo.c -o pinfo
clean:
	rm shell ls pwd echo pinfo parser.o exec.o fileio.o pipe.o

Linux 
=======

Specification 1: Display requirement
When you execute your code a shell prompt of the
following form must appear:
<username@system_name:curr_dir>
E.g. <Name@UBUNTU:~>

Specification 2: Builtin commands
* cd : To change the current directory
* pwd : Prints the current path
* echo : Prints

Specification 3: ls command
Implement ls [al] – (it should be able to handle 
* ls
* ls -l
* ls -a 
* ls -la

Specification 4: System commands with and without arguments
All other commands are treated as system commands like : emacs, vi and so on. 
Foreground processes: For example, executing a "vi" command in the foreground implies that the
shell will wait for this process to complete and regain control when this process exits.
Background processes: Any command invoked with "&" is treated as background command.
This implies that the shell will spawn that process and doesn't wait for the process to exit. It will
keep taking user commands.
E.g
<Name@UBUNTU:~> ls &
This command when finished, should print its result to stdout.
<Name@UBUNTU:~>emacs &
<Name@UBUNTU:~> ls -l -a ( Make sure all the given flags are executed properly for any command and
not just ls.)


Specification 5: pinfo command (user defined)
pinfo : prints the process related info of your shell program.


Specification 6: Foreground and background processes
Apart from the user defined commands, all other commands are treated as system commands like:
emacs, vi and so on. 
Foreground processes: For example, executing a "vi" command in the foreground implies that your shell will
wait for this process to complete and regain control when this process exits.
Background processes: Any command invoked with "&" is treated as background command. 
E.g
<NAME@UBUNTU:~>ls &
This command when finished, should print its result to stdout.
<NAME@UBUNTU:~>emacs &
<NAME@UBUNTU:~>ls -l -a
<NAME@UBUNTU:~>echo hello
As and when emacs exits, your shell program should check the exit status of emacs and print it
on stderr
<NAME@UBUNTU:~>
emacs with pid 456 exited normally
<NAME@UBUNTU:~>

Specification 7: Input-output redirection functionality
Output of running one (or more) commands must be redirected to a file. Similarly, a command might
be prompted to read input data from a file and asked to write output to another file. 
E.g. Output redirection
<NAME@UBUNTU:~>diff file1.txt file2.txt > output.txt
E.g. Input redirection
<NAME@UBUNTU:~>sort < lines.txt
E.g. Input-Output redirection
<NAME@UBUNTU:~>sort < lines.txt > sortedlines.txt
Note: There is another clause for output direction '>>', and that must be implemented appropriately.

Specification 8: Implement command redirection using pipes
A pipe is identified by "|". One or more commands can be piped as the following examples show.
E.g. Two Commands
<NAME@UBUNTU:~>more file.txt | wc
E.g. Three commands
<NAME@UBUNTU:~>grep "new" temp.txt | cat - somefile.txt | wc

Specification 9: Implement I/O redirection + pipes redirection
E.g.
<NAME@UBUNTU:~>ls | grep *.txt > out.txt
<NAME@UBUNTU:~>cat < in.txt | wc -l > lines.txt


## To run the code :
	- Make file : 'make'
	- Run the shell file : './shell'
	- To clean, run 'make clean'

	
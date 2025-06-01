OurShell
OurShell is a lightweight custom shell written in C that mimics basic functionalities of UNIX shells. It supports internal commands like changing directories, listing directory contents, displaying environment variables, clearing the screen, pausing execution, echoing user input, and more. It also supports executing external commands using fork and execvp.

Features
Custom prompt showing the current working directory

Built-in commands:

mycd <directory> – Change the current directory (no argument shows current path)

myclr – Clear the screen

mydir <directory> – List contents of a directory

myenviron – List all environment variables

myecho <comment> – Display the provided comment

myhelp – Display a user manual

mypause – Pause shell until Enter is pressed

myquit – Exit the shell

myone – Display the number of files in the current directory and their modification times

External command execution via child process

How to Compile
Use gcc to compile the shell:
gcc -o OurShell ourshell.c
Replace ourshell.c with your actual filename if different.

How to Run
./OurShell


You should see the custom shell prompt like:
OurShell:/current/path$


Notes
Uses standard C libraries like stdio.h, unistd.h, dirent.h, and sys/types.h

Supports up to 64 arguments per command

Demonstrates use of fork(), execvp(), and wait()

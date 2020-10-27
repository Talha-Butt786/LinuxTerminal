Created by : Muhammad Talha Butt s195475

Run Program by Terminal on linux(Debian/Ubuntu):
1. open the terminal
2. Go to your project root by typing cd followed by address to your root project.
3. Type "gcc main.c -o main", this will compile your main file.
4. Now simply type "./main" this will execute main file
Note: This process requires gcc to be installed, install it by typing "sudo apt-get install build-essential".

User Manual:
** program acts as a linux shell and you are given your current directory.
** you can change the directory as you do in a normal linux shell by typing e.g. 'cd /home'.
** you can do piping, though only through a single pipeline.
** you can type different commands like 'ls -l' , 'cat filename | wc -w' etc.
** you can close the shell by typing 'exit'.

NOTE:
** Error handling is not implemented in the program, we pre-assume that you type the right command.
** For piping: a space must be added before or after typing '|'. e.g. ls -l | wc

System calls:
Short description of system calls is that, it is a way of going from user mode to the kernel mode by using system calls.
e.g when we have to access a file which is located in a hard disc, we have go to the kernel mode to access the file, because
all of these devices are govern by the OS itself. Basically you get the functionality of a kernel mode.

exit:
This system call terminates the calling process.

pipe:
So we need the pipe system call when we have two programs running and the output of one is the input of another.
In our case we first create an array pipefd of two int values that represent the file descriptor. File descriptor is a value given by the OS,
representing each opening file. pipe() takes array of these value as a parameter and creates a pipe, [1] representing the write channel,
while [0] represents the read channel. So now two processes can communicate through this pipe.

Exec:
This system call runs an executable file, in an already running process,eg. after creating the new process through fork(), we call this function.
we are using the Execvp() function to run the Exec system call in a c program. it takes two parameters, a pointer to the executable file and the pointer to arguments.
Note: The code after Execvp() will never run as this function replaces the calling process image with a new one.

Fork:
This system call is used to create a child process. a child process is a 'clone' of a parent process though has its own id.
In c, we use Fork(), that returns three values, 0 denotes the child, a positive value denotes the parent process and a negative value means a child is not
created.
In our program we call this method different times. if there is no pipe input, then we only create a single child by calling Fork().
if there is a pipe, means two processes are needed to create, therefore we call fork() two times.

Dup2:
To implement the pipe functionality we need dup2 system call.
we call dup2(), which takes two arguments, old fd and new fd. Basically it duplicates the file descriptor and deletes the old one. this comes handy,
when we have to redirect the output of one program to the input of another program
In our case we redirect the standard output of the program to write channel of the pipe.
Later on we redirect the standard input of the program to read channel of the pipe, means program takes the input from the pipe.

Wait:
This system call 'waits' for a child process to finish, that's why we called wait() two times for the implementation of pipe.
It is called in a parent process.



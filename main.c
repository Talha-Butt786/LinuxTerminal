#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
void ExternalCommands (char **inputCommand);

int main() {
    while (1){
        char currentdirectory[500];
        //https://man7.org/linux/man-pages/man2/getcwd.2.html
        getcwd(currentdirectory,sizeof(currentdirectory));
        printf("%s $ ",currentdirectory);

        //input from the keyboard is stored in a heap.
        char *stringInput = malloc(sizeof(char) * 200);
        // https://stackoverflow.com/questions/6282198/reading-string-from-input-with-space-character
        scanf("%[^\n]%*c", stringInput);




       /**
        * we create two arrays that hold the pointer to different inputs
        * arrays are initialised with NULL values
        **/

        char *inputCommand[10] = {0};  // (without pipe command)
        char *pipeCommand[10] = {0};   // (only pipe command)


        int inputCommandCounter = 0;
        int pipeCommandCounter = 0;
        int isPipe = 0;


        /**
         * split the string input from keyboard ino tokens where there is a space.
        *  temp now points at the first input from the keyboard  e.g. "ls"
        * https://www.tutorialspoint.com/c_standard_library/c_function_strtok.htm
       **/
        char *temp = strtok(stringInput," ");


        // This loop runs runs until temp points at NULL.
        // This code stores the logic of storing the inputs accordingly.
        while (temp != NULL){

            //checks if the input includes a pipe sign.
            if(*temp=='|'){
                // when temp is pointing at '|', thus we know a pipe command will be given after this
                //therefore we move the pointer to the next string.
                temp = strtok(NULL, " ");
                isPipe++;
            }

            // if there is a pipe input
            else if(isPipe > 0){
                //store the pipeCommand, temp is pointing at.
                pipeCommand[pipeCommandCounter]=strdup(temp);
                //again we move the pointer to next argument.
                temp = strtok(NULL, " ");
                pipeCommandCounter++;
            }

            // if no pipe input
            else {
                inputCommand[inputCommandCounter] = strdup(temp);
                temp = strtok(NULL, " ");
                inputCommandCounter++;
            }
        }


        ExternalCommands(inputCommand);


        int cdchecker = strcmp(inputCommand[0],"cd");


        /**
         * this piece of code runs if there is 'no' pipe
         * secondly it also checks if the request is for cd, because
         * we must not create a child process after changing a directory while we are
         * in a same loop.
         * Problem occurs when we type the exit command, which terminates the calling
         * child process only, therefore we had to type exit two times to terminate the program completely.
        **/
        if(isPipe == 0 && cdchecker != 0){
            int pid = fork();
            if(pid > 0){
                //parent process
                wait(NULL);    // wait for child process to finish
            } else {
                //child process
                execvp(inputCommand[0], inputCommand); //inspired from bhupjit
            }
        }


        //implementation of pipe command and again checks for "cd"
        //Note: Implementation of pipe is inspired from bhupjits code in pipe_if.c
        else if(cdchecker!=0) {

            int pipefd[2];    // pipe channels, one for write and other to read
            pipe(pipefd);    //creates a pipe, so that processes can communicate
            int pid = fork();  // child process is created and value is stored

            // Under any worst scenario,where kernel doesn't creates the child process.
            if(pid<0)
            {
                perror("fork");
                exit(1);
            }

                //child proccess
            else if (pid==0)
            {
                //first we close the read end of the pipe.
                close (pipefd[0]);
                // we give dup2 the file descriptor of write channel and give the
                //stdout as an argument,so that the output from this process is written to the pipe.
                dup2(pipefd[1],STDOUT_FILENO);
                // execute the input command
                execvp(inputCommand[0],inputCommand);

            }

                // parent process of child 1.
            else
            {
                // another child process is forked.
                if(fork()==0){
                    // since we want to read and not write to the pipe, the write channel must be closed.
                    close (pipefd[1]);
                    // again dup2 helps us to read the input from the pipe, which holds the output
                    // of the last process. and be given as an input to new program.
                    dup2(pipefd[0],STDIN_FILENO);
                    // execute the pipe command
                    execvp(pipeCommand[0],pipeCommand);
                }


                else
                {   //https://linux.die.net/man/2/close
                    // after finishing the request we close the read and write end of the pipeline
                    close(pipefd[0]);
                    close(pipefd[1]);
                    // wait for both the processes to finish off.
                    wait(NULL);
                    wait(NULL);
                    //free the the pointers.
                    free(*inputCommand);
                    free(*pipeCommand);
                }

            }


        }
        free(temp);
    }
}

//Method looks after the  "exit" and "cd" commands from the input.
void ExternalCommands (char **inputCommand) {

    if (strcmp(inputCommand[0], "exit") == 0) {
        // from bhupjit
        exit(0);
    }

        //https://www.geeksforgeeks.org/chdir-in-c-language-with-examples/
    if (strcmp(inputCommand[0], "cd") == 0) {
        //change the directory according to the given one.
        chdir(inputCommand[1]);
    }
}
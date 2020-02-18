#include "parser/ast.h"
#include "shell.h"
#include<stdio.h>
#include<unistd.h>
#include<limits.h>
#include<string.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<sys/wait.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<readline/readline.h> 
#include<readline/history.h> 

void initialize(void)
{
    /* This code will be called once at startup */
    if (prompt)
        prompt = "vush$ ";
}

void run_command(node_t *node)
{
    /* For testing: */
    //print_tree(node);

    if (prompt)
        prompt = "vush$ ";
    
    if(node->type == NODE_REDIRECT){
        pid_t pid, wpid;
        mode_t accessmode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
        int fd, fd2, status = 0;
        fd= node->redirect.fd;
        fd2= node->redirect.fd2;
        
        
        pid = fork();
        
        if(pid == 0){
            
            if(node->redirect.mode == REDIRECT_INPUT){
                fd = open(node->redirect.target, O_RDWR, accessmode);
                if(fd<0)
                     printf("Input File error");
                dup2(fd, STDIN_FILENO);
                close(fd);
            }
            
            if(node->redirect.mode == REDIRECT_OUTPUT){
               fd2 = open(node->redirect.target, O_WRONLY|O_CREAT|O_TRUNC, accessmode);
                if(fd2<0)
                     printf("Output File error");
                dup2(fd2, STDOUT_FILENO);
                close(fd2);
            }
            if(node->redirect.mode == REDIRECT_APPEND){
               FILE *test = fopen(node->redirect.target, "a");
                fd2=fileno(test);
                if(fd2<0)
                     printf("Output File error");
                dup2(fd2, 1);
                close(fd2);
            }
                
            if(execvp(node->redirect.child->command.argv[0], node->redirect.child->command.argv) == -1){
                    perror("Error in redirection");
                }
                exit(0);
        }
        else if(pid < 0){
            perror("Error in forking process");
        }
        else{
            do{
                wpid = waitpid(pid, &status, WUNTRACED);
            }
            while(!WIFEXITED(status) && !WIFSIGNALED(status));
        }
    }
    
    if(node->type == NODE_PIPE){
    for(int i=0; i<node->pipe.n_parts;i++){
            char** parsed = node->pipe.parts[i]->command.argv;
            char** parsedpipe = node->pipe.parts[i+1]->command.argv;
            int pipefd[2];  
            pid_t p1, p2; 
   
            if (pipe(pipefd) < 0) { 
                printf("\nPipe could not be initialized"); 
                return; 
            } 
          
            if(strcmp(parsed[0], "cd") == 0){
            chdir(parsed[1]);
            }
            
            p1 = fork();
              
            if (p1 < 0) { 
                printf("\nCould not fork"); 
                return; 
            } 
  
            if (p1 == 0) { 
                close(pipefd[0]); 
                dup2(pipefd[1], STDOUT_FILENO); 
                close(pipefd[1]); 
  
            if (execvp(parsed[0], parsed) < 0) { 
            printf("\nCould not execute first command"); 
            exit(0); 
            } 
                   
              
        } else { 
        // Parent executing 

        p2 = fork();
            if (p2 < 0) { 
            printf("\nCould not fork"); 
            return; 
            } 
  
        // Child 2 executing.. 
        // It only needs to read at the read end 
            if (p2 == 0) { 
            close(pipefd[1]); 
            dup2(pipefd[0], STDIN_FILENO); 
            close(pipefd[0]); 
            if (execvp(parsedpipe[0], parsedpipe) < 0) { 
                printf("\nCould not execute second command"); 
                exit(0); 
            } 
            
            } else { 
            // parent executing, waiting for two children 
            wait(NULL); 
            wait(NULL);
            return;
            } 
        }
     }
    }
    if(node->type == NODE_SEQUENCE){
        run_command(node->sequence.first);
        run_command(node->sequence.second);
    }
    
    if(node->type == NODE_COMMAND){
        char *program = node->command.program;
        char **argv = node->command.argv;
        
        
        if(strcmp(argv[0], "cd") == 0){
           chdir(argv[1]);
        }
        else if(strcmp(argv[0], "exit") == 0){
            exit(atoi(argv[1]));
        }
        else{
        
        pid_t pid = fork();
        
        if(pid==-1)
        {
            perror("\nError in forking the process");
            return;
        }
        else if(pid == 0)
        {
            if(execvp(argv[0], argv) == -1)
            {
            perror("\nInnapproriate command");
            }
            exit(0);
        }
        else
        {
            wait(NULL);
            return;
        }
        }
    }
}
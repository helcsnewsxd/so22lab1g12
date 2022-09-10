#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <unistd.h>
#include <sys/types.h> 
#include <time.h>
#include <sys/wait.h>
#include <fcntl.h>

#include "execute.h"
#include "builtin.h"
#include "command.h"



void execute_pipeline(pipeline apipe){
    // REQUIRES
    assert(apipe != NULL);

    scommand fst_command = pipeline_front(apipe);
    
    char *redir_in = scommand_get_redir_in(fst_command);
    char *redir_out = scommand_get_redir_out(fst_command);

    if(redir_in != NULL){
        close(STDIN_FILENO);
    }

    if(redir_out != NULL){
        close(STDOUT_FILENO);
        
    }
    

    if(builtin_alone(apipe)){
        // run internal command
        builtin_run(fst_command); 
    } else {
        // one external command
        int pid_fork = fork();
        if(pid_fork < 0){
            perror("System error with fork: ");
            exit(EXIT_FAILURE);
        } else if (pid_fork == 0) {
            char **argv = scommand_to_char_list(fst_command); // auxiliar function that saves args in argv (list of pointers)
            execvp(argv[0], argv);
            printf("Esto no tendria que aparecer\n");
        } else {
            wait(NULL);
        }
    }
}
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include "helper_functions.h"

void command_pipe(string s, int newsockfd);
void process_shshcmd(string received_buf, int newsockfd);
  	
void process_shshcmd(string received_buf, int newsockfd){

	string command;
	string argument;

	command = get_command(received_buf);
	argument = get_argument(received_buf);
	
	// CMD
	if(command.compare("cmd")==0){
		system(argument.c_str());		
	}
	
	// PIPE
	else if(command.compare("pipe")==0){
		command_pipe(argument, newsockfd);
	}

	else cout << "[Shsh] '"<< command <<"' is NOT a valid command"<<endl;
}

	// PIPE
void command_pipe(string s, int newsockfd){

		//save commands to array
		int cmds_size = 0;
		string cmds[10];
		string delimiter = ";";
		size_t index = 0;
		string token;

		while ((index = s.find(delimiter)) != std::string::npos) {
    		token = s.substr(0, index);
    		cmds[cmds_size] = token;
		cmds_size++;
    		s.erase(0, index + delimiter.length());
		}
	   	cmds[cmds_size] = s;
		cmds_size++;

	//------------------------ IMPLEMENTING PIPE -------------------------------------	
     		
     		//file descriptor to handle input/output 
     		int fd_input;
     		int fd_output;
     		int pid;
  
		for(int i=0;i<cmds_size;i++){
		
			//redirect input
      			dup2(fd_input, 0);
      			close(fd_input);
      			
      			//setup output
      			if (i == cmds_size-1){
        			// last command 
          			fd_output=dup(newsockfd);
      			}
      			
      			else {
          			// if not last command, then create pipe
          			int fdpipe[2];
          			pipe(fdpipe);
          			fd_input=fdpipe[0];
          			fd_output=fdpipe[1];
       			}
	   		
	   		// Redirect output
       			dup2(fd_output,1);
       			close(fd_output);
       			
       			// Create child and process command
       			pid=fork();
       			if(pid==0) {
         			system(cmds[i].c_str());
         			_exit(0);
       			}

			else wait(NULL);
       			
       		}//end for

	   //-----------------------------------------------------------------------

}


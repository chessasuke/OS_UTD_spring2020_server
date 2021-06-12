#include <iostream>
#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "shsh_functions.h"

#define PORT 53276 //5 + UTD ID 4 last digits

using namespace std;

//pthread function
void * new_upcoming_conn(void * arg);


//signal handlers
void catch_end_process(int sig){
	//cout << "Interrupt signal is (" << sig << ")"<<endl;
	int n;
	for (auto it = begin(users_table); it != end(users_table); ++it) {
		n = write(it->get_sock_fd(), "exit", 5);
   		if (n < 0) perror("ERROR writing to socket");
		close(it->get_sock_fd());	
		kill(it->get_pid(), SIGTERM);
	}
	sleep(1);
	cout << "[UM] terminated on admin request with "<<active_users<<" active users"<<endl;
	kill(getpid(), SIGTERM);
};


void catch_sleep(int sig){
	//cout << "Interrupt signal is (" << sig << ")"<<endl;
	int t = 5;
	cout << "[UM] received the sleep signal, going to sleep for "<< t<<" seconds"<<endl;
	sleep(t);
};


void catch_print_info(int sig){
	//cout << "Interrupt signal is (" << sig << ")"<<endl;
	cout << "*******[UM] INFO*******"<<endl;
	cout << "pid: "<<getpid()<<endl;
	cout << "port: "<<PORT<<endl;
	cout << "active users: "<<active_users<<endl;
	cout << "*********************"<<endl;
};

void catch_print_users(int sig){
	//cout << "Interrupt signal is (" << sig << ")"<<endl;
	cout << "*******[UM] User List*******"<<endl;
	for (auto it = begin(users_table); it != end(users_table); ++it) {
		it->print_info();
	}
};


int main(){

	signal(SIGQUIT, catch_end_process);
	signal(SIGINT, catch_sleep);
	signal(SIGRTMAX, catch_print_info);
	signal(SIGRTMIN, catch_print_users);

     	int sockfd, newsockfd, clilen, pid;
     	struct sockaddr_in serv_addr, cli_addr;

	//server socket
     	sockfd = socket(AF_INET, SOCK_STREAM, 0);
     	if (sockfd < 0) perror("ERROR opening socket");

     	bzero((char *) &serv_addr, sizeof(serv_addr));
     	//portno = 53276; //5 + UTD ID 4 last digits
     	serv_addr.sin_family = AF_INET;
     	serv_addr.sin_addr.s_addr = INADDR_ANY;
     	serv_addr.sin_port = htons(PORT);

	// bind server socket
     	if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
              perror("ERROR on binding");
     	listen(sockfd,5);
     	clilen = sizeof(cli_addr);
     	
     	cout << "[UM] Server Socket ready (pid="<<getpid()<<")"<<endl;
     	
	pthread_t users_thread[10];
	int i = 0;
    
	while (1) {

//accept new user connection
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, (socklen_t*)&clilen);
        if (newsockfd < 0) perror("ERROR on accept");
	
	else{

	int c_addr = htons(cli_addr.sin_addr.s_addr);
	int c_port = htons(cli_addr.sin_port);
	
	cout << "***[UM] Connection Accepted***"<<endl;
	//cout << " newsockfd: "<<newsockfd  <<endl;
	//cout << " cli_addr.sin_addr.s_addr: "<<cli_addr.sin_addr.s_addr<<endl;
	//cout << " c_addr: "<<c_addr<<endl;
	//cout << " cli_addr.sin_port: "<<cli_addr.sin_port<<endl;
	//cout << " c_port: "<<c_port<<endl;

	char buffer[1024];
	int n;
	
	//get user ID
	string u_id = "";
   	n = read(newsockfd,buffer,1024);//first line read should be new user id
   	u_id = buffer;
   	//if(!is_only_digits(u_id))continue; //not needed since user.exe handles (only digits)id
	User new_user(u_id, newsockfd);
   	
// NEW-USER SHSH-PROCESS

	int fd1[2];
	pipe(fd1); //parent to child (UM to User)
	
	int pid;
	pid = fork();
	
	if( pid < 0 ) 
      	{perror("Error forking");  exit(1);}
	
	// child process (new user shsh process)
	else if(pid == 0){
		
		//close unused pipes ends
		close(fd1[1]);
  		
  		dup2(newsockfd, 1); //redirect output to corresponding user socket
		
		char buf[1024];
		int received;
		
		while(1){
			received = read (fd1[0], &buf, 1024);
			if(received>0){
				string received_msg = buf;
				//handle logout
				if(received_msg.compare("logout")==0){
					write(newsockfd, "exit", 4);
					_exit(0);
				}
				process_shshcmd(received_msg, newsockfd);
			}
			bzero(buf,1024); // copy 'null' (/0) to memory address to be used
		}
	}// end else if

	// close unused pipes ends
	close(fd1[0]);
	
	// save new user's pid
	new_user.set_pid(pid);
	
	// save new user's port
	new_user.set_port(c_port);
	
	// save new_user sender pipe 
	new_user.set_pipe_fd(fd1[1]);
	
	// print user info:
	new_user.print_info();
	
	// save new user to users table and inc users count
	users_table.push_back(new_user);
	active_users++;
	
// Create new user's thread	
        if( pthread_create(&users_thread[i], NULL, new_upcoming_conn, &u_id) != 0 )
           cout << "Failed to create thread" << endl;
        else cout << "[UM] New user "<< u_id << " logs in, thread created, socket#: "<< newsockfd << " port#: "<< c_port <<endl;

        if( i >= 10)
        {i = 0;
          while(i < 10)
          {
            pthread_join(users_thread[i++],NULL);
          }
          i = 0;
        }
	
	}//end if (pthread_create)
    	
	
     } //end while
     
     
     return 0; 
}

void * new_upcoming_conn(void * arg){
	
	string user_id = *static_cast<string*>(arg);
	int n;
  	char buffer[1024];
  	int user_socket = users_table.at(get_user(user_id)).get_sock_fd();
 	int user_pipe = users_table.at(get_user(user_id)).get_pipe_fd();
 	int user_port = users_table.at(get_user(user_id)).get_port();
   	
	while(1){

		string received_msg;
   		bzero(buffer,1024); // copy 'null' (/0) to memory address to be used
   		n = read(user_socket,buffer,1024);
   		if (n < 0) perror("ERROR reading from socket");
		   		
		if(n > 0){
			buffer[n] = '\0'; // to avoid c string problems
			//handle logout
   			if(strstr(buffer, "logout")){
   				//cout << buffer << endl;
				write (user_pipe, buffer, strlen(buffer)+1);
        			cout << "[UM] User "<< user_id << " logs out - Socket#: "<< user_socket << " - Port#: "<< user_port<< " has been closed, thread is terminating" <<endl;
   				close(user_socket);
   				kill(users_table.at(get_user(user_id)).get_pid(), SIGTERM);
   				remove_user(user_id);
   				active_users--;
   				break;
   			}
   			received_msg = buffer;
			//cout << "[UM] received:"<< received_msg << endl;
			//cout << "received_msg:"<< received_msg << endl;
			
			//send message to user's shsh through the corresponding pipe
			write (user_pipe, buffer, strlen(buffer)+1);
   		}
   		received_msg = "";
   		//n = write(user_socket,"got it",6);
   		//if (n < 0) perror("ERROR writing to socket");
	}
}

// signal functions here





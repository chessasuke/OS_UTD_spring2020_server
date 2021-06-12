#ifndef USER_H
#define USER_H
#include <iostream>

using namespace std;

class User {
	private:
	string user_id = "";
	int sock_fd;
	int pipe_fd;
	pid_t pid;
	int port_num;
	
	public:

	//constructors
		User(string u, int socket) {
		 	user_id = u;
		 	sock_fd = socket;
		}

	void print_info(){
		cout << "User_id: "<<user_id<< " Port#: "<<port_num<<" Pid: "<<pid<<endl;
	}

	//setters
		void set_user_id(string s)
		{ user_id = s; }
		
		void set_sock_fd(int fd_sock)
		{ sock_fd = fd_sock; }

		void set_pipe_fd(int fd_pipe)
		{ pipe_fd = fd_pipe; }

		void set_pid(pid_t id_process)
		{ pid = id_process; }

		void set_port(int port)
		{ port_num = port; }		

	//getters
		string get_user_id()
		{ return user_id; }
		
		int get_sock_fd()
		{ return sock_fd; }

		int get_pipe_fd()
		{ return pipe_fd; }

		pid_t get_pid()
		{ return pid; }

		int get_port()
		{ return port_num; }

};
 
#endif




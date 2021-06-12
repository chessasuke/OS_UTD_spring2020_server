// Admin Process
#include <iostream>
#include <signal.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <sys/types.h> 
#include <unistd.h> 

using namespace std;
 

int main(){
	
	pid_t um_pid;
	string admin_in ="";
	
	//get UM's pid
	cout << "Enter UM's pid: "<<endl;
	cin >> um_pid;
	
	//send signals
	while(1){
	
	cout << "Enter signal: "<<endl;
	cin  >> admin_in;
	cout << "admin_in: "<<admin_in<<endl;
	
	//terminate (SIGQUIT)
	if(admin_in.compare("terminate") == 0){
		cout <<"terminate - SIGQUIT"<<endl;
		kill(um_pid, SIGQUIT);
		break;
	}
	
	//sleep (SIGINT)
	else if(admin_in.compare("sleep")== 0){
		cout <<"sleep - SIGINT"<<endl;
		kill(um_pid, SIGINT);
	}

	//infor (SIGRTMAX)
	else if(admin_in.compare("infor")== 0){
		cout <<"infor - SIGRTMAX"<<endl;
		kill(um_pid, SIGRTMAX);
	}
	
	//listuser (SIGRTMIN)
	else if(admin_in.compare("listuser")== 0){
		cout <<"listuser - SIGRTMIN"<<endl;
		kill(um_pid, SIGRTMIN);
	}

	else if(admin_in.compare("help")== 0){
		cout << "--Available Commands--"<<endl;
		cout << "listuser"<<endl;
		cout << "infor"<<endl;
		cout << "sleep"<<endl;
		cout << "terminate"<<endl;
	}
	
	else cout << "COMMAND NOT VALID"<<endl;
	
	admin_in = "";
}
	
    return 0; 
} 











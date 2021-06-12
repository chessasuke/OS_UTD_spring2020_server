// Client side
#include <iostream>
#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h>
#include "str_manipulation.h"

//#define PORT 53276 //5 + UTD ID 4 last digits

using namespace std;

int create_socket(const char * hostname, int PORT){

    int sock = 0;
    struct sockaddr_in serv_addr; 

	//get socket file descriptor
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        cout<<" Socket creation error "; 
        return -1; 
    } 
   
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(PORT); 
    
    //convert address to binary
    if(inet_pton(AF_INET, hostname, &serv_addr.sin_addr)<=0)  
    { 
        cout << " Invalid address "<<endl; 
        return -1; 
    } 
   
    //connect
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        cout << " Connection Failed "<<endl; 
        return -1; 
    }
    
    return sock;
}

int main() 
{
	//char hostname[] = "127.0.0.1";
	char hostname[100];
	int port = 53276;
	int sock_fd;
	
	string temp;
	string in = "";
	string out;
	string u_id;
	char buffer_read[1024];
	char buffer_send[1024];
	
	cout << "**Please Login**"<<endl;
	cout <<	"Insert IP Address (for localhost use 127.0.0.1): ";
    	getline (cin, in);
	strcpy(hostname, in.c_str());
	cout << "Insert Port Number (please use 53276): ";
    	getline (cin, in);
	port = atoi(in.c_str());
	in = "";
	cout << "Insert User ID: ";
    	getline (cin, in);
	
	//Establish connection
	do{
		//to login first word of the line must be a number
		temp = in;
		u_id = split_str1(temp, " ");
		trim(u_id);
		if(is_only_digits(u_id) && u_id.size()>0){
		
			sock_fd = create_socket(hostname, port);
			if(sock_fd!=-1){
				cout << "Connected"<<endl;
				strcpy(buffer_send, u_id.c_str());
				send(sock_fd , buffer_send , strlen(buffer_send)+1 , 0);
				cout << "sending User ID: "<<buffer_send<<endl;
				break;
			}
			
			else {
				cout << "Unable to connect, try again"<<endl;
				
				while(1){
					cout <<	"Insert IP Address (for localhost use 127.0.0.1): "<<endl;
    					getline (cin, in);
					strcpy(hostname, in.c_str());
					cout << "Insert Port Number (please use 53276): "<<endl;
    					getline (cin, in);
					port = atoi(in.c_str());
					in = "";
					cout << "Insert User ID: "<<endl;
    					getline (cin, in);

					if(sock_fd!=-1){
						cout << "Connected"<<endl;
						strcpy(buffer_send, u_id.c_str());
						send(sock_fd , buffer_send , strlen(buffer_send)+1 , 0);
						cout << "User ID: "<<buffer_send<<endl;
						break;
					}
					else cout << "Unable to connect, try again"<<endl;
			
				}
			}
		}
		
		else { 	
		cout << ("Invalid ID, try again: ");
    		getline (cin, in);
		}
	}while(1);
	
	cout << "User "<<u_id<<" - Socket "<<sock_fd<<" - Port "<< port << " has been created"<<endl;
	
	while(1){
		
		//get users input
		cout << ("ucmd>");
    		getline (cin, out);
		strcpy(buffer_send, out.c_str());

		send(sock_fd , buffer_send , strlen(buffer_send)+1 , 0);
		//cout << "sending: "<<buffer_send<<endl;

		if(out.compare("logout") == 0){
			break;
		}
		
		int n;
		string received_msg;
		n = read(sock_fd , buffer_read, 1024);
		if( n > 0){
			buffer_read[n] = '\0';
			received_msg = buffer_read;
			if(received_msg.compare("exit") == 0){
				cout << "Exiting..."<<endl;
				break;
			}
			//cout << "buffer_read: "<< endl << buffer_read << endl;
			cout <<buffer_read << endl;
		}
	}
	
	close(sock_fd);
	cout << "User "<<u_id<<" - Socket "<<sock_fd<<" has been closed, process terminated"<<endl;

    return 0; 
} 











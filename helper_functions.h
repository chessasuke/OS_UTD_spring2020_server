#include <iostream>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h> 
#include <stdio.h>
#include <fstream>
#include <fcntl.h>
#include <vector>

#include "str_manipulation.h"
//#include "shsh_functions.h"
#include "User1.h"

vector<User> users_table;
int active_users = 0;

// function to return index of user from user table
// given its user id
int get_user(string id){

	for(int i=0; i<users_table.size(); i++)
	{if(id.compare(users_table.at(i).get_user_id().c_str())==0)return i;}
	return -1;
}

// remove user given id
void remove_user(string u_id){
	//cout << "Removing user with ID: "<<u_id<<endl;
	users_table.erase(users_table.begin() + get_user(u_id));
}

// get command from received message
string get_command(string temp){

	string command = split_str1(temp, " ");
	trim(command); 
	return command;
}

// get argument from received message
string get_argument(string temp){

	string argument = split_str2(temp, " ");
	trim(argument);
	return argument;
}

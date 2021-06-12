#include <iostream>
#include <sstream>
#include <string>
#include <algorithm> 
#include <cctype>
#include <locale>

#ifdef WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

using namespace std;

std::string get_current_dir() {
   char buff[FILENAME_MAX]; //create string buffer to hold path
   GetCurrentDir( buff, FILENAME_MAX );
   string current_working_dir(buff);
   return current_working_dir;
}

int str_to_int(string str){
	
	int str_num = 0;
	stringstream converter (str);
	converter >> str_num;
	return str_num;
}

//check if only digits
//return true if is all digits
bool is_only_digits(string s)
{	
    	for(int i = 0; i<s.size();i++){
        	if (isdigit(s[i]++) == 0) return false;
    	}

    return true;
}

//remove user id from command
string str_rm_id(string s){
	string delimiter = " ";
	if(isdigit(s[0]) != 0)
		return s.substr(0, s.find(delimiter));
	else return s;
}


// return first string after 1 split
string split_str1(string s, string delimiter){
	if(s.find(delimiter) != std::string::npos)
	return (s.substr(0, s.find(delimiter)));
	else return s;
}

// return second string after 1 split
string split_str2(string s, string delimiter){
	if(s.find(delimiter) != std::string::npos)
	return (s.substr(s.find(delimiter), s.size()));
	else return s;
}

//remove spaces
string str_rm_spaces(string str){
 	string s = "";
	int j = 0;
	for (int i = 0; i < str.size(); i++){
		if(str[i] != ' '){
		s = s + str[i];
		j++;}
	}
	return s;
} 

//remove substring
string str_rm_substr(string s, string substr){
	
	string out = s;
	if(out.size()<= substr.size())return out;
	int i = out.find(substr);
	int j = i + substr.size();
	out.erase(i, j);
	return out;
}

// trim from start (in place)
static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
        return !std::isspace(ch);
    }));
}

// trim from end (in place)
static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}



---------- UTD COURSE OPERATING SYSTEM project 3 spring 2020 -----------------------


Carlos Hevia Alejano

*missing pre command*--(not sure what is required there)


***THE FOLDER HAS***
-- three(3) '.cpp' files -> "admin.cpp", "user.cpp", "um.cpp", 
-- four(4)  '.h' header files -> "helper_functions.h", "shsh_functions.h", "str_manipulation.h", "User1.h"
-- three(3) executable files -> "admin", "um", "user"

CPP FILES

admin.cpp has the code for the admin process, first accepts the process id of 'UM' and then only accepts the corresponding strings for send the interrupt signals.

user.cpp has the code for users (client side), first the connection has to be setup providing ip address, port number and the user id (Handling for connection setup error is not optimal). After connection the program accepts shsh commands like 'cmd', 'pipe' and 'logout'.

um.cpp has the code for the user manager. Accepts incoming clients and makes a thread and a shsh process for each (up to 10 users, there is no error handling for more users). Saves required info from users in an instance of class 'User1' and saves the instance in the vector 'users_table'. When a user writes to UM its message goes to the corresponding thread, the thread sends the message to the corresponding shsh process, and the shsh-process answers back to the user.


HEADER FILES
helper_functions, str_manipulation have useful functions for convenience.

User1 is the user class to handle users' info.

shsh_functions has the implementation of shsh functions 'cmd' and 'pipe'

EXECUTABLE FILES
"admin", "um", "user" are the output of the CPP files compilation.

If not present just compile with:
g++ um.cpp -pthread -o um
g++ user.cpp -o user
g++ admin.cpp -o admin


The program was tested on localhost and on two different computers and works perfect.

EXAMPLE:

ifconfig (to get ip address of um)
./um	 (um's pid will be printed at the very first line)
./admin	 (input into admin program the um's pid)
./user   (run in another window and test shsh commands)

After setup both connections test shsh commands:

(EXAMPLES IN USER PROCESS)
cmd echo ok
pipe ls;grep "user"
pipe ls -l;sort -r;head

(COMMANDS IN ADMIN PROCESS)
sleep		(put um main thread to sleep for 5 secs)
infor		(prints on UM's info on UM display)
listuser	(prints active users and their info on UM display)
terminate	(gracefully terminates UM and closes its processes/sockets/pipes and sends exit message to active users. There is an intentional 1 sec delay in UM's SIGQUIT signal handler)

 




 


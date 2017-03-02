#include <iostream>
// Stuff for AJAX
#include "cgicc/Cgicc.h"
#include "cgicc/HTTPHTMLHeader.h"
#include "cgicc/HTMLClasses.h"

//Stuff for pipes
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <iostream>
#include <fstream>
#include "fifo.h"
#include <string>

using namespace std;
using namespace cgicc; // Needed for AJAX functions.

string parseMessage(string);


// fifo for communication
string receive_fifo = "CRreply";
string send_fifo = "CRrequest";

int main() {
	cout << "test";
  Cgicc cgi;    // Ajax object
  char *cstr;
  // Create AJAX objects to recieve information from web page.
  form_iterator username = cgi.getElement("user");
  form_iterator messagetext = cgi.getElement("message");

  // create the FIFOs for communication
  Fifo recfifo(receive_fifo);
  Fifo sendfifo(send_fifo);
  
while(1) {

  // Call server to get results
  string user = **username;
string message = **messagetext;
  string ajaxmessage =  "&&"+user+"~~"+message;
  sendfifo.openwrite();
  sendfifo.send(ajaxmessage);
  
  /* Get a message from a server */
  recfifo.openread();
  string results = recfifo.recv();
  recfifo.fifoclose();
  sendfifo.fifoclose();
  string reply = parseMessage(results);
  cout << "Content-Type: text/plain\n\n";

  cout << reply;
  }
  
return 0;
}

string parseMessage(string message) {
	string user;
const string userDelineator = "&&";
 const string messageDelineator = "~~";
   size_t userPos = message.find_first_of(userDelineator);
 size_t messagePos = message.find_first_of(messageDelineator); 
   if (userPos != string::npos) {
     //message[userPos] = ""; 
     user = message.substr(2, userPos-1);
     //userPos = message.find_first_of(userDelineator, userPos+1); 
     }
      if (messagePos != string::npos) {
     message = message.substr(userPos+2,messagePos-1);
     //message[messagePos] = ""; 
     messagePos = message.find_first_of(messageDelineator, messagePos+1); 
     }
      message = user + ": " + message;	
	  return message;
}

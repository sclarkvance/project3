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
	string finalmessage, results;
  Cgicc cgi;    // Ajax object
  char *cstr;
  // Create AJAX objects to recieve information from web page.
  form_iterator username = cgi.getElement("user");
  form_iterator messagetext = cgi.getElement("message");

  // create the FIFOs for communication
  Fifo recfifo(receive_fifo);
  Fifo sendfifo(send_fifo);
  

  // Call server to get results
  string user = **username;
string message = **messagetext;
  string ajaxmessage =  "&&"+user+"~~"+message;
  sendfifo.openwrite();
  sendfifo.send(ajaxmessage);
    sendfifo.fifoclose();
  /* Get a message from a server */
   cout << "Content-Type: text/plain\n\n";
	
  recfifo.openread();
  //do-while to loop until end message is sent
  do {
  results = recfifo.recv();

finalmessage = parseMessage(results);
//if statement to make sure final message has text
 if (finalmessage != "") {
cout<< "<p>" << finalmessage << "</p>";
}
  }
  while (results.find("<!--$END-->") == string::npos);

    recfifo.fifoclose();
return 0;
}


string parseMessage(string message) {
	string user;
const string userDelineator = "&&";
 const string messageDelineator = "~~";
   size_t userPos = message.find_first_of(userDelineator);
 size_t messagePos = message.find_first_of(messageDelineator); 
 //if statement to take out delineators from user
   if (userPos != string::npos) {
     user = message.substr(2, messagePos-2); 
     }
     //if statement to take out delineators from message
      if (messagePos != string::npos) {
     message = message.substr(messagePos+2,userPos-2);
     messagePos = message.find_first_of(messageDelineator, messagePos+1); 
     }
     //if statement to prevent end message being sent and to prevent blank messages 
     //showing up as a colon
	 if(message.find("<!--$END-->") == string::npos && message != "") {
      message = user + ": " + message;	
	  return message;
	 }
	 else {
		 message = "";
		 return message;
	 }
}
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
using namespace cgicc; // Needed for AJAX functions

string parseMessage(string);
// Precondition: string is received
// Postcondition: delineators are removed from string and final message is returned

// FIFOs for communication
string receive_fifo = "CRreply";
string send_fifo = "CRrequest";

int main() {
	string results, finalMessage;
	Cgicc cgi; // AJAX object
	char *cstr;
	
	// Create AJAX objects to receive information from web page
	form_iterator username = cgi.getElement("user");
	form_iterator messageText = cgi.getElement("message");

	// Create the FIFOs for communication
	Fifo recfifo(receive_fifo);
	Fifo sendfifo(send_fifo);
  
	// Call server to get results
	string user = **username;
	string message = **messageText;
	if (user.size() != 0 && message.size() != 0) {
	string ajaxMessage =  "&&"+user+"~~"+message; // Insert delineators
	sendfifo.openwrite();
	sendfifo.send(ajaxMessage);
    sendfifo.fifoclose();
    }
	/* Get a message from a server */
	cout << "Content-Type: text/plain\n\n";
	
	recfifo.openread();
	// Get results up to end message
	do {
		results = recfifo.recv();
		finalMessage = parseMessage(results);
		// Make sure final message has text
		if (finalMessage != "") {
			cout << "<p>" << finalMessage << "</p>";
		}
	} while (results.find("<!--$END-->") == string::npos);
	recfifo.fifoclose();
	
	return 0;
}

string parseMessage(string message) {
	string user;
	string finalMessage;
	const string userDelineator = "&&";
	const string messageDelineator = "~~";
	size_t userPos = message.find_first_of(userDelineator);
	size_t messagePos = message.find_first_of(messageDelineator); 
	
	// Remove delineators from user
	if (userPos != string::npos) {
		user = message.substr(2, messagePos-2); 
    }
	
    // Remove delineators from message
    if (messagePos != string::npos) {
		message = message.substr(messagePos+2,userPos-2);
		messagePos = message.find_first_of(messageDelineator, messagePos+1); 
    }
	
    // Prevent end message from being sent and prevent blank messages from showing up as a colon
	if (message.find("<!--$END-->") == string::npos && message != "") {
		finalMessage = user + ": " + message;	
		return finalMessage;
	} else {
		finalMessage = "";
		return finalMessage;
	}
}
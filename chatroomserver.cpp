#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include "fifo.h"
#include <string>
#include <vector>
#include <stddef.h>

using namespace std;

/* Fifo names */
string receive_fifo = "CRrequest";
string send_fifo = "CRreply";

string getMessage(string);
// Precondition:
// Postcondition:

string sendMessage(string);
// Precondition:
// Postcondition:

int main() {
	vector<string> chatVector;
	string user, message, fullChat;
	
    // Create the FIFOs for communication
	Fifo recfifo(receive_fifo);
	Fifo sendfifo(send_fifo);
	
	// Get results from AJAX
	while(1) {
		getMessage(message);
		sendMessage(fullChat);
    }
}

string getMessage(string) {
	fullChat = "";
	cout << "Getting fifo" << endl;
	/* Get a message from a client */
	recfifo.openread();
	cout << "Open read" << endl;   
	fullChat = recfifo.recv();
	cout << "Received: " << fullChat << endl;
}

string sendMessage(string) {
	// Make sure chats have text
		if (fullChat.length() > 1) {
			chatVector.push_back(fullChat);
		
			sendfifo.openwrite();
			//for loop to send contents of vector to the ajax
			for(int i=0; i < chatVector.size(); i++) {
			
				cout << "Open write" << endl;
				sendfifo.send(chatVector[i]);
				cout << "Sending message " << i << endl;
				cout << chatVector[i];
			}   
		sendfifo.send("<!--$END-->");
		sendfifo.fifoclose();
		recfifo.fifoclose();
		}
		
}
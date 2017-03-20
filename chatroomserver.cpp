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

int main() {
	vector<string> chatVector;
	string user, message, fullChat;
	
    // create the FIFOs for communication
	Fifo recfifo(receive_fifo);
	Fifo sendfifo(send_fifo);
//while loop to get results from ajax
	while(1) {
		fullChat = "";
		cout << "Getting fifo" << endl;

		/* Get a message from a client */
		recfifo.openread();
		cout << "Open read" << endl;
    
		fullChat = recfifo.recv();
		cout << "Received: " << fullChat << endl;
		//if statement to only push back chats with text
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
}










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

void buildVector(string);
void buildMap(string);
vector<string> indexSearch(string);
void sendVector(vector<string>);
void checkVector(string);

/* Fifo names */
string receive_fifo = "CRrequest";
string send_fifo = "CRreply";

// create the FIFOs for communication
	Fifo recfifo(receive_fifo);
	Fifo sendfifo(send_fifo);

const string messageDelineator = "~~";
map<string, vector<string> > directChatMap;
map<string, vector<string> >::iterator it;
vector<string> chatVector;
vector<string> onlineUsers;


int main() {
	
	string message, fullChat;
	
    
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
		if (fullChat.length() > 6) {
		string chatType = fullChat.substr(fullChat.length()-13, fullChat.length());
		if (chatType == "directMessage") {
		buildMap(fullChat);

		}
		else {
		buildVector(fullChat);
		}
		
    
		}
    }
}


void buildVector(string fullChat) {
chatVector.push_back(fullChat);
		if (chatVector.size() >= 100) {
		chatVector.erase(chatVector.begin());
		}
		sendVector(chatVector);
}

void buildMap(string fullChat) {
		cout << "direct message: " << endl;
		size_t userPos = fullChat.find_first_of(messageDelineator);
		string user = (fullChat.substr(2, userPos));
		directChatMap[user].push_back(fullChat);
		if (directChatMap[user].size() >= 100) {
		directChatMap[user].erase(chatVector.begin());
		}
		sendVector(indexSearch(user));
}


vector<string> indexSearch(string user) {
vector<string> blank;
it = directChatMap.find(user);
if (it == directChatMap.end()) {
        return (blank);
    } else {
        return (directChatMap[user]);
    }
}

void sendVector(vector<string> chats) {
sendfifo.openwrite();
    //for loop to send contents of vector to the ajax
		for(int i=0; i < chats.size(); i++) {
			
		cout << "Open write" << endl;
			sendfifo.send(chats[i]);
			cout << "Sending message " << i << endl;
			cout << chats[i];
		}   
		sendfifo.send("<!--$END-->");
		sendfifo.fifoclose();
		recfifo.fifoclose();
}

void checkVector(string user) {
int numOfUsers = onlineUsers.size();
for (int i=0; i < numOfUsers; i++) {
if (onlineUsers[i]==user) {
return;
}
}
if (onlineUsers[numOfUsers] != user) {
onlineUsers.push_back(user);
}
}

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

void buildVector(string, Fifo, Fifo);
void buildMap(string, Fifo, Fifo);
vector<string> indexSearch(string);
void sendVector(vector<string>, Fifo, Fifo);
void checkVector(string);

/* Fifo names */
string receive_fifo = "CRrequest";
string send_fifo = "CRreply";





const string messageDelineator = "~~";
map<string, vector<string> > directChatMap;
map<string, vector<string> >::iterator it;
vector<string> chatVector;
vector<string> onlineUsers;


int main() {

Fifo recfifo(receive_fifo);
	Fifo sendfifo(send_fifo);
	
	string message, fullChat;
	
    // create the FIFOs for communication
//while loop to get results from ajax
	while(1) {
		fullChat = "";
		cout << "Getting fifo" << endl;

		/* Get a message from a client */
		recfifo.openread();
		cout << "Open read" << endl;
    
		fullChat = recfifo.recv();
		cout << "Received: " << fullChat << endl;
		
		recfifo.fifoclose();
		
		//if statement to only push back chats with text
		if (fullChat.length() > 6) {
		string chatType = fullChat.substr(fullChat.length()-13, fullChat.length());
		if (chatType == "directMessage") {
		cout << "buildMap" << endl;
		buildMap(fullChat, sendfifo, recfifo);

		}
		else {
		buildVector(fullChat, sendfifo, recfifo);
		cout << "buildVector" << endl;
		}
		
    
		}
    }
}


void buildVector(string fullChat, Fifo sendfifo, Fifo recfifo) {
chatVector.push_back(fullChat);
		if (chatVector.size() >= 100) {
		chatVector.erase(chatVector.begin());
		}
		sendVector(chatVector, sendfifo, recfifo);
}

void buildMap(string fullChat, Fifo sendfifo, Fifo recfifo) {
		cout << "direct message: " << endl;
		size_t userPos = fullChat.find_first_of(messageDelineator);
		string user = (fullChat.substr(2, userPos));
		checkVector(user);
		directChatMap[user].push_back(fullChat);
		if (directChatMap[user].size() >= 100) {
		directChatMap[user].erase(chatVector.begin());
		}
		sendVector(indexSearch(user), sendfifo, recfifo);
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

void sendVector(vector<string> chats, Fifo sendfifo, Fifo recfifo) {
	    
sendfifo.openwrite();
cout << "Open write" << endl;
    //for loop to send contents of vector to the ajax
		for(int i=0; i < chats.size(); i++) {
			
		
			sendfifo.send(chats[i]);
			cout << "Sending message " << i << endl;
			cout << chats[i] << endl;
		}   
		sendfifo.send("<!--$END-->");
		sendfifo.fifoclose();
		cout << "fifo sent" << endl;
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

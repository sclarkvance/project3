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
#include <pthread.h>

using namespace std;

void buildVector(string, Fifo, Fifo);
void buildMap(string, Fifo, Fifo);
vector<string> indexSearch(string);
void sendVector(vector<string>, Fifo, Fifo);
void checkVector(string);
void *getDirectResponse(void *);
void *getRoomResponse(void *);

/* Fifo names */
string receive_fifo_room = "CRrequest";
string send_fifo_room = "CRreply";

string receive_fifo_direct = "DCrequest";
string send_fifo_direct = "DCreply";





const string messageDelineator = "~~";
map<string, vector<string> > directChatMap;
map<string, vector<string> >::iterator it;
vector<string> chatVector;
vector<string> onlineUsers;

Fifo recfiforoom(receive_fifo_room);
	Fifo sendfiforoom(send_fifo_room);
	
	Fifo recfifodirect(receive_fifo_direct);
	Fifo sendfifodirect(send_fifo_direct);


int main() {
void *status;
void *statustwo;
pthread_t threads[2];
int rc;
int rcTwo;
	string message, fullChatRoom, fullChatDirect;
	
while(1) {
    cout << "main() : creating thread for direct chat responses" << endl;
    rc = pthread_create(&threads[0], NULL, 
			getDirectResponse, NULL);
    if (rc){
      cout << "Error:unable to create thread," << rc << endl;
      exit(-1);
    }
    
     cout << "main() : creating thread for chat room responses" << endl;
    rcTwo = pthread_create(&threads[1], NULL, 
			getRoomResponse, NULL);
    if (rcTwo){
      cout << "Error:unable to create thread," << rcTwo << endl;
      exit(-1);
    }
    
    int rcJoin = pthread_join(threads[0], &status);
    if (rcJoin) {
     cout << "ERROR; return code from pthread_join() is: " << rc << endl;
      exit(-1);
    }
    
    int rcJoinTwo = pthread_join(threads[1], &statustwo);
    if (rcJoinTwo) {
    cout << "ERROR; return code from pthread_join() is: " << rcTwo << endl;
      exit(-1);
    }

  pthread_exit(NULL);
	
}
}

void *getDirectResponse(void *unused) {
string fullChatDirect = "";
		cout << "Getting fifo direct" << endl;

		/* Get a message from a client */
		recfifodirect.openread();
		cout << "Open read direct" << endl;
    
		fullChatDirect = recfifodirect.recv();
		cout << "Direct Received: " << fullChatDirect << endl;
		
		recfifodirect.fifoclose();
		
		
		//if statement to only push back chats with text
		
		
		if (fullChatDirect.length() > 6) {
		string chatType = fullChatDirect.substr(fullChatDirect.length()-13, fullChatDirect.length());
		cout << "buildMap" << endl;
		buildMap(fullChatDirect, sendfifodirect, recfifodirect);
		}
		pthread_exit((void*) unused);
		}


void *getRoomResponse(void *unused) {

string fullChatRoom = "";

cout << "Getting fifo room" << endl;

		/* Get a message from a client */
		recfiforoom.openread();
		cout << "Open read room" << endl;
    
		fullChatRoom = recfiforoom.recv();
		cout << "Room Received: " << fullChatRoom << endl;
		
		recfiforoom.fifoclose();
		
		if (fullChatRoom.length() > 6) {
		string chatType = fullChatRoom.substr(fullChatRoom.length()-13, fullChatRoom.length());
		buildVector(fullChatRoom, sendfiforoom, recfiforoom);
		cout << "buildVector" << endl;
		}
		pthread_exit((void*) unused);
}


void buildVector(string fullChatRoom, Fifo sendfiforoom, Fifo recfiforoom) {
chatVector.push_back(fullChatRoom);
		if (chatVector.size() >= 100) {
		chatVector.erase(chatVector.begin());
		}
		sendVector(chatVector, sendfiforoom, recfiforoom);
}

void buildMap(string fullChatDirect, Fifo sendfifodirect, Fifo recfifodirect) {
		cout << "direct message: " << endl;
		size_t userPos = fullChatDirect.find_first_of(messageDelineator);
		string user = (fullChatDirect.substr(2, userPos));
		checkVector(user);
		directChatMap[user].push_back(fullChatDirect);
		if (directChatMap[user].size() >= 100) {
		directChatMap[user].erase(chatVector.begin());
		}
		
		
		vector<string> directVector = indexSearch(user);
		sendfifodirect.openwrite();
cout << "Open write" << endl;
    //for loop to send contents of vector to the ajax
		for(int i=0; i < (directVector.size()-1); i++) {
			
		
			sendfifodirect.send(directVector[i]);
			cout << "Sending message " << i << endl;
			cout << directVector[i] << endl;
		}   
		sendfifodirect.send("<!--$END-->");
		sendfifodirect.fifoclose();
		cout << "fifo sent" << endl;
		
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

void sendVector(vector<string> chats, Fifo sendfiforoom, Fifo recfiforoom) {
	    
sendfiforoom.openwrite();
cout << "Open write" << endl;
    //for loop to send contents of vector to the ajax
		for(int i=0; (i < chats.size()-1); i++) {
			
		
			sendfiforoom.send(chats[i]);
			cout << "Sending message " << i << endl;
			cout << chats[i] << endl;
		}   
		sendfiforoom.send("<!--$END-->");
		sendfiforoom.fifoclose();
		cout << "fifo sent" << endl;
}

void checkVector(string user) {
int numOfUsers = onlineUsers.size();
for (int i=0; i < (numOfUsers-1); i++) {
if (onlineUsers[i]==user) {
return;
}
}
if (onlineUsers[numOfUsers] != user) {
onlineUsers.push_back(user);
}
}

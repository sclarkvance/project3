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

void pushMessage(string, string&, string&); 


/* Fifo names */
string receive_fifo = "CRrequest";
string send_fifo = "CRreply";


  
vector <string> chatVector;
int main() {
cout << "main started";

    // create the FIFOs for communication
  Fifo recfifo(receive_fifo);
  Fifo sendfifo(send_fifo);

    string user, message, fullChat;
    


while(1) {
cout << "started";

  /* Get a message from a client */
    recfifo.openread();
    
    fullChat = recfifo.recv();
    
	recfifo.fifoclose();
     pushMessage(fullChat, message, user);
    for(int a=0; a <= chatVector.size(); a++) {
    sendfifo.openwrite();
    sendfifo.send(chatVector[a]);
    sendfifo.fifoclose();
    }   
    
    }

}


void pushMessage(string fullChat, string& message, string& user) {
string chatMessage;
const string userDelineator = "&&";
const string messageDelineator = "~~";
  size_t userPos = fullChat.find_first_of(userDelineator);
size_t messagePos = fullChat.find_first_of(messageDelineator);
  while (userPos != string::npos || messagePos != string::npos) { 
  if (userPos != string::npos) {
    //message[userPos] = ""; 
    if (userPos > messagePos) {
    string user = fullChat.substr(messagePos+1, userPos-1);
    }
    userPos = fullChat.find_first_of(userDelineator, userPos+1); 
    }
     if (messagePos != string::npos) {
         if (messagePos > userPos) {
    message = fullChat.substr(userPos+1,messagePos-1);
    }
    //message[messagePos] = ""; 
    messagePos = fullChat.find_first_of(messageDelineator, messagePos+1); 
    }
    chatMessage = user + ": " + message;
    chatVector.push_back(chatMessage);
  }


}




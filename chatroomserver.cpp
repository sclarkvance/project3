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


  

int main() {
vector <string> chatVector;
    // create the FIFOs for communication
  Fifo recfifo(receive_fifo);
  Fifo sendfifo(send_fifo);

    string user, message, fullChat;
    


while(1) {

  /* Get a message from a client */
    recfifo.openread();
    
    fullChat = recfifo.recv();
    
	recfifo.fifoclose();
      chatVector.push_back(fullChat);
    for(int a=0; a <= chatVector.size(); a++) {
    sendfifo.openwrite();
    sendfifo.send(chatVector[a]);
    sendfifo.fifoclose();
    }   
    
    }

}








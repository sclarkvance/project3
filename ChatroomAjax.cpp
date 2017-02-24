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

using namespace std;
using namespace cgicc; // Needed for AJAX functions.


// fifo for communication
string receive_fifo = "CRreply";
string send_fifo = "CRrequest";

int main() {
  Cgicc cgi;    // Ajax object
  char *cstr;
  // Create AJAX objects to recieve information from web page.
  form_iterator user = cgi.getElement("user");
  form_iterator message = cgi.getElemtn("message");

  // create the FIFOs for communication
  Fifo recfifo(receive_fifo);
  Fifo sendfifo(send_fifo);
  


  // Call server to get results
  string user = **user;
string message = **message;
  string ajaxmessage =  "&&"+user+"~~"+message;
  sendfifo.openwrite();
  sendfifo.send(ajaxmessage);
  
  /* Get a message from a server */
  recfifo.openread();
  string results = recfifo.recv();
  recfifo.fifoclose();
  sendfifo.fifoclose();
  cout << "Content-Type: text/plain\n\n";

  cout << results;
  
return 0;
}

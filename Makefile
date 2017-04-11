#MakeFile to build and deploy the Sample US CENSUS Name Data using ajax
# For CSC3004 Software Development

# Put your user name below:
USER= vances

CC= g++

#For Optimization
#CFLAGS= -O2
#For debugging
CFLAGS= -g
FLAGS =  -lpthread -std=c++11

RM= /bin/rm -f

all: chatroomserver chatroomajax directchatajax PutCGI PutHTML
#all: chatroomserver

chatroomserver.o: chatroomserver.cpp fifo.h
	$(CC) -c $(CFLAGS) chatroomserver.cpp 

chatroomajax.o: chatroomajax.cpp fifo.h
	$(CC) -c $(CFLAGS) chatroomajax.cpp
	
directchatajax.o: directchatajax.cpp fifo.h
	$(CC) -c $(CFLAGS) directchatajax.cpp


chatroomserver: chatroomserver.o fifo.o
	$(CC) chatroomserver.o  fifo.o -o chatroomserver $(FLAGS)

fifo.o:		fifo.cpp fifo.h
		g++ -c fifo.cpp
		
chatroomajax: chatroomajax.o  fifo.o
	$(CC) chatroomajax.o  fifo.o -o chatroomajax -L/usr/local/lib -lcgicc

directchatajax: directchatajax.o fifo.o	
	$(CC) directchatajax.o  fifo.o -o directchatajax -L/usr/local/lib -lcgicc


PutCGI: 
	chmod 757 chatroomajax 
	chmod 757 directchatajax
	cp chatroomajax /usr/lib/cgi-bin/$(USER)_chatroomajax.cgi 
	cp directchatajax /usr/lib/cgi-bin/$(USER)_directchatajax.cgi 

	echo "Current contents of your cgi-bin directory: "
	ls -l /usr/lib/cgi-bin/
	
PutHTML:
	cp chatroom_ajax.html /var/www/html/class/softdev/$(USER)
	cp chatroom_ajax.css /var/www/html/class/softdev/$(USER)
	cp chatroomjs.js /var/www/html/class/softdev/$(USER)

	echo "Current contents of your HTML directory: "
	ls -l /var/www/html/class/softdev/$(USER)

clean:
	rm -f *.o chatroom_ajax directchat_ajax chatroomserver chatroomajax directchatajax

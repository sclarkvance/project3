var XMLHttp;
var cgiFlag = false;
var chatType;

// Things to do at page load
function pageInit() {
if(navigator.appName == "Microsoft Internet Explorer") {
    XMLHttp = new ActiveXObject("Microsoft.XMLHTTP");
} else {
    XMLHttp = new XMLHttpRequest();
}

	AutoRefresh();
}

// Function to force scrollable window at bottom
function updateScroll() {
    	var element = document.getElementById("boxtext");
    	element.scrollTop = element.scrollHeight;
}

// Make user box read only
function makeReadOnly() {
    	document.getElementById('user').readOnly = true;
} 
  
// Get user and message from input forms
function getUser (chatType) {

	var message = document.getElementById(chatType).value;
	var user = document.getElementById('user').value;
	if (message.length < 0 || user.length < 0) {
	return;
	}
	if(cgiFlag == false) { XMLHttp.open("GET", "/cgi-bin/vances_chatroomajax.cgi?"
						+ "&user=" + user
						+ "&message=" + message
						+ "&chatType=" + chatType 
						,true);
						cgiFlag = true; 
	XMLHttp.onreadystatechange=function() {
	if (XMLHttp.readyState == 4) {
		// Clear the input text
		document.getElementById(chatType).value = "";
		// Force to bottom
		//updateScroll();
		cgiFlag = false;
    	}
    	}
	XMLHttp.send(null);   
	} 	
	makeReadOnly();
}

// Make sure message doesn't contain the delineators
function validateMessage(chatType) {
	var message = document.getElementById(chatType).value;
	if (message.indexOf('&&') > -1)
	{
		alert("Message may not contain '&&' ");
	}
}

// Take chats from the chat vector and output them
function getChats(chatType) {

	if (cgiFlag == false) { XMLHttp.open("GET", "/cgi-bin/vances_chatroomajax.cgi?");
	cgiFlag = true;
	XMLHttp.onreadystatechange=function() {
	if (XMLHttp.readyState == 4) {
    		document.getElementById(chatType).value = XMLHttp.responseText;
    	cgiFlag = false;
    	}
    	}
	XMLHttp.send(null);
	}
}

var intVar;
// Function to refresh chats every second
function AutoRefresh() {
if (cgiFlag == false && XMLHttp.readyState == 4) {
cgiFlag = true;
	intVar = setInterval(function(){ getChats("directBoxText"); getChats("roomBoxText")}, 2000);
	cgiFlag = false;
	}
}

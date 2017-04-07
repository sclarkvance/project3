var XMLHttp;
var cgiFlag = false;
var chatType = "";

// Things to do at page load
function pageInit() {
if(navigator.appName == "Microsoft Internet Explorer") {
    XMLHttp = new ActiveXObject("Microsoft.XMLHTTP");
} else {
    XMLHttp = new XMLHttpRequest();
}
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
alert("getuser initiated");
	var message = document.getElementById(chatType).value;
	alert(message);
	var user = document.getElementById('user').value;
	alert(user);
	if (message.length < 0 || user.length < 0) {
	alert("return");
	return;
	}
	alert(cgiFlag);
	if(cgiFlag == false) { 
	cgiFlag = true;
	XMLHttp.open("GET", "/cgi-bin/vances_chatroomajax.cgi?"
						+ "&user=" + user
						+ "&message=" + message
						+ "&chatType=" + chatType 
						,true);
						 
	XMLHttp.onreadystatechange=function() {
	alert("statechange--getuser");
	alert(XMLHttp.readyState);
	if (XMLHttp.readyState == 4) {
	alert("ready state is 4!!!");
		// Clear the input text
		document.getElementById(chatType).value = "";
		// Force to bottom
		//updateScroll();
		cgiFlag = false;
    	}
	} 	
		
	alert(XMLHttp.readyState);
	alert("getuser sent"); 
	makeReadOnly();
	XMLHttp.send(null);
	}
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
	if (cgiFlag == false) {
		cgiFlag = true;
	 XMLHttp.open("GET", "/cgi-bin/vances_chatroomajax.cgi?", true);
	XMLHttp.onreadystatechange=function() {
	alert("readystate change");
	if (XMLHttp.readyState == 4) {
    		document.getElementById(chatType).innerHTML = XMLHttp.responseText;
    	
    	alert("get chats is working");
    	cgiFlag=false;
    	if (chatType != "directBoxText") getChats("roomBoxText");
    	}

	}
	
	alert("getCHats sent");
	cgiFlag = false;
	XMLHttp.send(null);
	
}
}



var intVar;
// Function to refresh chats every second
function AutoRefresh() {
//if (XMLHttp.readyState == 4 && XMLHttp.status == 200) {
//cgiFlag = true;
	intVar = setInterval(function(){ getChats("directBoxText");}, 2000);
	alert("autorefresh working");
	//cgiFlag = false;
//	}
}

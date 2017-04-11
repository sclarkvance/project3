var XMLHttpCR;
var XMLHttpDC;
var cgiFlag = false;
var cgiFlagDirect = false;
var chatType;

// Things to do at page load
function pageInit() {
console.log("page init");
alert("page init");
if(navigator.appName == "Microsoft Internet Explorer") {
    XMLHttpCR = new ActiveXObject("Microsoft.XMLHTTP");
    XMLHttpDC = new ActiveXObject("Microsoft.XMLHTTP");
} else {
    XMLHttpCR = new XMLHttpRequest();
    XMLHttpDC = new XMLHttpRequest();
}
}

// Function to force scrollable window at bottom
//function updateScroll() {
 //   	var element = document.getElementById("boxtext");
   // 	element.scrollTop = element.scrollHeight;
//}

// Make user box read only
function makeReadOnly() {
    	document.getElementById('user').readOnly = true;
} 
  
// Get user and message from input forms
function getUser(chatType) {
alert("getuser initiated");
alert(chatType);
//alert("getuser initiated");
	var message = document.getElementById(chatType).value;
	var user = document.getElementById('user').value;
	alert(message);
	alert(chatType);
	alert(cgiFlag);
	if (message.length < 0 || user.length < 0) {
	alert("return");
	return;
	}
	if(cgiFlag == false && chatType == "roomMessage") {
	alert("ROOM MESSAGE");
	cgiFlag = true;
		XMLHttpCR.open("GET", "/cgi-bin/vances_chatroomajax.cgi?"
						+ "&user=" + user
						+ "&message=" + message
						+ "&chatType=" + chatType 
						,true);
						alert(message);
						 alert("xml");
						 alert(XMLHttpCR.readyState);
	XMLHttpCR.onreadystatechange=function() {
	alert("statechange--getuser");
	alert(XMLHttpCR.readyState);
	if (XMLHttpCR.readyState == 4) {
	//alert("ready state is 4!!!");
		// Clear the input text
		document.getElementById(chatType).value = "";
		alert(XMLHttpCR.readyState);
	alert("getuser sent- room"); 
	makeReadOnly();
	
		// Force to bottom
		//updateScroll();
		cgiFlag = false;
    	}
	}
	XMLHttpCR.send(null); 
	}
	
	if(cgiFlagDirect == false && chatType == "directMessage") {
	//alert("DRIECT MESSAGE");
	cgiFlagDirect = true;
			//alert(user);		
				XMLHttpDC.open("GET", "/cgi-bin/vances_directchatajax.cgi?"
						+ "&user=" + user
						+ "&message=" + message
						+ "&chatType=" + chatType 
						,true);	 
	XMLHttpDC.onreadystatechange=function() {
//	alert("statechange--getuser");
//	alert(XMLHttpDC.readyState);
	if (XMLHttpDC.readyState == 4) {
	//alert("ready state is 4!!!");
		// Clear the input text
		document.getElementById(chatType).value = "";
		// Force to bottom
		//updateScroll();
	//	alert(XMLHttpDC.readyState);
	//alert("getuser sent- direct"); 
	makeReadOnly();
		cgiFlagDirect = false;
    	}
	} 
	//alert(XMLHttpDC.readyState);
		//	alert("readystate skipped");
	
		XMLHttpDC.send(null);	
	
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
function getChats() {
alert("Get CHats");
if(cgiFlag == true) return;
	if (cgiFlag == false) {
		cgiFlag = true;
		alert("getChats running");
				 XMLHttpCR.open("GET", "/cgi-bin/vances_chatroomajax.cgi?", true);
	XMLHttpCR.onreadystatechange=function() {
	alert("readystate change");
	if (XMLHttpCR.readyState == 4) {
	alert(XMLHttpCR.responseText);
    		document.getElementById('roomBoxText').innerHTML = XMLHttpCR.responseText;
    	
    	alert("get chats is working");
    	cgiFlag=false;
    	}
alert("getCHats sent");
	}
	XMLHttpCR.send(null);
}
}


function getDirect() {
alert("get direct started");
if(cgiFlagDirect == true) return;
	if (cgiFlagDirect == false) {
		 XMLHttpDC.open("GET", "/cgi-bin/vances_directchatajax.cgi?", true);
		cgiFlagDirect = true;
		alert("getChats running");
	XMLHttpDC.onreadystatechange=function() {
    alert("readystate change");
	if (XMLHttpDC.readyState == 4) {
    		document.getElementById('directBoxText').innerHTML = XMLHttpDC.responseText;
    	
    	alert("get chats is working");
    	cgiFlagDirect=false;
    	}
	alert("getCHats sent");

	}
	XMLHttpDC.send(null);
}
}



var intVar;
// Function to refresh chats every second
function AutoRefresh() {
//if (XMLHttp.readyState == 4 && XMLHttp.status == 200) {
//cgiFlag = true;
	intVar = setInterval(function(){ getChats(); getDirect();}, 2000);
	//cgiFlag = false;
//	}
}

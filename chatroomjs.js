var XMLHttp;

if(navigator.appName == "Microsoft Internet Explorer") {
    XMLHttp = new ActiveXObject("Microsoft.XMLHTTP");
} else {
    XMLHttp = new XMLHttpRequest();
}

// Things to do at page load
function pageInit() {
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
function getUser () {
	var message = document.getElementById('message').value;
	var user = document.getElementById('user').value;
	XMLHttp.open("GET", "/cgi-bin/vances_chatroomajax.cgi?"
						+ "&user=" + user
						+ "&message=" + message 
						,true);
	XMLHttp.onreadystatechange=function() {
		// Clear the input text
		document.getElementById("message").value = "";
		// Force to bottom
		updateScroll();
    	}
	XMLHttp.send(null);
}

// Make sure message doesn't contain the delineators
function validateMessage() {
	var message = document.getElementById("message").value;
	if (message.indexOf('&&') > -1)
	{
		alert("Message may not contain '&&' ");
	}
}

// Take chats from the chat vector and output them
function getChats() {
	XMLHttp.open("GET", "/cgi-bin/vances_chatroomajax.cgi?");
	XMLHttp.onreadystatechange=function() {
    		document.getElementById('boxtext').innerHTML = XMLHttp.responseText;
    	}
	XMLHttp.send(null);
}

var intVar;
// Function to refresh chats every second
function AutoRefresh() {
	intVar = setInterval(function(){ getChats()}, 1000);
}
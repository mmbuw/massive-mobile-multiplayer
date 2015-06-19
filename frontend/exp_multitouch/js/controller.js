/****************************************************************************************************************
  OnLoad
****************************************************************************************************************/
checkLogin();

window.addEventListener('load', function(){

		controller= document.getElementById('controller');
		button = document.getElementById('button');
		wrapper = document.getElementById('canvas-wrap');
		canvas = document.getElementById('sadstick');
		circle = document.getElementById('circle');
		hint = document.getElementById('mmmturn');

		colorbase = 'grey';
		idletimer = setTimeout(logout, 3000000);
		context = null;
		socket = null;
		startx = null;
		starty = null;
		socket = null;

		initView();
		initSocket();

		window.addEventListener('resize', updateBasicView, false);

		button.addEventListener('touchstart', pushButton, false);
		button.addEventListener('touchend', releaseButton, false);

		circle.addEventListener('touchstart', pushCircle, false);
		circle.addEventListener('touchmove', moveCircle, false);
		circle.addEventListener('touchend', releaseCircle, false);

}, false)


/****************************************************************************************************************
  Controller Basic View, Functions
****************************************************************************************************************/


function isLandscape() {
		var width = window.innerWidth;
		var height = window.innerHeight;

		if (width < height) {
			hint.style.display="block";
			button.style.display="none";
			circle.style.display="none";
			canvas.style.display="none";
		} else {
			hint.style.display="none";
			button.style.display="";
			circle.style.display="";
			canvas.style.display="";
		}
}

function initView() {
		isLandscape();
		context = canvas.getContext('2d');
	 	canvas.width =  window.innerWidth/2;
 		canvas.height = window.innerHeight;
 		var canvasCenterX = canvas.width /2;
 		var canvasCenterY = canvas.height /2;
 		circle.style.left = canvasCenterX - circle.offsetWidth/2;
 		circle.style.top = canvasCenterY - circle.offsetHeight/2;
}

function updateBasicView() {
		isLandscape();
	    canvas.width =  window.innerWidth/2;
 		canvas.height = window.innerHeight;
 		var canvasCenterX = canvas.width /2;
 		var canvasCenterY = canvas.height /2;
 		circle.style.left = canvasCenterX - circle.offsetWidth/2;
 		circle.style.top = canvasCenterY - circle.offsetHeight/2;
}

function colorBackground(object, color) {
	object.style.backgroundColor = color;
}

function resetTimer() {
	clearTimeout(idletimer);
	idletimer = setTimeout(logout, 30000);
}

/****************************************************************************************************************
  Canvas Draw
****************************************************************************************************************/
function drawControllerStick(currx, curry) {
		clearCanvas();
		drawLineFromCenterTo(currx, curry);
		drawCircleAtPos(currx,curry);
}

function drawLineFromCenterTo(currx, curry) {
	 	var canvasCenterX = canvas.width /2;
 		var canvasCenterY = canvas.height /2;
		context.beginPath();
 		context.moveTo(canvasCenterX ,canvasCenterY);
	    context.lineTo(currx, curry);
	    context.closePath();
	    context.stroke();
}

function drawCircleAtPos(currx, curry) {
		context.beginPath();
	    context.arc(currx, curry, 50, 0, 2 * Math.PI, false);
	    context.fillStyle = 'black';
		context.fill();
		context.lineWidth = 5;
	    context.closePath();
	    context.stroke();
}

function clearCanvas() {
		context.clearRect(0, 0, canvas.width, canvas.height);
		context.lineWidth = 4;
	    context.shadowColor = "transparent";
}


/****************************************************************************************************************
  Touchevents
****************************************************************************************************************/

function pushButton(e) {
	e.preventDefault();
	colorBackground(button,"#000000");
	resetTimer();
	socketSend(socket, 'K A 1');
	socketSend(socket, 'K A 0');
}

function releaseButton() {
	colorBackground(button,"#333333");
}

function pushCircle(e) {
	e.preventDefault();
	var touchobj = e.changedTouches[0];
	startx = parseInt(touchobj.clientX);
	starty = parseInt(touchobj.clientX);
	circle.style.display ="none";

	var canvasCenterX = canvas.width /2;
	var canvasCenterY = canvas.height /2;
	drawControllerStick(canvasCenterX,canvasCenterY);

	socketSend(socket, 'V * '+ 0 + ' ' + 0);
	resetTimer();
}

function moveCircle(e) {
	e.preventDefault();
	var touchobj = e.changedTouches[0];
	var currx = parseInt(touchobj.clientX);
	var curry = parseInt(touchobj.clientY);
	drawControllerStick(currx,curry);

	diffx = currx - startx;
	diffy = curry - starty;

	sentToSocket(diffx,diffy);
	resetTimer();
	socketSend(socket, 'V * '+ diffx + ' ' + diffy);
}

function releaseCircle(e) {
	circle.style.display = "";
	clearCanvas();
	socketSend(socket, 'V * '+ 0 + ' ' + 0);
}

/****************************************************************************************************************
  Socket
****************************************************************************************************************/
function initSocket() {
	//socket = new WebSocket("ws://29.4.93.1:53000");
	socket = new WebSocket("ws://localhost:53000");

	socket.onerror = function(error) {
 		window.location.href = './leaving.html';
	};

	socket.onopen = function() { 
		socketSend(socket, 'NAME '+sessionStorage.getItem('playername'));
		window.sessionStorage.removeItem('playername');
	}

	socket.onmessage = function(evt) {
		var splitted_string = evt.data.split(" ");

		window.sessionStorage.setItem("team", splitted_string[1]);
		window.sessionStorage.setItem("number", splitted_string[2]);

		if(sessionStorage.getItem('team') == "RED") {
	    	colorbase = 'red';
	    	button.style.backgroundColor = colorbase;
	    	circle.style.backgroundColor = colorbase;

	    } else if(sessionStorage.getItem('team') == "BLUE") {
	    	colorbase = 'blue';
	    	button.style.backgroundColor = colorbase;
	    	circle.style.backgroundColor = colorbase;
	    }	
	}
}

function socketSend(socket, message) {
	messageToSend = '^' + message + '$';
	socket.send(messageToSend);
}

function closeSocket() {
	if (socket.readyState == 1) {
		socket.close();
	}
}

/****************************************************************************************************************
  Logout, check, forward
****************************************************************************************************************/
function checkLogin() {
	if(sessionStorage.getItem('playername') === null) {
		window.location.href = './leaving.html';
	}
}

function logout(){
	window.sessionStorage.removeItem('team');
	window.sessionStorage.removeItem('number');
	closeSocket();
	window.location.href = './leaving.html';
}

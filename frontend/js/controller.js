/****************************************************************************************************************
  OnLoad
****************************************************************************************************************/
redirectOnError = true;
checkLogin();

window.addEventListener('load', function(){

		controller= document.getElementById('controller');
		button = document.getElementById('button');
		wrapper = document.getElementById('canvas-wrap');
		canvas = document.getElementById('sadstick');
		hint = document.getElementById('mmmturn');

		idletimer = setTimeout(logout, 3000000);
		context = null;
		socket = null;
		startx = null;
		starty = null;
		socket = null;

		stickRadius = 40;
		stickMaxDistance = 2 * stickRadius;
		teamColor = '#bbbbbb';
		stickIsDragged = false;
		sendInputMaxValue = 255;

		initSocket();

		window.addEventListener('resize', onWindowResize, false);
		onWindowResize();

		button.addEventListener('touchstart', pushButton, false);
		button.addEventListener('touchend', releaseButton, false);

		canvas.addEventListener('touchstart', touchCanvas, false);
		canvas.addEventListener('touchmove', moveTouchOnCanvas, false);
		canvas.addEventListener('touchend', releaseCanvasTouch, false);
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
		canvas.style.display="none";
		socketSend(socket, 'V * '+ 0 + ' ' + 0);
	} else {
		hint.style.display="none";
		button.style.display="inline";
		canvas.style.display="inline";
	}
}

function onWindowResize() {
	isLandscape();
	setCanvasSize();
	clearCanvas();
}

function setCanvasSize() {
	context = canvas.getContext('2d');
	canvas.width =  window.innerWidth/2;
	canvas.height = window.innerHeight;
	canvasCenter = [canvas.width/2, canvas.height/2];
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
function drawControllerStick(pos, color) {
	clearCanvas();
	drawLineFromCenterTo(pos);
	drawCircleAtPos(pos, stickRadius, color);
}

function drawLineFromCenterTo(pos) {
	context.beginPath();
	context.moveTo(canvasCenter[0],canvasCenter[1]);
	context.lineTo(pos[0], pos[1]);
	context.closePath();
	context.stroke();
}

function drawCircleAtPos(pos, radius, color) {
	context.beginPath();
	context.arc(pos[0], pos[1], radius, 0, 2 * Math.PI, false);
	context.fillStyle = color;
	context.fill();
	context.lineWidth = 3;
	context.closePath();
	context.stroke();
}

function clearCanvas() {
	context.clearRect(0, 0, canvas.width, canvas.height);
	context.lineWidth = 4;
	context.shadowColor = "transparent";
	drawCircleAtPos(canvasCenter, stickRadius*0.25, '#bbbbbb');
	if (!stickIsDragged) drawCircleAtPos(canvasCenter, stickRadius, teamColor);
	drawCircleAtPos(canvasCenter, stickMaxDistance, "transparent");
}


/****************************************************************************************************************
  Touchevents
****************************************************************************************************************/

function pushButton(e) {
	e.preventDefault();
	colorBackground(button,'#ffffff');
	resetTimer();
	socketSend(socket, 'K A 1');
	socketSend(socket, 'K A 0');
}

function releaseButton() {
	colorBackground(button, teamColor);
}

function touchCanvas(e) {
	e.preventDefault();
	var touchobj = e.changedTouches[0];
	var touchCoords = [parseInt(touchobj.clientX), parseInt(touchobj.clientY)];
    var touchToCenter = [canvasCenter[0]-touchCoords[0], canvasCenter[1]-touchCoords[1]];
	var touchDistance = Math.sqrt(touchToCenter[0]*touchToCenter[0] + touchToCenter[1]*touchToCenter[1]);

	if (touchDistance < stickRadius)
	{
		stickIsDragged = true;
		stickDraggingOffset = touchToCenter;
		drawControllerStick(canvasCenter, teamColor);
		socketSend(socket, 'V * '+ 0 + ' ' + 0);
	}

	resetTimer();
}

function moveTouchOnCanvas(e) {

	if (stickIsDragged)
	{
		var touchobj = e.changedTouches[0];
		var touchCoords = [parseInt(touchobj.clientX), parseInt(touchobj.clientY)];
		var stickCoords = [touchCoords[0]+stickDraggingOffset[0], touchCoords[1]+stickDraggingOffset[1]];
		var centerToStick = [stickCoords[0]-canvasCenter[0], stickCoords[1]-canvasCenter[1]];
		var centerToStickMag = Math.sqrt(centerToStick[0]*centerToStick[0] + centerToStick[1]*centerToStick[1]);

		if (centerToStickMag > stickMaxDistance)
		{
			var shorteningFactor = stickMaxDistance/centerToStickMag;
			centerToStick = [centerToStick[0]*shorteningFactor, centerToStick[1]*shorteningFactor];
			centerToStickMag = stickMaxDistance;
			stickCoords = [canvasCenter[0]+centerToStick[0], canvasCenter[1]+centerToStick[1]];
		}
		drawControllerStick(stickCoords, teamColor);

		var normalizedStickCoords = [centerToStick[0]/stickMaxDistance, centerToStick[1]/stickMaxDistance];
		var inputToSend = [Math.round(normalizedStickCoords[0]*sendInputMaxValue), Math.round(normalizedStickCoords[1]*sendInputMaxValue)];
		socketSend(socket, 'V * '+ inputToSend[0] + ' ' + inputToSend[1]);
	}
	resetTimer();
}

function releaseCanvasTouch(e) {
	stickIsDragged = false;
	socketSend(socket, 'V * '+ 0 + ' ' + 0);
	clearCanvas();
}

/****************************************************************************************************************
  Socket
****************************************************************************************************************/
function initSocket() {
	
	socket = new WebSocket('ws://' + configuration.server_ip + ':' + configuration.server_port)
	
	socket.onerror = function(error) {
 		if (redirectOnError) window.location.href = './goodbye.html';
	};

	socket.onopen = function() { 
		socketSend(socket, 'NAME '+sessionStorage.getItem('playername'));
	}

	socket.onmessage = function(evt) {
		var splitted_string = evt.data.split(" ");

		window.sessionStorage.setItem("team", splitted_string[1]);
		window.sessionStorage.setItem("number", splitted_string[2]);

		if(sessionStorage.getItem('team') == "RED") {
	    	teamColor = 'red';
	    	button.style.backgroundColor = teamColor;
	    } else if(sessionStorage.getItem('team') == "BLUE") {
	    	teamColor = 'blue';
	    	button.style.backgroundColor = teamColor;
	    }

		button.innerHTML = "<div class='status-text'>Name: <span id='bold'>" + sessionStorage.getItem('playername') + "</span><br/><br/>Number: <span id='bold'>" + sessionStorage.getItem('number')  + "</span></div>";
		clearCanvas();
	}
}

function socketSend(socket, message) {
	messageToSend = '^' + message + '$';
	//console.log(messageToSend);
	if (socket.readyState == 1) {
		socket.send(messageToSend);
	}
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
		if (redirectOnError) window.location.href = './goodbye.html';
	}
}

function logout(){
	window.sessionStorage.removeItem('team');
	window.sessionStorage.removeItem('number');
	closeSocket();
	if (redirectOnError) window.location.href = './goodbye.html';
}


window.addEventListener('load', function(){
		//initialize timer
		var timer = setTimeout(endGame, 30000);
		// Websocket
		//var socket = new WebSocket("ws://29.4.93.1:53000");
		var socket = new WebSocket("ws://localhost:53000");
 
		// Nach dem öffnen des Sockets den Status anzeigen
		socket.onopen = function() 	
		{ 
			console.log('Socket Status: '+socket.readyState+' (open)');
			socket.send('NAME '+localStorage.getItem('playername')+'$');	
		}

		socket.onmessage = function(evt)
		{
			window.localStorage.setItem("team", evt.data);
			console.log(evt.data);
		}

 		//##################################################################################
 		//button control
	    var button = document.getElementById('button');
	 
	    button.addEventListener('touchstart', function(e){

	        button.style.backgroundColor = '#000000';
	       	e.preventDefault();

			//reset timer
	       	clearTimeout(timer);
	       	timer = setTimeout(endGame, 30000);

	       	//sentd to server
	       	socket.send('VAL A$');

	       	//console debug
	       	console.log('pushbutton-start');
	    }, false)
	 
	    button.addEventListener('touchend', function(e){
	    	button.style.backgroundColor = '#cccccc';
	       	e.preventDefault();

	       	//console debug
	       	console.log('pushbutton-end');
	    }, false)

		//##################################################################################
	    //steuernippel
	    var wrapper = document.getElementById('canvas-wrap');
 		var canvas = document.getElementById('sadstick');
 		var circle = document.getElementById('circle');

 		//initialize canvas with and height
 		var context = canvas.getContext('2d');
 		canvas.width =  wrapper.offsetWidth;
 		canvas.height = wrapper.offsetHeight;

 		//initialize midpoint
 		var centerX = canvas.width /2;
 		var centerY = canvas.height /2;

 		//initialize vars
 		var startx = 0;
	    var starty = 0;
	    var diffx = 0;
	    var diffy = 0;

	    //blurr fix
	   	context.translate(0.5, 0.5);

	    //set circle in center
 		circle.style.top = centerY - circle.offsetHeight/2;
 		circle.style.left = centerX -circle.offsetWidth/2;

 		//start touchinteraction on circle
 		circle.addEventListener('touchstart', function(e){
 			circle.style.backgroundColor = '#000000';
	        var touchobj = e.changedTouches[0];
	        startx = parseInt(touchobj.clientX);
	        starty = parseInt(touchobj.clientX);
	       	e.preventDefault();

	       	//sent to server
	       	socket.send('VAL ' +0+' '+0+'$');

	       	//console debug
	       	console.log('start'+startx+'/'+starty);
	    }, false)

 		//move circle and update canvas
	   	circle.addEventListener('touchmove', function(e){

	        var touchobj = e.changedTouches[0];
	        var currx = parseInt(touchobj.clientX);
	        var curry = parseInt(touchobj.clientY);

	        //clear canvas
	    	context.clearRect(0, 0, canvas.width, canvas.height);

	    	//line properties
	    	context.lineWidth = 4;
	    	context.shadowColor = "transparent";
	    	
	    	//draw line from center tu crrent position
	    	context.beginPath();
 			context.moveTo(centerX,centerY);
	    	context.lineTo(currx, curry);
	    	context.closePath();
	    	context.stroke();

	    	//calc movement
	    	diffx = currx - startx;
	    	diffy = curry - starty;

	    	//set circle to new position
	        circle.style.top = centerY - circle.offsetHeight/2 + diffy;
 			circle.style.left = centerX - circle.offsetWidth/2 + diffx;

	       	e.preventDefault();

	       	//reset timer
	       	clearTimeout(timer);
	       	timer = setTimeout(endGame, 30000);

	       	diffXsocket = Math.clip(Math.round((diffx/(canvas.width/2))*250)+250,0,250);
			diffYsocket = Math.clip(Math.round((diffy/(canvas.height/2))*250)+250,0,250);
	       	//sent to server
	       	socket.send('VAL '+ diffXsocket + ' ' + diffYsocket +'$');
	       	sleepFor(10);

	       	//console debug
	    	console.log('start: '+diffXsocket+'/'+diffYsocket);
	       	console.log('lineTo: '+Math.round((diffx/(canvas.width/2))*250)+'/'+canvas.height);

	    }, false)

	   	//touchend and reset joystick
	    circle.addEventListener('touchend', function(e){
	    	circle.style.backgroundColor = '#cccccc';
	    	//set circle to start
	        circle.style.top = centerY - circle.offsetHeight/2;
 			circle.style.left = centerX -circle.offsetWidth/2;

 			//clear canvas
 			context.clearRect(0, 0, canvas.width, canvas.height);
	       	e.preventDefault();
	    }, false)

}, false)

//close connection and forward	
function endGame(){
	//close socket
	//socket.close();
	//forward to controller
	window.location.href = './leaving.html';
}

function sleepFor( sleepDuration ){
    var now = new Date().getTime();
    while(new Date().getTime() < now + sleepDuration){ /* do nothing */ } 
}

//delete marker in string
function clearMarker(input){
	
	return input.replace('$','');
}

//clipping function
Math.clip = function(number, min, max) {
  return Math.max(min, Math.min(number, max));
}
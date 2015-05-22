window.addEventListener('load', function(){
		//global vars
		var timer = setTimeout(endGame, 30000);
		var colorbase;
		var socket;
		var button = document.getElementById('button');
		var wrapper = document.getElementById('canvas-wrap');
 		var canvas = document.getElementById('sadstick');
 		var circle = document.getElementById('circle');

 		//##################################################################################


		//Websocket
		//var socket = new WebSocket("ws://29.4.93.1:53000");

		var socket = new WebSocket("ws://localhost:53000");
 		

 		socket.onerror = function(error) {
 			window.location.href = './leaving.html';
		};
		// Nach dem öffnen des Sockets den Status anzeigen
		socket.onopen = function() 	
		{ 
			socket.send('NAME '+localStorage.getItem('playername')+'$');
		}
		//recive from server
		socket.onmessage = function(evt)
		{
			window.localStorage.setItem("team", evt.data);

			//set colorbase by socketinput/teamident
			if(localStorage.getItem('team').indexOf("RED") > -1) {
		    	colorbase = 'red';
		    	button.style.backgroundColor = colorbase;
		    	circle.style.backgroundColor = colorbase;

		    } else if(localStorage.getItem('team').indexOf("BLUE") > -1) {
		    	colorbase = 'blue';
		    	button.style.backgroundColor = colorbase;
		    	circle.style.backgroundColor = colorbase;

		    } 

		}


 		//##################################################################################
 		//button control
	 
	    button.addEventListener('touchstart', function(e){

	        button.style.backgroundColor = '#000000';
	       	e.preventDefault();

			//reset timer
	       	clearTimeout(timer);
	       	timer = setTimeout(endGame, 30000);

	       	//sentd to server
	       	socket.send('VAL A$');

	    }, false)
	 
	    button.addEventListener('touchend', function(e){
	    	button.style.backgroundColor = colorbase;
	       	e.preventDefault();

	    }, false)

		//##################################################################################

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
	    var diffxprev;
		var diffyprev;
		var interval;

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

	       	//sent to server
	       	var navtouchstate = 1;
			socket.send('VAL '+ 0 + ' ' + 0 +'$');

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
	    	
	    	//draw line from center to crrent position
	    	context.beginPath();
 			context.moveTo(centerX,centerY);
	    	context.lineTo(currx, curry);
	    	context.closePath();
	    	context.stroke();

			context.beginPath();
	    	context.arc(currx, curry, 50, 0, 2 * Math.PI, false);
	    	context.fillStyle = 'black';
		    context.fill();
		    context.lineWidth = 5;
	    	context.closePath();
	    	context.stroke();
			
			circle.style.display ="none";

	    	//calc movement
	    	diffx = currx - startx;
	    	diffy = curry - starty;

	    	//set circle to new position
	        //circle.style.top = centerY - circle.offsetHeight/2 + diffy;
 			//circle.style.left = centerX - circle.offsetWidth/2 + diffx;

	       	e.preventDefault();

	       	//reset timer
	       	clearTimeout(timer);
	       	timer = setTimeout(endGame, 30000);

	       	//sent to server
	       	socket.send('VAL '+ diffx + ' ' + diffy +'$');
	 

	    }, false)
	    
		 			        
 	  	//touchend and reset joystick
	    circle.addEventListener('touchend', function(e){
	    	circle.style.backgroundColor = colorbase;

 			//clear canvas
 			context.clearRect(0, 0, canvas.width, canvas.height);
	       	e.preventDefault();
	       	circle.style.display ="";

			socket.send('VAL '+ 0 + ' ' + 0 +'$');
	   	}, false)

		//##################################################################################
		//resize windows

	    $(window).resize(function() {
	    	canvas.width =  wrapper.offsetWidth;
 			canvas.height = wrapper.offsetHeight;
	    	centerX = canvas.width /2;
 			centerY = canvas.height /2;
	    	circle.style.top = centerY - circle.offsetHeight/2;
 			circle.style.left = centerX - circle.offsetWidth/2;
		});



		//##################################################################################
		//forward
		function endGame(){

			if (socket.readyState == 1) {
				socket.close();
			}
			//forward to controller
			window.location.href = './leaving.html';
		}



}, false)


//##################################################################################
//helper functions

//delete marker in string
function clearMarker(input){
	return input.replace('$','');
}

//clipping function
Math.clip = function(number, min, max) {
  return Math.max(min, Math.min(number, max));
}


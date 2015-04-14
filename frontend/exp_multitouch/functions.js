
window.addEventListener('load', function(){
 		// Websocket
		var socket = new WebSocket("ws://29.4.93.1:53000");
 
		// Nach dem öffnen des Sockets den Status anzeigen
		socket.onopen 	= function() 	{ console.log('Socket Status: '+socket.readyState+' (open)');	}

 		//button control
	    var button = document.getElementById('button');

	 
	    button.addEventListener('touchstart', function(e){
	        socket.send('A');
	        console.log('pushbutton-start');
	       	e.preventDefault();
	    }, false)
	 
	    button.addEventListener('touchend', function(e){

	        console.log('pushbutton-end');
	       	e.preventDefault();
	    }, false)

	    //steuernippel
 		var canvas = document.getElementById('sadstick');
 		var circle = document.getElementById('circle');


 		var context = canvas.getContext('2d');
 		var centerX = canvas.offsetWidth / 2;
 		var centerY = canvas.offsetHeight / 2;

 		var startx = 0;
	        var starty = 0;
	        var diffx = 0;
	        var diffy = 0;

	        //set circle in center
 		circle.style.top = centerY - circle.offsetHeight/2;
 		circle.style.left = centerX -circle.offsetWidth/2;

 		//start touchinteraction on circle
 	    circle.addEventListener('touchstart', function(e){
	        var touchobj = e.changedTouches[0];
	        startx = parseInt(touchobj.clientX);
	        starty = parseInt(touchobj.clientY);
	       	e.preventDefault();

	       	socket.send(startx+' '+starty);
	       	console.log('start'+startx+'/'+starty);
	    }, false)

 		//move circle through touch
	   	circle.addEventListener('touchmove', function(e){

	        var touchobj = e.changedTouches[0];
	        var currx = parseInt(touchobj.clientX);
	        var curry = parseInt(touchobj.clientY);


	    	context.clearRect(0, 0, canvas.width, canvas.height);

	    	context.lineWidth = 2;
	    	context.beginPath();

 			context.moveTo(centerX, centerY);
	    	context.lineTo(currx, curry);
	    	context.closePath();
	    	context.stroke();


	    	diffx = currx - startx;
	    	diffy = curry - starty;
	        circle.style.top = centerY - circle.offsetHeight/2 + diffy;
 			circle.style.left = centerX - circle.offsetWidth/2 + diffx;
	       	e.preventDefault();


	    	console.log('start: '+centerX+'/'+centerY);

	       	console.log('lineTo: '+currx+'/'+curry);

	        socket.send(currx + ' ' + curry);


	    }, false)

	    circle.addEventListener('touchend', function(e){
	        circle.style.top = centerY - circle.offsetHeight/2;
 			circle.style.left = centerX -circle.offsetWidth/2;
 			context.clearRect(0, 0, canvas.width, canvas.height);
	       	e.preventDefault();
	    }, false)




}, false)


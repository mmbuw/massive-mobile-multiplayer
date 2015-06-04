


window.addEventListener('load', function(){
		//global vars
		var timer = setTimeout(endGame, 30000);
		var colorbase;
		var socket;
		var button_up = document.getElementById('button_up');
		var button_upright = document.getElementById('button_upright');
		var button_right = document.getElementById('button_right');
		var button_rightdown = document.getElementById('button_rightdown');
		var button_down = document.getElementById('button_down');
		var button_leftdown = document.getElementById('button_leftdown');
		var button_left = document.getElementById('button_left');
		var button_leftup = document.getElementById('button_leftup');

		var wrapper = document.getElementById('canvas-wrap');
 		var button_a = document.getElementById('button_a');

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
	}
 
// Websocket-Server
var WebSocketServer = require('ws').Server
var wss = new WebSocketServer({host: '192.168.50.63',port: 8000});
 
wss.on('connection', function(ws) 
{
	console.log('client verbunden...');
 
	ws.on('message', function(message) 
	{
		console.log('von Client empfangen: ' + message);
		ws.send('von Server empfangen: ' + message);
    });
 
});
# Massive Mobile Multiplayer Framework
The Massive Mobile Multiplayer Framework is a sofware platform designed to enable setup-free interaction with shared public displays through personal mobile devices. In this repository, we provide its backend implementation together with the MMMBall application, a real-time soccer game to which client devices can connect. Using a frontend interface which resembles a game controller, users can control their respective soccer players easily.


## Setup and Installation
 - Install necessary dependencies on server: SFML2, python3; if you use a Raspberry Pi, a special fork of SFML2 (e.g https://github.com/mickelson/sfml) needs to be compiled
 - Install a web server and make its root directory point to frontend/exp_multitouch
 - Connect the server to a network from which it is reachable from the client devices, or open a Wifi network using hostapd and dnsmasq.
 - Paste the IP address for accessing the server to the WebSocket creation in frontend/exp_multitouch/js/controller.js (towards the end)
 - Compile the backend module using the provided Makefile in backend/ and MMMBall's configuration file: make CONFIG=configs/mmm-ball
 - Run the compiled backend module in backend/build. The application needs to have read and write access to /dev/uinput in order to create event devices
 - Compile MMMBall using the provided Makefile in game/
 - Run the compiled MMMBall application in game/build. You need to be superuser for that in order to have access to the event devices
 - On the client devices, connect to the web server in a browser, and follow the displayed instructions to join the game.


## Contributors
Tim Weißker (tim.weissker@uni-weimar.de)
Andreas Berst (andreas.berst@uni-weimar.de)
Johannes Hartmann (johannes.hartmann@uni-weimar.de)
Florian Echtler (florian.echtler@uni-weimar.de)
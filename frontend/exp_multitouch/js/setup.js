
function playerSetup() {

	//fetch stuff from dom element
	var playername = document.getElementById('input-name').value;

	playername = checkName(playername);
	//save in session variable
	window.localStorage.setItem("playername", playername);

	//forward to controller
	window.location.href = './controller.html';

}

function checkName(input) {
	//check for input and generate dumb names if no name is set

	return input;
}

function playerSetup() {

	//fetch stuff from dom element
	var playername = document.getElementById('input-name').value;

	playername = checkName(playername);
	//save in session variable
	window.localStorage.setItem("playername", playername);

	//forward to controller
	window.location.href = './controller.html';

}

//check for input and generate dumb names if no name is set
function checkName(input) {1429792994379

	var textArray = [
    'TimDONG',
    'BATMAN',    
    'SuperHARTMANN',
    'BERND',    
    'Gybrush',
    'Donald',    
    'Lobel',
    'SteveHobs',    
    'PeterPan',
    'Elvis',    
    'SiegeRhino',
    'Error',
    'anao',    
    'HAKAN',
    'DeadCat',    
    'BUG',
    'xxx',
    '(x,x)',
    'reallyDEADcat',
    'psphillip'
	];


	//check for input and select a dumb name if no name is set
	if(input == '') {
		var randomNumber = Math.floor(Math.random()*textArray.length);
		input = textArray[randomNumber];
	}

	input = input.replace('\$','');
	return input;
}

function browserCheck() {

	
}
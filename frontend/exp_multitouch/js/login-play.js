
function playerSetup() {
	var playername = document.getElementById('input-name').value;
	playername = checkName(playername);
	window.sessionStorage.setItem("playername", playername);
	window.location.href = './controller.html';
}


//check for input and use preset name if no name is set
function checkName(input) {

	var textArray = [
    'Emrakul',
    'Kozilek',    
    'Ulamog',
    'Liliana',    
    'Gybrush',
    'Donald',    
    'Jace',
    'Gideon',    
    'Elvis',    
    'Siege Rhino',
    'Ballack',    
    'Schneider',
    'Völler',
    'Jeremy',
    'Rahn',
    'Gerrad',
    'Keane'   
	];

	//check for input and select a name if no name is set
	if(input == '') {
		var randomNumber = Math.floor(Math.random()*textArray.length);
		input = textArray[randomNumber];
	}

  //clean name string
	input = input.replace(/Â|À|Å|Ã/g, "A")
                  .replace(/â|à|å|ã/g, "a")
                  .replace(/Ä/g, "AE")
                  .replace(/ä/g, "ae")
                  .replace(/Ç/g, "C")
                  .replace(/ç/g, "c")
                  .replace(/É|Ê|È|Ë/g, "E")
                  .replace(/é|ê|è|ë/g, "e")
                  .replace(/Ó|Ô|Ò|Õ|Ø/g, "O")
                  .replace(/ó|ô|ò|õ/g, "o")
                  .replace(/Ö/g, "OE")
                  .replace(/ö/g, "oe")
                  .replace(/Š/g, "S")
                  .replace(/š/g, "s")
                  .replace(/ß/g, "ss")
                  .replace(/Ú|Û|Ù/g, "U")
                  .replace(/ú|û|ù/g, "u")
                  .replace(/Ü/g, "UE")
                  .replace(/ü/g, "ue")
                  .replace(/Ý|Ÿ/g, "Y")
                  .replace(/ý|ÿ/g, "y")
                  .replace(/Ž/g, "Z")
                  .replace(/ž/, "z")
                  .replace(/ß/, "s")
                  .replace(/\$/g, "z");

    //slice to size
    input = input.slice(0,12);

          
	return input;
}

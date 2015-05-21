
function playerSetup() {

	//fetch stuff from dom element
	var playername = document.getElementById('input-name').value;

	playername = checkName(playername);
	//save in session variable
	window.localStorage.setItem("playername", playername);

	//forward to controller
	window.location.href = './controller.html';

}


function hideAddressBar()
{
  if(!window.location.hash)
  {
      if(document.height < window.outerHeight)
      {
          document.body.style.height = (window.outerHeight + 50) + 'px';
      }
 
      setTimeout( function(){ window.scrollTo(0, 1); }, 50 );
  }
}
 
window.addEventListener("load", function(){ if(!window.pageYOffset){ hideAddressBar(); } } );
window.addEventListener("orientationchange", hideAddressBar );


//check for input and generate dumb names if no name is set
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


	//check for input and select a dumb name if no name is set
	if(input == '') {
		var randomNumber = Math.floor(Math.random()*textArray.length);
		input = textArray[randomNumber];
	}


    //clean umlaute
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
                  .replace(/\$/g, "z");

    //slice to size
    input = input.slice(0,12);

          
	return input;
}

var onLine = false;

window.addEventListener('load', function(){
    document.getElementById('q8').value = navigator.userAgent;
	concheck();
});

window.addEventListener('click', function(){
	concheck();
});


/*
*  Reload image for connection dependent forwarding
*/
function concheck() {
	var img = new Image();
	img.src="http://www.agenda4-online.de/uploads/pics/logo_uni_weimar.jpg?d="+ escape(Date());

	img.onload = doConnectFunction;
	img.onerror = doNotConnectFunction;
}


function doConnectFunction() {
		document.getElementById('form').action = "http://www.basicbits.de/area51/mmm/form.php";	
		console.log("internetz");
}
function doNotConnectFunction() {
		document.getElementById('form').action = "form.php";
		console.log("no internetz");
}
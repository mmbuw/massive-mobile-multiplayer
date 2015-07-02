var onLine = false;

window.addEventListener('load', function(){
    document.getElementById('q8').value = navigator.userAgent;
	document.getElementById("ctimg").src="http://www.studycheck.de/images/institute/normal/uni-weimar.jpg";
	concheck();
	
	setInterval(function() {
		  concheck();
	}, 500);

});

/*
*  Reload image for connection dependent forwarding
*/
function concheck() {
	document.getElementById("ctimg").src="http://www.studycheck.de/images/institute/normal/uni-weimar.jpg?rand=" + Math.random();
	if (onLine) {
		document.getElementById('form').action = "http://www.basicbits.de/area51/mmm/form.php";
	} else {
		document.getElementById('form').action = "form.php";
	}
}



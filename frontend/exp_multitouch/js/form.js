
window.addEventListener('load', function(){

    document.getElementById('q8').value = navigator.userAgent;
    
	if (navigator.onLine) {
		document.getElementById('form').action = "http://www.basicbits.de/area51/mmm/form.php";
		console.log(navigator.onLine);
	}

});
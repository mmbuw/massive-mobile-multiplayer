function doesConnectionExist() {
    var xhr = new XMLHttpRequest();
    var file = "http://localhost/mmm/goodbye.html";

     
    xhr.open('HEAD', file, false);
     
    try {
        xhr.send();
         
        if (xhr.status >= 200 && xhr.status < 304) {
            return true;
        } else {
            return false;
        }
    } catch (e) {
        return false;
    }
}

console.log(doesConnectionExist());

window.addEventListener('load', function(){

	if (!doesConnectionExist()) {
		document.getElementById('form').action = "http://www.basicbits.de/area51/form.php";
	}

});
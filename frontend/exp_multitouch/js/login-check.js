window.addEventListener('load', function(){

	var playbutton  = document.getElementById('playbutton');
	var browser = getBrowser();
	var supported = ["Firefox/31", "Opera/30", "Chrome/42", "Safari/8", "OPR/30"];
	var issupported  = false;

	for (var i in supported) {

		var b = browser.split("/");
		var s = supported[i].split("/");
		if (b[0] == s[0]) {
			if(b[1].replace(".", "") >= s[1]) {
				issupported = true;
			}
		} 
	}

	console.log(navigator.userAgent);

	if (issupported) {
		playbutton.style.backgroundColor ="red";
	} else {
		playbutton.onclick=null;
		playbutton.innerHTML = "Your browser is not supported.";
	}

}, false)


function getBrowser() {

	var ua = navigator.userAgent;
	var indent = ["Chrome", "Firefox", "OPR", "OPiOS", "CriOS", "Version"];
	var os = ["Android", "iPhone", "iPad"];
	var start;
	var end;
	var output;

	for (var i in indent) {

		if (ua.search(indent[i]) > -1) {
			start = ua.search(indent[i]);
			end = ua.search(indent[i])+indent[i].length+3;
			output = ua.substring(start,end);

			// Safari
			if (ua.search("Version") > -1 && ua.search(os[2]) > -1 || ua.search(os[1]) > -1 ) {
				output = "Safari"+output.substring(output.length-3,output.length);
			}

			//dafault Android Browser
			if (ua.search("Version") > -1 && ua.search(os[0]) > -1 ) {
				output = "default"+output.substring(output.length-3,output.length);
			}
		}

	}

	return output;
	console.log(output);
}
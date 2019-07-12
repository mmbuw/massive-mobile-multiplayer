window.addEventListener('load', function(){

	var playbutton  = document.getElementById('playbutton');
	var browser = getBrowser();
	var supported = ["Firefox/31", "Opera/30", "Chrome/42", "Safari/7", "OPR/30", "CriOS/43", "OPiOS/10"];
	var issupported  = false;

	for (var i in supported) {

		var b = browser.split("/");
		var s = supported[i].split("/");
		
		if (b[0] == s[0]) {
			if(+b[1].replace(".", "") >= +s[1]) {
				issupported = true;
			}
		} 
	}

	if (issupported) {
		playbutton.style.backgroundColor ="red";
	} else {
		playbutton.onclick=null;
		playbutton.innerHTML = "Your browser is not supported.";
	}

}, false)


function getBrowser() {

	var ua = navigator.userAgent;
	var indent = ["Chrome", "Firefox", "Safari", "OPR", "OPiOS", "CriOS", "Version"];
	var os = ["Android", "iPhone", "iPad"];
	var start;
	var end;
	var output;

	for (var i in indent) {
		if (ua.search(indent[i]) > -1) {
			start = ua.search(indent[i]);
			//end = ua.search(indent[i])+indent[i].length+3;
			next_space = ua.indexOf(" ", start);
			next_dot = ua.indexOf(".", start);
			if (next_space < 0 && next_dot < 0) {
				end = ua.length;
			} else if (next_space < 0 && next_dot > 0) {
				end = next_dot;
			} else if (next_space > 0 && next_dot < 0) {
				end = next_space;
			} else {
				end = Math.min(next_space, next_dot);
			}
			
			output = ua.substring(start,end);

			// Safari
			// Should now appear in ua
			//if (ua.search("Version") > -1 && ua.search(os[2]) > -1 || ua.search(os[1]) > -1 ) {
			//	output = "Safari"+output.substring(output.length-3,output.length);
			//}

			//dafault Android Browser
			if (ua.search("Version") > -1 && ua.search(os[0]) > -1 ) {
				output = "default"+output.substring(output.length-3,output.length);
			}
			break;
		}

	}

	return output;
}
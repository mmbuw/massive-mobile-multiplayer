window.addEventListener('load', function(){

	var playbutton  = document.getElementById('playbutton');

	navigator.sayswho= (function(){
	    var ua= navigator.userAgent, tem, 
	    M= ua.match(/(opera|chrome|safari|firefox|msie|trident(?=\/))\/?\s*(\d+)/i) || [];
	    if(/trident/i.test(M[1])){
	        tem=  /\brv[ :]+(\d+)/g.exec(ua) || [];
	        return 'IE '+(tem[1] || '');
	    }
	    if(M[1]=== 'Chrome'){
	        tem= ua.match(/\bOPR\/(\d+)/);
	        if(tem!= null) return 'Opera '+tem[1];
	    }
	    M= M[2]? [M[1], M[2]]: [navigator.appName, navigator.appVersion, '-?'];
	    if((tem= ua.match(/version\/(\d+)/i))!= null) M.splice(1, 1, tem[1]);
	    return M.join('');
	})();

	var isChrome = (navigator.sayswho.indexOf("Chrome4") > -1);
	var isFirefox = (navigator.sayswho.indexOf("Firefox3") > -1);
	var isSafari = (navigator.sayswho.indexOf("Safari8") > -1);
	var isOpera = (navigator.sayswho.indexOf("Opera 3") > -1);

	//window.alert(navigator.sayswho);

	if (isChrome || isFirefox || isSafari || isOpera) {
		playbutton.style.backgroundColor ="red";
	} else {
		playbutton.onclick=null;
		playbutton.innerHTML = "Your browser is not supported.";
	}

}, false)
'use strict';

document.addEventListener("DOMContentLoaded", function(event) {

    var timer = null;
    const sendRequest = function(command, code) {
        return fetch(
          `http://${location.host}/api`,
          {
            method: 'POST',
            body: JSON.stringify({
              command: command,
              code: code
            }),
            headers: {
              'Content-Type': 'application/json'
            }
          }
        );
    }
    
    function run(e){
      const d = e.target.getAttribute('id').split(' ');
          const command = d[0];
          const code = d[1];
          promise = sendRequest(command, code);
    }
    let promise;
	let delayTime = 150;
    let targets = document.getElementsByClassName('icons');
    for (let i = 0; i < targets.length; i++) {
      targets[i].addEventListener('contextmenu', function(e) {
        e.preventDefault();
      });
      targets[i].addEventListener("touchstart", function(e) {
        timer = setInterval(function() {
          run(e);
        }, delayTime);
      });
      targets[i].addEventListener("touchend", function(e) {
        clearInterval(timer);
		return;
      });
      targets[i].addEventListener("mousedown", function(e) {
        timer = setInterval(function() {
          run(e);
        }, delayTime);
      });
      targets[i].addEventListener("mouseup", function(e) {
        clearInterval(timer);
		return;
		});
    }

});

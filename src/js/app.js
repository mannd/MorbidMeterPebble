var Clay = require('pebble-clay');
var clayConfig = require('./config');
var clay = new Clay(clayConfig, null, {autoHandleEvents: false});
var messageKeys = require('message_keys');

Pebble.addEventListener('showConfiguration', function(e) {
    Pebble.openURL(clay.generateUrl());
});

Pebble.addEventListener('webviewclosed', function(e) {
    if (e && !e.response) {
	return;
    }

    var dict = clay.getSettings(e.response);
    var startDate = dict[messageKeys.START_DATE];
    var startTime = dict[messageKeys.START_TIME];

    console.log(startDate);
    console.log(startTime);
  

    // add to dict here

    Pebble.sendAppMessage(dict, function(e) {
	console.log('Sent my config data to Pebble');
	console.log(startDate);
	console.log(startTime);
    }, function(e) {
	console.log('Failed to send config data!');
	console.log(JSON.stringify(e));
  });
});





function getTimeFromDateAndTime(date, time) {
  var dateParts = date.split('-');
  var timeParts = time.split(':');
  var today = new Date();
  var timezoneOffset = today.getTimezoneOffset();
  var d =  new Date(dateParts[0], dateParts[1] - 1, dateParts[2], timeParts[0],
                    timeParts[1] - timezoneOffset, 0);
  return d.getTime() / 1000;   // Pebble uses secs not msec for time
};


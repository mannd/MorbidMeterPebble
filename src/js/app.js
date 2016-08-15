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

    var settings = clay.getSettings(e.response);
    var startDate = settings[messageKeys.START_DATE];
    var startTime = settings[messageKeys.START_TIME];
    var endDate = settings[messageKeys.END_DATE];
    var endTime = settings[messageKeys.END_TIME];

    var startSecs = getTimeFromDateAndTime(startDate, startTime);
    var endSecs = getTimeFromDateAndTime(endDate, endTime);

    // make start and end dates into secs
    dict = settings;
    dict[messageKeys.START_DATE] = startSecs.toString();
    dict[messageKeys.END_DATE] = endSecs.toString();
  
    Pebble.sendAppMessage(dict, function(e) {
	console.log('Sent my config data to Pebble');
    }, function(e) {
	console.log('Failed to send config data!');
	console.log(JSON.stringify(e));
  });
});

function getTimeFromDateAndTime(date, time) {
  var dateParts = date.split('-');
  var timeParts = time.split(':');
  // apparently date returns local date, including msec, so
  // no need to adjust for time zone.
  // This was a bug in earlier versions of MMP
  //var timezoneOffset = new Date().getTimezoneOffset();

  // Config doesn't allow adjusting seconds, so they are zeroed out
  var d =  new Date(dateParts[0], dateParts[1] - 1, dateParts[2], timeParts[0],
                    timeParts[1]);
  return d.getTime() / 1000;   // Pebble uses secs not msec for time
}
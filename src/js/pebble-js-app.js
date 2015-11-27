Pebble.addEventListener('ready', function() {
  console.log('PebbleKit JS ready!');

});

Pebble.addEventListener('showConfiguration', function() {
//  var url = 'http://127.0.0.1:8080';
//  var url = 'http://6adb98e7.ngrok.io';
//  var url = 'http://epstudiossoftware.com/pebble/config/';
  var url = 'http://mannd.github.io/MorbidMeterPebble/config/';
  console.log('Showing configuration page: ' + url);
  Pebble.openURL(url);
});

Pebble.addEventListener('webviewclosed', function(e) {
  var configData = JSON.parse(decodeURIComponent(e.response));

  console.log('Configuration page returned: ' + JSON.stringify(configData));

  if (configData.backgroundColor) {
      Pebble.sendAppMessage({
      backgroundColor: parseInt(configData.backgroundColor, 16),
      twentyFourHourFormat: configData.twentyFourHourFormat ? 1 : 0,
      timescaleSelector: configData.timescaleSelector,
      localTimeShowSecondsCheckBox: configData.localTimeShowSecondsCheckBox ? 1 : 0,
      shakeWristTogglesTimeCheckBox: configData.shakeWristTogglesTimeCheckBox ? 1 : 0,
      reverseTimeCheckBox: configData.reverseTimeCheckBox ? 1 : 0,
      startDateTimeInSecs: configData.startDateTimeInSecs,
      endDateTimeInSecs: configData.endDateTimeInSecs
    }, function() {
      console.log('Send successful!');
    }, function() {
      console.log('Send failed!');
    });
  }
});

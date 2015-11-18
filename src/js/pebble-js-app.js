Pebble.addEventListener('ready', function() {
  console.log('PebbleKit JS ready!');
});

Pebble.addEventListener('showConfiguration', function() {
//  var url = 'http://127.0.0.1:8080';
  var url = 'http://6adb98e7.ngrok.io';
  console.log('Showing configuration page: ' + url);
  Pebble.openURL(url);
});

Pebble.addEventListener('webviewclosed', function(e) {
  var configData = JSON.parse(decodeURIComponent(e.response));

  console.log('Configuration page returned: ' + JSON.stringify(configData));

  if (configData.backgroundColor) {
    Pebble.sendAppMessage({
      backgroundColor: parseInt(configData.backgroundColor, 16),
      twentyFourHourFormat: configData.twentyFourHourFormat,
      timescaleSelector: configData.timescaleSelector,
      localTimeShowSecondsCheckBox: configData.localTimeShowSecondsCheckBox,
      shakeWristTogglesTimeCheckBox: configData.shakeWristTogglesTimeCheckBox,
      reverseTimeCheckBox: configData.reverseTimeCheckBox,
      startDate: configData.startDate,
      startTime: configData.startTime,
      endDate: configData.endDate,
      endTime: configData.endTime
// add rest here
    }, function() {
      console.log('Send successful!');
    }, function() {
      console.log('Send failed!');
    });
  }
});

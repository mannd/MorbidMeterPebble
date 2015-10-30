Pebble.addEventListener('ready', function() {
  console.log('PebbleKit JS ready!');
});

Pebble.addEventListener('showConfiguration', function() {
//  var url = 'http://238.0.0.2:8080'p
  var url = 'http://127.0.0.1:8080';
//  var url = 'http://d0478d6d.ngrok.io';
  console.log('Showing configuration page: ' + url);

  Pebble.openURL(url);
});

Pebble.addEventListener('webviewclosed', function(e) {
  var configData = JSON.parse(decodeURIComponent(e.response));

  console.log('Configuration page returned: ' + JSON.stringify(configData));

  if (configData.backgroundColor) {
    Pebble.sendAppMessage({
      backgroundColor: parseInt(configData.backgroundColor, 16),
      twentyFourHourFormat: configData.twentyFourHourFormat
// add rest here
    }, function() {
      console.log('Send successful!');
    }, function() {
      console.log('Send failed!');
    });
  }
});
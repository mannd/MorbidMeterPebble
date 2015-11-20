(function() {
  loadOptions();
  submitHandler();
})();

function submitHandler() {
  var $submitButton = $('#submitButton');

  $submitButton.on('click', function() {
    console.log('Submit');

    var return_to = getQueryParam('return_to', 'pebblejs://close#');
    document.location = return_to
                      + encodeURIComponent(
                        JSON.stringify(getAndStoreConfigData()));
  });
}

function loadOptions() {
  var $backgroundColorPicker = $('#backgroundColorPicker');
  var $timeFormatCheckBox = $('#timeFormatCheckBox');
  var $timescaleSelector = $('#timescaleSelector');
  var $localTimeShowSecondsCheckBox = $('#localTimeShowSecondsCheckBox');
  var $shakeWristTogglesTimeCheckBox = $('#shakeWristTogglesTimeCheckBox');
  var $reverseTimeCheckBox = $('#reverseTimeCheckBox');
  var $startDate = $('#startDate');
  var $startTime = $('#startTime');
  var $endDate = $('#endDate');
  var $endTime = $('#endTime');

  // for development only!
  //localStorage.clear();

  if (localStorage.backgroundColor) {
    $backgroundColorPicker[0].value = localStorage.backgroundColor;
    $timeFormatCheckBox[0].checked =
      (localStorage.twentyFourHourFormat === 'true');
    $timescaleSelector[0].value = localStorage.timescaleSelector;
    $localTimeShowSecondsCheckBox[0].checked =
      (localStorage.localTimeShowSecondsCheckBox === 'true');
    $shakeWristTogglesTimeCheckBox[0].checked =
      (localStorage.shakeWristTogglesTimeCheckBox === 'true');
    $reverseTimeCheckBox[0].checked =
      (localStorage.reverseTimeCheckBox === 'true');
    $startDate[0].value = localStorage.startDate;
    $startTime[0].value = localStorage.startTime;
    $endDate[0].value = localStorage.endDate;
    $endTime[0].value = localStorage.endTime;
  }
}

function getTimeFromDateAndTime(date, time) {
  var dateParts = date.split('-');
  var timeParts = time.split(':');
  var d =  new Date(dateParts[0], dateParts[1] - 1, dateParts[2], timeParts[0], timeParts[1], 0);
  return d.getTime() / 1000;   // Pebble uses secs not msec for time
}


function getAndStoreConfigData() {
  var $backgroundColorPicker = $('#backgroundColorPicker');
  var $timeFormatCheckBox = $('#timeFormatCheckBox');
  var $timescaleSelector =$('#timescaleSelector');
  var $localTimeShowSecondsCheckBox = $('#localTimeShowSecondsCheckBox');
  var $shakeWristTogglesTimeCheckBox = $('#shakeWristTogglesTimeCheckBox');
  var $reverseTimeCheckBox = $('#reverseTimeCheckBox');
  var $startDate = $('#startDate');
  var $startTime = $('#startTime');
  var $endDate = $('#endDate');
  var $endTime = $('#endTime');

  var options = {
    backgroundColor: $backgroundColorPicker.val(),
    twentyFourHourFormat: $timeFormatCheckBox[0].checked,
    timescaleSelector: $timescaleSelector.val(),
    localTimeShowSecondsCheckBox: $localTimeShowSecondsCheckBox[0].checked,
    shakeWristTogglesTimeCheckBox: $shakeWristTogglesTimeCheckBox[0].checked,
    reverseTimeCheckBox: $reverseTimeCheckBox[0].checked,
    startDateTimeInSecs: getTimeFromDateAndTime($startDate.val(),
                                             $startTime.val()),
    endDateTimeInSecs: getTimeFromDateAndTime($endDate.val(),
                                             $endTime.val())
  };

  localStorage.backgroundColor = options.backgroundColor;
  localStorage.twentyFourHourFormat = options.twentyFourHourFormat;
  localStorage.timescaleSelector = options.timescaleSelector;
  localStorage.localTimeShowSecondsCheckBox =
    options.localTimeShowSecondsCheckBox;
  localStorage.shakeWristTogglesTimeCheckBox =
    options.shakeWristTogglesTimeCheckBox;
  localStorage.reverseTimeCheckBox =
    options.reverseTimeCheckBox;
  localStorage.startDate = $startDate.val();
  localStorage.startTime = $startTime.val();
  localStorage.endDate = $endDate.val();
  localStorage.endTime = $endTime.val();

  console.log('Got options: ' + JSON.stringify(options));

  return options;
}

function getQueryParam(variable, defaultValue) {
  var query = location.search.substring(1);
  var vars = query.split('&');
  for (var i = 0; i < vars.length; i++) {
    var pair = vars[i].split('=');
    if (pair[0] === variable) {
      return decodeURIComponent(pair[1]);
    }
  }
  return defaultValue || false;
}
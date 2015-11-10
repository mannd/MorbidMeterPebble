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
  var $timeFormatCheckbox = $('#timeFormatCheckbox');
  var $timescaleSelector = $('#timescaleSelector');
  var $localTimeShowSecondsCheckBox = $('#localTimeShowSecondsCheckBox');
  var $morbidMeterTimeUpdateIntervalSelector =
    $('#morbidMeterTimeUpdateIntervalSelector');

  if (localStorage.backgroundColor) {
    $backgroundColorPicker[0].value = localStorage.backgroundColor;
    $timeFormatCheckbox[0].checked =
      (localStorage.twentyFourHourFormat === 'true');
    $timescaleSelector[0].value = localStorage.timescaleSelector;
    $localTimeShowSecondsCheckBox[0].checked =
      (localStorage.localTimeShowSecondsCheckBox === 'true');
    $morbidMeterTimeUpdateIntervalSelector[0].value =
      localStorage.morbidMeterTimeUpdateIntervalSelector;
  }
}

function getAndStoreConfigData() {
  var $backgroundColorPicker = $('#backgroundColorPicker');
  var $timeFormatCheckbox = $('#timeFormatCheckbox');
  var $timescaleSelector =$('#timescaleSelector');
  var $localTimeShowSecondsCheckBox = $('#localTimeShowSecondsCheckBox');
  var $morbidMeterTimeUpdateIntervalSelector =
    $('#morbidMeterTimeUpdateIntervalSelector');

  var options = {
    backgroundColor: $backgroundColorPicker.val(),
    twentyFourHourFormat: $timeFormatCheckbox[0].checked,
    timescaleSelector: $timescaleSelector.val(),
    localTimeShowSecondsCheckBox: $localTimeShowSecondsCheckBox[0].checked,
    morbidMeterTimeUpdateIntervalSelector: $morbidMeterTimeUpdateIntervalSelector.val()
  };

  localStorage.backgroundColor = options.backgroundColor;
  localStorage.twentyFourHourFormat = options.twentyFourHourFormat;
  localStorage.timescaleSelector = options.timescaleSelector;
  localStorage.localTimeUpdateIntervalSelector =
    options.localTimeUpdateIntervalSelector;
  localStorage.morbidMeterTimeUpdateIntervalSelector =
    options.morbidMeterTimeUpdateIntervalSelector;

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
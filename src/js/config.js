module.exports = [
    { 
    	"type": "heading", 
    	"defaultValue": "MorbidMeter Configuration",
	"size": 1,
    },
    {
	"type": "section",
	"items": [
	    {
		"type": "heading",
		"defaultValue": "Appearance and Behavior"
	    },
	    {
		"type": "color",
		"messageKey": "KEY_BACKGROUND_COLOR",
		"label": "Background Color",
		"defaultValue": "000000",
		"sunlight": true
	    },
	    {
		"type": "toggle",
		"messageKey": "KEY_TWENTY_FOUR_HOUR_FORMAT",
		"label": "Local Time Use 24-Hour Format",
		"defaultValue": true
	    },
	    {
		"type": "toggle",
		"messageKey": "KEY_LOCAL_TIME_SHOW_SECONDS",
		"label": "Local Time Show Seconds",
		"defaultValue": true
	    },
	    {
		"type": "toggle",
		"messageKey": "KEY_SHAKE_WRIST_TOGGLES_TIME",
		"label": "Shake Wrist Toggles Time",
		"defaultValue": true
	    }
	]
    },
    {
	"type": "section",
	"items": [
	    {
		"type": "heading",
		"defaultValue": "Timer Configuration"
	    },
	    {
		"type": "select",
		"messageKey": "KEY_TIMESCALE",
		"defaultValue": "localtime",
		"label": "Timescale",
		"options": [
		    {
			"label": "Local Time",
			"value": "localtime"
		    },
		    { 
			"label": "Seconds",
			"value": "seconds" 
		    },
		    { 
			"label": "Minutes",
			"value": "minutes" 
		    },
		    { 
			"label": "Hours",
			"value": "hours" 
		    },
		    { 
			"label": "Days",
			"value": "days"
		    },
		    { 
			"label": "D H M S",
			"value": "dhms" 
		    },
		    { 
			"label": "Years",
			"value": "years" 
		    },
		    { 
			"label": "One Hour",
			"value": "onehour" 
		    },
		    { 
			"label": "One Day",
			"value": "oneday"
		    },
		    { 
			"label": "One Month",
			"value": "onemonth" 
		    },
		    { 
			"label": "One Year",
			"value": "oneyear" 
		    },
		    { 
			"label": "Universe",
			"value": "universe" 
		    },
		    { 
			"label": "X Universe",
			"value": "xuniverse" 
		    },
		    { 
			"label": "X Universe 2",
			"value": "xuniverse2" 
		    },
		    { 
			"label": "Percent",
			"value": "percent" 
		    }
		]
	    },
	    {
		"type": "toggle",
		"messageKey": "KEY_REVERSE_TIME",
		"label": "Countdown Time",
		"defaultValue": false
	    },
	    {
		"type": "input",
		"messageKey": "KEY_START_DATE",
		"label": "Start Date",
		"attributes":
		{
		    "type": "date"
		}
	    },
	    {
		"type": "input",
		"messageKey": "KEY_START_TIME",
		"label": "Start Date",
		"attributes":
		{
		    "type": "time"
		}
	    },
	    {
		"type": "input",
		"messageKey": "KEY_END_DATE",
		"label": "Start Date",
		"attributes":
		{
		    "type": "date"
		}
	    },
	    {
		"type": "input",
		"messageKey": "KEY_END_TIME",
		"label": "Start Date",
		"attributes":
		{
		    "type": "time"
		}
	    },
	    // add fracture time/ fracture time interval
	]
    },
    {
	"type": "submit",
	"defaultValue": "Save"
    }
];


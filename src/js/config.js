module.exports = [
    { 
    	"type": "heading", 
    	"defaultValue": "MorbidMeter",
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
		"messageKey": "BACKGROUND_COLOR",
		"label": "Background Color",
		"defaultValue": "FFFFFF",
		"sunlight": true
	    },
	    {
		"type": "toggle",
		"messageKey": "TWENTY_FOUR_HOUR_FORMAT",
		"label": "Local Time Use 24-Hour Format",
		"defaultValue": true
	    },
	    {
		"type": "toggle",
		"messageKey": "LOCAL_TIME_SHOW_SECONDS",
		"label": "Local Time Show Seconds",
		"defaultValue": true
	    },
	    {
		"type": "toggle",
		"messageKey": "SHAKE_WRIST_TOGGLES_TIME",
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
		"messageKey": "TIMESCALE",
		"defaultValue": "Local Time",
		"label": "Timescale",
		"options": [
		    {
			"label": "Local Time",
			"value": "Local Time"
		    },
		    { 
			"label": "Seconds",
			"value": "Seconds"
		    },
		    { 
			"label": "Minutes",
			"value": "Minutes"
		    },
		    { 
			"label": "Hours",
			"value": "Hours"
		    },
		    { 
			"label": "Days",
			"value": "Days"
		    },
		    { 
			"label": "D H M S",
			"value": "D H M S"
		    },
		    { 
			"label": "Years",
			"value": "Years"
		    },
		    { 
			"label": "One Day",
			"value": "One Day"
		    },
		    { 
			"label": "One Hour",
			"value": "One Hour"
		    },
		    { 
			"label": "One Month",
			"value": "One Month"
		    },
		    { 
			"label": "One Year",
			"value": "One Year"
		    },
		    { 
			"label": "Universe",
			"value": "Universe"
		    },
		    { 
			"label": "X Universe",
			"value": "X Universe"
		    },
		    { 
			"label": "X Universe 2",
			"value": "X Universe 2"
		    },
		    { 
			"label": "Percent",
			"value": "Percent"
		    }
		]
	    },
	    {
		"type": "toggle",
		"messageKey": "REVERSE_TIME",
		"label": "Countdown Time",
		"defaultValue": false
	    },
	    {
		"type": "input",
		"messageKey": "START_DATE",
		"label": "Start Date",
		"attributes":
		{
		    "type": "date"
		}
	    },
	    {
		"type": "input",
		"messageKey": "START_TIME",
		"label": "Start Time",
		"attributes":
		{
		    "type": "time"
		}
	    },
	    {
		"type": "input",
		"messageKey": "END_DATE",
		"label": "End Date",
		"attributes":
		{
		    "type": "date"
		}
	    },
	    {
		"type": "input",
		"messageKey": "END_TIME",
		"label": "End Time",
		"attributes":
		{
		    "type": "time"
		}
	    },
	    {
		"type": "toggle",
		"messageKey": "FRACTURE_TIME",
		"label": "Fracture Time",
		"defaultValue": false
	    },
	    {
		"type": "select",
		"messageKey": "FRACTURE_TIME_INTERVAL",
		"label": "Fracture Time Interval",
		"defaultValue": "By Sec",
		"options": [
		    {
			"label": "By Sec",
			"value": "By Sec"
		    },
		    {
			"label": "By Min",
			"value": "By Min"
		    },
		    { 
			"label": "By Hour",
			"value": "By Hour"
		    },
		    {
			"label": "By Day",
			"value": "By Day"
		    }
		]
	    },
	]
    },
    {
	"type": "submit",
	"defaultValue": "Save"
    }
];

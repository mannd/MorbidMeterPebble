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
	    // 			{
	    //  				"type": "select",
	    //  				"messageKey": "SELECT_BATTERY_COLOUR",
	    //  				"defaultValue": "white",
	    //  				"label": "Low battery colour:",
	    //  				"options": [
	    //  					{ 
	    //  						"label": "white",
	    //  						"value": "white" 
	    // 					},
	    // 					{ 
	    // 						"label": "black",
	    // 						"value": "black" 
	    // 					},
	    // 				]
	    // 			},
	]
    },
    {
 	"type": "section",
 	"items": [
	    // 			{
	    // 				"type": "select",
	    // 				"messageKey": "SELECT_DATE_BOTTOM",
	    // 				"defaultValue": "0",
	    // 				"label": "Date layout",
	    // 				"options": [
	    // 					{ 
	    // 						"label": "Day Month",
	    // 						"value": "0" 
	    // 					},
	    // 					{ 
	    // 						"label": "Month Day",
	    // 						"value": "1" 
	    // 					},
	    // 				]
	    // 			},
	    {
		"type": "toggle",
		"messageKey": "TOGGLE_SUFFIX",
		"label": "Toggle suffix for date",
		//				"description": "You may need to wait a minute for the change to occur",
		"defaultValue": false,
	    },
	]
    },
    // 	{
    // 		"type": "section",
    // 		"items": [		
    // 			{
    // 				"type": "checkboxgroup",
    // 				"messageKey": "CHECK_DATE",
    // 				"label": "Dates to display",
    // 				"defaultValue": [false, false, false],
    // 				"options": ["New Years Day", "Christmas", "Halloween"]
    // 			}
    // 		]
    // 	},			
    {
	"type": "submit",
	"defaultValue": "Save"
    }
];


#include <pebble.h>
#include "timescales.h"
#include "myatof.h"

#define UINT_MAX  4294967295U
#define MM_TITLE "MorbidMeter"
#define DATE "%n%b %e %Y"
#define COMPLETE "Done"
#define LEFT "Left"
#define TOO_SOON_MESSAGE "\nToo Soon!"
#define TOO_LATE_MESSAGE "\nTimer Complete!"
#define NEGATIVE_TIME_DURATION_MESSAGE "\nEnd Sooner Than Start!"
#define OUT_OF_RANGE_SECS_MSG "Too Many Secs To Count"

#define SECS_IN_HOUR (60 * 60)
#define SECS_IN_DAY (24 * SECS_IN_HOUR)
#define SECS_IN_YEAR (365.25 * SECS_IN_DAY)
// must be a large int, bigger than largest counter value
#define MAX_COUNTER 999999    
static Window *s_main_window;
static GBitmap *s_bitmap;
static BitmapLayer *s_bitmap_layer;
static TextLayer *s_time_layer;
static TextLayer *s_timescale_layer;
static bool twenty_four_hour_format = false;
static bool shake_wrist_toggles_time = true;
static bool reverse_time = false;
static bool timer_expired = false;
static bool local_time_show_seconds = true;
static int64_t start_date_time_in_secs = 0;
static int64_t end_date_time_in_secs = 0;
static bool fracture_time = false;
static fracturetimeinterval fracture_time_interval;
static int fracturetime_counter = MAX_COUNTER;


static char time_buffer[] = MM_TITLE "\nMMM 00 0000\n00:00:00 pm Left";
static char timescale_buffer[] = "   " LOCAL_TIME "   ";
static char fracture_time_buffer[] = " By Hour ";

static timescale selected_timescale = TS_LOCAL_TIME;
static timescale displayed_timescale = TS_LOCAL_TIME;
static timescale alternative_timescale = TS_LOCAL_TIME;

static void set_background_and_text_color(int color) {
#ifdef PBL_SDK_3
  GColor background_color = GColorFromHEX(color);
  window_set_background_color(s_main_window, background_color);
  text_layer_set_text_color(s_time_layer, gcolor_legible_over(background_color));
  text_layer_set_text_color(s_timescale_layer, gcolor_legible_over(background_color));
#endif
}

static void vertically_center_time_layer() {
  // center layer
  Layer *window_layer = window_get_root_layer(s_main_window);
  GRect bounds = layer_get_bounds(window_layer);
  GSize size = text_layer_get_content_size(s_time_layer);
  layer_set_frame((Layer *)s_time_layer, GRect(0, (bounds.size.h - size.h) / 2, bounds.size.w,
					       100));
}


static int get_decimal_portion_of_double(double d) {
  return (int)((d < 0 ? -d : d) * 1000) % 1000;
}

static double random_double() {
  // returns random double between 0 and 1, not including 1
  return (double)rand() / (double)((unsigned)RAND_MAX + 1);
  // use below if we need to include 1.0 in range
  // return (double)rand() / (double)RAND_MAX;
}

static void update_time() {
  time_t real_time = time(NULL);
  struct tm *tick_time = localtime(&real_time);
  int64_t diff = (int64_t)real_time - start_date_time_in_secs;
  int64_t reverse_diff = end_date_time_in_secs - (int64_t)real_time;
  int64_t total_time = end_date_time_in_secs - start_date_time_in_secs;
  // handle bad or time-out situations here
  if (displayed_timescale != TS_LOCAL_TIME) {
    if (total_time <= 0) {
      APP_LOG(APP_LOG_LEVEL_DEBUG, "end time = %d", (int)end_date_time_in_secs);
      APP_LOG(APP_LOG_LEVEL_DEBUG, "start time = %d", (int)start_date_time_in_secs);
      snprintf(time_buffer, sizeof(time_buffer),
    	       MM_TITLE NEGATIVE_TIME_DURATION_MESSAGE);
      text_layer_set_text(s_time_layer, time_buffer);
      vertically_center_time_layer();
      return;
    }
    if (diff < 0) {
      snprintf(time_buffer, sizeof(time_buffer),
  	       MM_TITLE TOO_SOON_MESSAGE);
      text_layer_set_text(s_time_layer, time_buffer);
      vertically_center_time_layer();
      return;
    }
    else if (reverse_diff < 0) {
      snprintf(time_buffer, sizeof(time_buffer),
  	       MM_TITLE TOO_LATE_MESSAGE);
      if (!timer_expired) {
	vibes_short_pulse();
	timer_expired = true;
      }
      text_layer_set_text(s_time_layer, time_buffer);
      vertically_center_time_layer();
      return;
    }
  }
  if (displayed_timescale != TS_LOCAL_TIME && fracture_time) {
    // don't bother with counters if updating fracture time every second
    if (fracture_time_interval != FT_BY_SEC) {
      int reset_interval = get_number_of_secs_from_timeinterval(fracture_time_interval);
      if (fracturetime_counter <= reset_interval) {
	fracturetime_counter++;
	return;
      }
      else {
	fracturetime_counter = 0;
      }
    }
    diff *= random_double();
    reverse_diff *= random_double();
  }
  
  char format_str[40];
  // Title alwasy on top line
  // rest of message formatted by Pebble
  strcpy(format_str, MM_TITLE "\n");
  char suffix[9]; 
  int64_t time_duration;
  if (!reverse_time) {
    time_duration = diff;
    strcpy(suffix, COMPLETE);
  }
  else {
    time_duration = reverse_diff;
    strcpy(suffix, LEFT);
  }

  if (displayed_timescale == TS_LOCAL_TIME) {
    // Write the current hours and minutes into the buffer
    if (twenty_four_hour_format) {
      if (local_time_show_seconds) {
	strftime(time_buffer, sizeof(time_buffer),
		 MM_TITLE DATE "\n%H:%M:%S", tick_time);
      } else {
	strftime(time_buffer, sizeof(time_buffer),
		 MM_TITLE DATE "\n%H:%M", tick_time);
      }
    } else {
      if (local_time_show_seconds) {
	strftime(time_buffer, sizeof(time_buffer),
		 MM_TITLE DATE "\n%l:%M:%S %p", tick_time);
      } else {
	strftime(time_buffer, sizeof(time_buffer),
		 MM_TITLE DATE "\n%l:%M %p", tick_time);
      }
    }
  }
  else if (displayed_timescale == TS_PERCENT) {
      double percent_time = 100.0 * (double)time_duration / total_time;
      strcat(format_str, "%d.%03d%% ");
      strcat(format_str, suffix);
      snprintf(time_buffer, sizeof(time_buffer), format_str,
	       (int)percent_time, get_decimal_portion_of_double(percent_time));
  }
  else if (displayed_timescale == TS_SECONDS) {
    // secs can overflow if time_duration > ~136 years, so don't allow this.
    // Note that other timescales may overflow without warning for "ridiculous"
    // numbers of years
    if (time_duration >= UINT_MAX) {
      strcat(format_str, OUT_OF_RANGE_SECS_MSG);
    }
    else {
      strcat(format_str, "%lu Secs ");
      strcat(format_str, suffix);
    }
    snprintf(time_buffer, sizeof(time_buffer), format_str, (unsigned long)time_duration);
  }
  else if (displayed_timescale == TS_MINUTES) {
    strcat(format_str, "%lu Mins ");
    strcat(format_str, suffix);
    snprintf(time_buffer, sizeof(time_buffer), format_str,
	     (unsigned long)time_duration / 60);
  }
  else if (displayed_timescale == TS_HOURS) {
    double hours = (double)time_duration / SECS_IN_HOUR;
    strcat(format_str, "%d.%03d Hours ");
    strcat(format_str, suffix);
    snprintf(time_buffer, sizeof(time_buffer), format_str, (int)hours,
	       get_decimal_portion_of_double(hours));
  }
  else if (displayed_timescale == TS_DAYS) {
    double days = (double)time_duration / SECS_IN_DAY;
    strcat(format_str, "%d.%03d \nDays ");
    strcat(format_str, suffix);
    snprintf(time_buffer, sizeof(time_buffer), format_str, (int)days,
	       get_decimal_portion_of_double(days));
  }
  else if (displayed_timescale == TS_YEARS) {
    double years = (double)time_duration / SECS_IN_YEAR;
    strcat(format_str, "%d.%0d \nYears ");
    strcat(format_str, suffix);
    snprintf(time_buffer, sizeof(time_buffer), format_str, (int)years,
	       get_decimal_portion_of_double(years));
  }
  else if (displayed_timescale == TS_DAYS_HOURS_MINS_SECS) {
    if (time_duration >= UINT_MAX) {
      strcat(format_str, OUT_OF_RANGE_SECS_MSG);
      snprintf(time_buffer, sizeof(time_buffer), format_str);
    }
    else {
      unsigned long secs = (unsigned long) time_duration;
      int mins = secs / 60;
      int hours = mins /60;
      int days = hours / 24;
      strcat(format_str, "%dd %dh \n%dm %ds ");
      strcat(format_str, suffix);
      snprintf(time_buffer, sizeof(time_buffer), format_str,
	       days, hours % 24, mins % 60, secs % 60);
    }
  }
  else if (displayed_timescale == TS_DAY) {
    double fraction_alive = (double)time_duration / total_time;
    // one day goes from 2000-01-01 to 2000-01-02
    struct tm start = {0};
    start.tm_year = 100;
    start.tm_mon = 0;
    start.tm_mday = 1;
    struct tm end = {0};
    end.tm_year = 100;
    end.tm_mon = 0;
    end.tm_mday = 2;
    time_t start_in_secs = mktime(&start);
    time_t end_in_secs = mktime(&end);
    time_t mm_time = start_in_secs +
      (time_t) (fraction_alive * (end_in_secs - start_in_secs));
    struct tm *mm_time_struct = gmtime(&mm_time);
    // no suffix for calendar time scales
    strcat(format_str, reverse_time ? "-" : "");
    strcat(format_str, "%l:%M:%S %p");
    strftime(time_buffer, sizeof(time_buffer), format_str, mm_time_struct);
  }
  else if (displayed_timescale == TS_HOUR) {
    double fraction_alive = (double)time_duration / total_time;
    // one hour goes from 2000-01-01 11:00 to 2000-01-01 12:00
    struct tm start = {0};
    start.tm_year = 100;
    start.tm_mon = 0;
    start.tm_mday = 1;
    start.tm_hour = 11;
    struct tm end = {0};
    end.tm_year = 100;
    end.tm_mon = 0;
    end.tm_mday = 1;
    end.tm_hour = 12;
    time_t start_in_secs = mktime(&start);
    time_t end_in_secs = mktime(&end);
    time_t mm_time = start_in_secs +
      (time_t) (fraction_alive * (end_in_secs - start_in_secs));
    struct tm *mm_time_struct = gmtime(&mm_time);
    strcat(format_str, reverse_time ? "-" : "");
    strcat(format_str, "%l:%M:%S %p ");
    strftime(time_buffer, sizeof(time_buffer), format_str, mm_time_struct);
  }
  else if (displayed_timescale == TS_MONTH) {
    double fraction_alive = (double)time_duration / total_time;
    // one month goes from 2000-01-01 00:00 to 2000-02-01 00:00
    struct tm start = {0};
    start.tm_year = 100;
    start.tm_mon = 0;  // Jan
    start.tm_mday = 1;
    struct tm end = {0};
    end.tm_year = 100;
    end.tm_mon = 1;  // Feb
    end.tm_mday = 1;
    time_t start_in_secs = mktime(&start);
    time_t end_in_secs = mktime(&end);
    time_t mm_time = start_in_secs +
      (time_t) (fraction_alive * (end_in_secs - start_in_secs));
    struct tm *mm_time_struct = gmtime(&mm_time);
    strcat(format_str, reverse_time ? "-" : "");
    strcat(format_str, "%b %e\n%l:%M:%S %p ");
    strftime(time_buffer, sizeof(time_buffer), format_str, mm_time_struct);
  }
  else if (displayed_timescale == TS_YEAR) {
    double fraction_alive = (double)time_duration / total_time;
    // one year goes from 2000-01-01 00:00 to 2001-01-01 00:00
    struct tm start = {0};
    start.tm_year = 100;
    start.tm_mon = 0;
    start.tm_mday = 1;
    struct tm end = {0};
    end.tm_year = 101;
    end.tm_mon = 0;
    end.tm_mday = 1;
    time_t start_in_secs = mktime(&start);
    time_t end_in_secs = mktime(&end);
    time_t mm_time = start_in_secs +
      (time_t) (fraction_alive * (end_in_secs - start_in_secs));
    struct tm *mm_time_struct = gmtime(&mm_time);
    strcat(format_str, reverse_time ? "-" : "");
    strcat(format_str, "%b %e\n%l:%M:%S %p ");
    strftime(time_buffer, sizeof(time_buffer), format_str, mm_time_struct);
  }
  else if (displayed_timescale == TS_UNIVERSE) {
    double fraction_alive = (double)time_duration / total_time;
    /* Note Pebble only has 32 bit ints, so it can't handle
       15 billion years, so will use 15 million millenia instead
    */
    int universe_years = (int) (fraction_alive * 15000000);
    //    strcat(format_str, reverse_time ? "-" : "");
    strcat(format_str, "%d Millenia ");
    strcat(format_str, suffix);
    snprintf(time_buffer, sizeof(time_buffer), format_str, universe_years);
  }
  else if (displayed_timescale == TS_X_UNIVERSE) {
    // Pebble can't support enough years to make X_UNIVERSE work, so..
    /* double fraction_alive = (double)time_duration / total_time; */
    /* // one year goes from 4000 BC to 2001 AD */
    /* struct tm start = {0}; */
    /* // struct tm counts years from 1900 */
    /* start.tm_year = -5900; */
    /* start.tm_mon = 0; */
    /* start.tm_mday = 1; */
    /* struct tm end = {0}; */
    /* end.tm_year = 101; */
    /* end.tm_mon = 0; */
    /* end.tm_mday = 1; */
    /* int64_t start_in_secs = mktime(&start); */
    /* int64_t end_in_secs = mktime(&end); */
    /* time_t mm_time = start_in_secs + */
    /*   (time_t) (fraction_alive * (end_in_secs - start_in_secs)); */
    /* struct tm *mm_time_struct = gmtime(&mm_time); */
    /* strcat(format_str, reverse_time ? "-" : ""); */
    /* strcat(format_str, "%b %e %Y\n%l:%M:%S %p "); */
    /* strftime(time_buffer, sizeof(time_buffer), format_str, mm_time_struct); */
    double fraction_alive = (double)time_duration / total_time;
    double universe_years = fraction_alive * 6000;
    //    strcat(format_str, reverse_time ? "-" : "");
    strcat(format_str, "%d.%03d Yrs ");
    strcat(format_str, reverse_time ? "To Armageddon" : "From Creation");
    snprintf(time_buffer, sizeof(time_buffer), format_str, (int)universe_years,
	     get_decimal_portion_of_double(universe_years));
    
  }
  else if (displayed_timescale == TS_X_UNIVERSE_2) {
    double fraction_alive = (double)time_duration / total_time;
    double universe_years = fraction_alive * 6000;
    //    strcat(format_str, reverse_time ? "-" : "");
    strcat(format_str, "%d.%03d Yrs ");
    strcat(format_str, suffix);
    snprintf(time_buffer, sizeof(time_buffer), format_str, (int)universe_years,
	     get_decimal_portion_of_double(universe_years));
  }
  else { 			/* TS_NONE, TS_DEBUG, TS_ERROR */
    strcpy(time_buffer, "MorbidMeter\nSomething Ain't Right!?");
  }
  text_layer_set_text(s_time_layer, time_buffer);
  vertically_center_time_layer();
}

static void set_timescale() {
  text_layer_set_text(s_timescale_layer, get_string_from_timescale(displayed_timescale));
}

static void inbox_received_handler(DictionaryIterator *iter, void *context) {
  Tuple *background_color_t = dict_find(iter, MESSAGE_KEY_BACKGROUND_COLOR);
  Tuple *twenty_four_hour_format_t = dict_find(iter, MESSAGE_KEY_TWENTY_FOUR_HOUR_FORMAT);
  Tuple *local_time_show_seconds_t = dict_find(iter, MESSAGE_KEY_LOCAL_TIME_SHOW_SECONDS);
  Tuple *shake_wrist_toggles_time_t = dict_find(iter, MESSAGE_KEY_SHAKE_WRIST_TOGGLES_TIME);
  Tuple *timescale_t = dict_find(iter, MESSAGE_KEY_TIMESCALE);
  Tuple *reverse_time_t = dict_find(iter, MESSAGE_KEY_REVERSE_TIME);
  Tuple *start_date_time_in_secs_t = dict_find(iter, MESSAGE_KEY_START_DATE);
  Tuple *end_date_time_in_secs_t = dict_find(iter, MESSAGE_KEY_END_DATE);
  Tuple *fracture_time_t = dict_find(iter, MESSAGE_KEY_FRACTURE_TIME);
  Tuple *fracture_time_interval_t = dict_find(iter, MESSAGE_KEY_FRACTURE_TIME_INTERVAL);
  
  if (background_color_t) {
    int background_color = background_color_t->value->int32;
    persist_write_int(MESSAGE_KEY_BACKGROUND_COLOR, background_color);
    set_background_and_text_color(background_color);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "background color = %d", background_color);
  }
  if (twenty_four_hour_format_t) {
    twenty_four_hour_format = (bool) twenty_four_hour_format_t->value->int8;
    persist_write_bool(MESSAGE_KEY_TWENTY_FOUR_HOUR_FORMAT, twenty_four_hour_format);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "24 hr format  = %d", twenty_four_hour_format);
  }
  if (timescale_t) {
    strncpy(timescale_buffer, timescale_t->value->cstring, sizeof(timescale_buffer));
    persist_write_string(MESSAGE_KEY_TIMESCALE, timescale_buffer);
    selected_timescale = get_timescale_from_string(timescale_buffer);
    displayed_timescale = selected_timescale;
  }
  if (local_time_show_seconds_t) {
    local_time_show_seconds = (bool) local_time_show_seconds_t->value->int8;
    persist_write_bool(MESSAGE_KEY_LOCAL_TIME_SHOW_SECONDS, local_time_show_seconds);
  }
  if (shake_wrist_toggles_time_t) {
    shake_wrist_toggles_time = (bool) shake_wrist_toggles_time_t->value->int8;
    persist_write_bool(MESSAGE_KEY_SHAKE_WRIST_TOGGLES_TIME, shake_wrist_toggles_time);
  }
  if (reverse_time_t) {
    reverse_time = (bool) reverse_time_t->value->int8;
    persist_write_bool(MESSAGE_KEY_REVERSE_TIME, reverse_time);
  }
  if (start_date_time_in_secs_t) {
    persist_write_string(MESSAGE_KEY_START_DATE, start_date_time_in_secs_t->value->cstring);
    start_date_time_in_secs = (int64_t)myatof(start_date_time_in_secs_t->value->cstring);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Start date in secs = %d", (int)start_date_time_in_secs);
  }
  if (end_date_time_in_secs_t) {
    persist_write_string(MESSAGE_KEY_END_DATE, end_date_time_in_secs_t->value->cstring);
    end_date_time_in_secs = (int64_t)myatof(end_date_time_in_secs_t->value->cstring);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "End date in secs = %d", (int)end_date_time_in_secs);
  }
  if (fracture_time_t) {
    fracture_time  = (bool) fracture_time_t->value->int8;
    persist_write_bool(MESSAGE_KEY_FRACTURE_TIME, fracture_time);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "fracture time   = %d", fracture_time);
  }
  if (fracture_time_interval_t) {
    strncpy(fracture_time_buffer, fracture_time_interval_t->value->cstring, sizeof(fracture_time_buffer));
    persist_write_string(MESSAGE_KEY_FRACTURE_TIME_INTERVAL, fracture_time_buffer);
    fracture_time_interval = get_fracturetimeinterval_from_string(fracture_time_buffer);
  }

  // reseed random number generator
  srand(time(NULL));
  // config resets timer buzz
  timer_expired = false;
  set_timescale();
  fracturetime_counter = MAX_COUNTER;
  update_time();
}

static void main_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  // Create bitmap layer
#if defined(PBL_BW)
  s_bitmap = gbitmap_create_with_resource(RESOURCE_ID_SKULL_IMAGE_APLITE);
#elif defined(PBL_COLOR)
  s_bitmap = gbitmap_create_with_resource(RESOURCE_ID_SKULL_IMAGE);
#endif
  s_bitmap_layer = bitmap_layer_create(GRect(0, 0, bounds.size.w, 48));
  bitmap_layer_set_bitmap(s_bitmap_layer, s_bitmap);
#if defined(PBL_BW)
  bitmap_layer_set_compositing_mode(s_bitmap_layer, GCompOpAssign);
#elif defined(PBL_COLOR)
  bitmap_layer_set_compositing_mode(s_bitmap_layer, GCompOpSet);
#endif

  layer_add_child(window_layer, bitmap_layer_get_layer(s_bitmap_layer));
  // Create time TextLayer
  s_time_layer = text_layer_create(GRect(0, 30, bounds.size.w, 100));
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorBlack);
  text_layer_set_text(s_time_layer, MM_TITLE "\nLoading");
  text_layer_set_font(s_time_layer,
		      fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  // vertically center text
  GSize size = text_layer_get_content_size(s_time_layer);
  layer_set_frame((Layer *)s_time_layer, GRect(0, (bounds.size.h - size.h) / 2, bounds.size.w,
				      100));

  // Create timescale TextLayer
#if defined(PBL_RECT)
  s_timescale_layer = text_layer_create(GRect(0, bounds.size.h - 20,
					      bounds.size.w, 20));
#elif defined(PBL_ROUND)
  s_timescale_layer = text_layer_create(GRect(0, bounds.size.h - 40,
					      bounds.size.w, 20));
#endif
  text_layer_set_background_color(s_timescale_layer, GColorClear);
  text_layer_set_text_color(s_timescale_layer, GColorBlack);
  text_layer_set_font(s_timescale_layer,
		      fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  text_layer_set_text_alignment(s_timescale_layer, GTextAlignmentCenter);
  strncpy(timescale_buffer, LOCAL_TIME, sizeof(timescale_buffer));

  if (persist_read_int(MESSAGE_KEY_BACKGROUND_COLOR)) {
    int background_color = persist_read_int(MESSAGE_KEY_BACKGROUND_COLOR);
    set_background_and_text_color(background_color);
  }
  if (persist_read_bool(MESSAGE_KEY_TWENTY_FOUR_HOUR_FORMAT)) {
    twenty_four_hour_format = persist_read_bool(MESSAGE_KEY_TWENTY_FOUR_HOUR_FORMAT);
  }

  if (persist_exists(MESSAGE_KEY_TIMESCALE)) {
    persist_read_string(MESSAGE_KEY_TIMESCALE, timescale_buffer, sizeof(timescale_buffer));
    selected_timescale = get_timescale_from_string(timescale_buffer);
    displayed_timescale = selected_timescale;
  }
  if (persist_read_bool(MESSAGE_KEY_LOCAL_TIME_SHOW_SECONDS)) {
    local_time_show_seconds = persist_read_bool(MESSAGE_KEY_LOCAL_TIME_SHOW_SECONDS);
  }
  if (persist_read_bool(MESSAGE_KEY_SHAKE_WRIST_TOGGLES_TIME)) {
    shake_wrist_toggles_time = persist_read_bool(MESSAGE_KEY_SHAKE_WRIST_TOGGLES_TIME);
  }
  if (persist_read_bool(MESSAGE_KEY_REVERSE_TIME)) {
    reverse_time = persist_read_bool(MESSAGE_KEY_REVERSE_TIME);
  }
  char buf[30];
  if (persist_exists(MESSAGE_KEY_START_DATE)) {
    persist_read_string(MESSAGE_KEY_START_DATE, buf,
			sizeof(buf));
    start_date_time_in_secs = (int64_t)myatof(buf);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "start = %d", (int)start_date_time_in_secs);
  }
  if (persist_exists(MESSAGE_KEY_END_DATE)) {
    persist_read_string(MESSAGE_KEY_END_DATE, buf,
			sizeof(buf));
    end_date_time_in_secs = (int64_t)myatof(buf);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "end = %d", (int)end_date_time_in_secs);
  }
  if (persist_exists(MESSAGE_KEY_FRACTURE_TIME_INTERVAL)) {
    persist_read_string(MESSAGE_KEY_FRACTURE_TIME_INTERVAL, fracture_time_buffer, sizeof(fracture_time_buffer));
    fracture_time_interval = get_fracturetimeinterval_from_string(fracture_time_buffer);
  }
  if (persist_read_bool(MESSAGE_KEY_FRACTURE_TIME)) {
    fracture_time = persist_read_bool(MESSAGE_KEY_FRACTURE_TIME);
  }

  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_timescale_layer));

  //  fracturetime_counter = 
  update_time();
  set_timescale();
}

static void main_window_unload(Window *window) {
  gbitmap_destroy(s_bitmap);
  bitmap_layer_destroy(s_bitmap_layer);
  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_timescale_layer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

static void tap_handler(AccelAxisType axis, int32_t direction) {
  /* (void)axis; */
  /* (void)direction; */
  // return if Local Time or config doesn't allow shaking
  if (selected_timescale == TS_LOCAL_TIME || !shake_wrist_toggles_time) {
    return;
  }
  if (displayed_timescale == selected_timescale) {
    displayed_timescale = alternative_timescale;
  }
  else {
    displayed_timescale = selected_timescale;
  }
  // sneaky way to force redisplay of fractured time
  fracturetime_counter = MAX_COUNTER;
  update_time();
  set_timescale();
}
  
static void init() {
  s_main_window = window_create();
 
  window_set_window_handlers(s_main_window, (WindowHandlers) {
      .load = main_window_load,
	.unload = main_window_unload
	});

  window_stack_push(s_main_window, true);
  
  tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
  accel_tap_service_subscribe(tap_handler);

  app_message_register_inbox_received(inbox_received_handler);
  app_message_open(1024, 1024);
}

static void deinit() {
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}

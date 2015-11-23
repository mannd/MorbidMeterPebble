#include <pebble.h>
#include "timescales.h"

#define MM_TITLE "MorbidMeter"
#define DATE "%n%b %e %Y"

#define KEY_BACKGROUND_COLOR 0
#define KEY_TWENTY_FOUR_HOUR_FORMAT 1
#define KEY_TIMESCALE 2
#define KEY_LOCAL_TIME_SHOW_SECONDS 3
#define KEY_SHAKE_WRIST_TOGGLES_TIME 4
#define KEY_REVERSE_TIME 5
#define KEY_START_DATE_TIME_IN_SECS 6
#define KEY_END_DATE_TIME_IN_SECS 7

#define SECS_IN_HOUR (60 * 60)
#define SECS_IN_DAY (24 * SECS_IN_HOUR)
#define SECS_IN_YEAR (365.25 * SECS_IN_DAY)
    
static Window *s_main_window;
static TextLayer *s_time_layer;
static TextLayer *s_timescale_layer;
static bool twenty_four_hour_format = false;
static bool shake_wrist_toggles_time = true;
static bool reverse_time = false;
static time_t start_date_time_in_secs = 0;
static time_t end_date_time_in_secs = 0;

static char time_buffer[] = MM_TITLE "\nMMM 00 0000\n00:00:00 pm";
static char timescale_buffer[] = "   " LOCAL_TIME "   ";
static bool local_time_show_seconds = true;

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

static int get_decimal_portion_of_double(double d) {
  return (int)((d < 0 ? -d : d) * 1000) % 1000;
}

static void update_time() {
  time_t real_time = time(NULL);
  struct tm *tick_time = localtime(&real_time);
  time_t diff = real_time - start_date_time_in_secs;
  time_t reverse_diff = end_date_time_in_secs - real_time;
  // handle bad situations here
  
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
    if (!reverse_time) {
      double percent_time = 100.0 * (double)diff /
	(end_date_time_in_secs - start_date_time_in_secs);
      snprintf(time_buffer, sizeof(time_buffer), MM_TITLE "\n%d.%03d%%\nDone", (int)percent_time,
	       get_decimal_portion_of_double(percent_time));
    }
    else {
      double percent_time = (double) 100.0 * (double)reverse_diff /
	(end_date_time_in_secs - start_date_time_in_secs);
      snprintf(time_buffer, sizeof(time_buffer), MM_TITLE "\n%d.%03d%%\nLeft", (int)percent_time,
	       get_decimal_portion_of_double(percent_time));
    }
  }
  else if (displayed_timescale == TS_SECONDS) {
    if (!reverse_time) {
      snprintf(time_buffer, sizeof(time_buffer), MM_TITLE "\n%d \nSecs", (int)diff);
    }
    else {
      snprintf(time_buffer, sizeof(time_buffer), MM_TITLE "\n%d \nSecs Left", (int)reverse_diff);
    }
  }
  else if (displayed_timescale == TS_MINUTES) {
    if (!reverse_time) {
      snprintf(time_buffer, sizeof(time_buffer), MM_TITLE "\n%d \nMins", (int)diff / 60);
    }
    else {
      snprintf(time_buffer, sizeof(time_buffer), MM_TITLE "\n%d \nMins Left", (int)reverse_diff / 60);
    }
  }
  else if (displayed_timescale == TS_HOURS) {
    if (!reverse_time) {
      double hours = diff / SECS_IN_HOUR;
      snprintf(time_buffer, sizeof(time_buffer), MM_TITLE "\n%d.%03d \nHours", (int)hours,
	       get_decimal_portion_of_double(hours));
    }
    else {
      double hours = reverse_diff / SECS_IN_HOUR;
      snprintf(time_buffer, sizeof(time_buffer), MM_TITLE "\n%d.%03d \nHours Left", (int)hours,
	       get_decimal_portion_of_double(hours));
    }
  }
  else if (displayed_timescale == TS_DAYS) {
    if (!reverse_time) {
      double days = diff / SECS_IN_DAY;
      snprintf(time_buffer, sizeof(time_buffer), MM_TITLE "\n%d.%03d \nDays", (int)days,
	       get_decimal_portion_of_double(days));
    }
    else {
      double days = reverse_diff / SECS_IN_DAY;
      snprintf(time_buffer, sizeof(time_buffer), MM_TITLE "\n%d.%03d \nDays Left", (int)days,
	       get_decimal_portion_of_double(days));
    }
  }
  else if (displayed_timescale == TS_YEARS) {
    if (!reverse_time) {
      double years = diff / SECS_IN_YEAR;
      snprintf(time_buffer, sizeof(time_buffer), MM_TITLE "\n%d.%0d \nYears", (int)years,
	       get_decimal_portion_of_double(years));
    }
    else {
      double years = reverse_diff / SECS_IN_YEAR;
      snprintf(time_buffer, sizeof(time_buffer), MM_TITLE "\n%d.%0d \nYears Left", (int)years,
	       get_decimal_portion_of_double(years));
    }
  }
  /* More and more and more timescales!! */
  else {
    strcpy(time_buffer, "MorbidMeter\nTime\nSoon!");
  }
  text_layer_set_text(s_time_layer, time_buffer);
}

static void set_timescale() {
  text_layer_set_text(s_timescale_layer, get_string_from_timescale(displayed_timescale));
}

static void inbox_received_handler(DictionaryIterator *iter, void *context) {
  Tuple *background_color_t = dict_find(iter, KEY_BACKGROUND_COLOR);
  Tuple *twenty_four_hour_format_t = dict_find(iter, KEY_TWENTY_FOUR_HOUR_FORMAT);
  Tuple *timescale_t = dict_find(iter, KEY_TIMESCALE);
  Tuple *local_time_show_seconds_t = dict_find(iter, KEY_LOCAL_TIME_SHOW_SECONDS);
  Tuple *shake_wrist_toggles_time_t = dict_find(iter, KEY_SHAKE_WRIST_TOGGLES_TIME);
  Tuple *reverse_time_t = dict_find(iter, KEY_REVERSE_TIME);
  Tuple *start_date_time_in_secs_t = dict_find(iter, KEY_START_DATE_TIME_IN_SECS);
  Tuple *end_date_time_in_secs_t = dict_find(iter, KEY_END_DATE_TIME_IN_SECS);

  if (background_color_t) {
    int background_color = background_color_t->value->int32;
    persist_write_int(KEY_BACKGROUND_COLOR, background_color);
    set_background_and_text_color(background_color);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "background color = %d", background_color);
  }
  if (twenty_four_hour_format_t) {
    twenty_four_hour_format = (bool) twenty_four_hour_format_t->value->int8;
    persist_write_bool(KEY_TWENTY_FOUR_HOUR_FORMAT, twenty_four_hour_format);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "24 hr format  = %d", twenty_four_hour_format);
  }
  if (timescale_t) {
    strncpy(timescale_buffer, timescale_t->value->cstring, sizeof(timescale_buffer));
    persist_write_string(KEY_TIMESCALE, timescale_buffer);
    selected_timescale = get_timescale_from_string(timescale_buffer);
    displayed_timescale = selected_timescale;
  }
  if (local_time_show_seconds_t) {
    local_time_show_seconds = (bool) local_time_show_seconds_t->value->int8;
    persist_write_bool(KEY_LOCAL_TIME_SHOW_SECONDS, local_time_show_seconds);
  }
  if (shake_wrist_toggles_time_t) {
    shake_wrist_toggles_time = (bool) shake_wrist_toggles_time_t->value->int8;
    persist_write_bool(KEY_SHAKE_WRIST_TOGGLES_TIME, shake_wrist_toggles_time);
  }
  if (reverse_time_t) {
    reverse_time = (bool) reverse_time_t->value->int8;
    persist_write_bool(KEY_REVERSE_TIME, reverse_time);
    // update timescale and reverse time
    // prepend '-' to timescale with reverse time?
    /* APP_LOG(APP_LOG_LEVEL_DEBUG, "reverse_time = %d", reverse_time); */
  }
  if (start_date_time_in_secs_t) {
    start_date_time_in_secs = (time_t)start_date_time_in_secs_t->value->uint32;
    persist_write_int(KEY_START_DATE_TIME_IN_SECS, start_date_time_in_secs);
    struct tm *time_struct = localtime(&start_date_time_in_secs);
    char tmp_date_buffer[30];
    strftime(tmp_date_buffer, sizeof(tmp_date_buffer),
    	     DATE "\n%l:%M %p", time_struct);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "start date time = %s", tmp_date_buffer);
  }
  if (end_date_time_in_secs_t) {
    end_date_time_in_secs = (time_t)end_date_time_in_secs_t->value->uint32;
    persist_write_int(KEY_END_DATE_TIME_IN_SECS, end_date_time_in_secs);
    struct tm *time_struct = localtime(&end_date_time_in_secs);
    char tmp_date_buffer[30];
    strftime(tmp_date_buffer, sizeof(tmp_date_buffer),
    	     DATE "\n%l:%M %p", time_struct);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "end date time = %s", tmp_date_buffer);
    time_t real_time = time(NULL);
    time_t diff = real_time - start_date_time_in_secs;
    time_t reverse_diff = end_date_time_in_secs - real_time;
    APP_LOG(APP_LOG_LEVEL_DEBUG, "real_time = %d, diff = %d, reverse_diff = %d",
	    (int)real_time, (int)diff, (int)reverse_diff);
  }

  set_timescale();
  update_time();

}

static void main_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  // Create time TextLayer
  s_time_layer = text_layer_create(GRect(0, 30, bounds.size.w, 100));
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorBlack);
  text_layer_set_text(s_time_layer, MM_TITLE "\n00:00:00");
  text_layer_set_font(s_time_layer,
		      fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);

  // Create timescale TextLayer
#if defined(PBL_RECT)
  s_timescale_layer = text_layer_create(GRect(0, bounds.size.h - 20,
					      bounds.size.w, 20));
#elif defined(PBL_ROUND)
  s_timescale_layer = text_layer_create(GRect(0, bounds.size.h - 50,
					      bounds.size.w, 20));
#endif
  text_layer_set_background_color(s_timescale_layer, GColorClear);
  text_layer_set_text_color(s_timescale_layer, GColorBlack);
  text_layer_set_font(s_timescale_layer,
		      fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  text_layer_set_text_alignment(s_timescale_layer, GTextAlignmentCenter);
  strncpy(timescale_buffer, LOCAL_TIME, sizeof(timescale_buffer));

  if (persist_read_int(KEY_BACKGROUND_COLOR)) {
    int background_color = persist_read_int(KEY_BACKGROUND_COLOR);
    set_background_and_text_color(background_color);
  }
  if (persist_read_bool(KEY_TWENTY_FOUR_HOUR_FORMAT)) {
    twenty_four_hour_format = persist_read_bool(KEY_TWENTY_FOUR_HOUR_FORMAT);
  }
  char tmp_buffer[sizeof(timescale_buffer)];
  if (persist_read_string(KEY_TIMESCALE, tmp_buffer,
    			  sizeof(tmp_buffer)) > 0) {
    strncpy(timescale_buffer, tmp_buffer, sizeof(timescale_buffer));
    selected_timescale = get_timescale_from_string(timescale_buffer);
    displayed_timescale = selected_timescale;
  }
  if (persist_read_bool(KEY_LOCAL_TIME_SHOW_SECONDS)) {
    local_time_show_seconds = persist_read_bool(KEY_LOCAL_TIME_SHOW_SECONDS);
  }
  if (persist_read_bool(KEY_SHAKE_WRIST_TOGGLES_TIME)) {
    shake_wrist_toggles_time = persist_read_bool(KEY_SHAKE_WRIST_TOGGLES_TIME);
  }
  if (persist_read_bool(KEY_REVERSE_TIME)) {
    reverse_time = persist_read_bool(KEY_REVERSE_TIME);
  }
  if (persist_read_int(KEY_START_DATE_TIME_IN_SECS)) {
    start_date_time_in_secs = persist_read_int(KEY_START_DATE_TIME_IN_SECS);
  }
  if (persist_read_int(KEY_END_DATE_TIME_IN_SECS)) {
    end_date_time_in_secs = persist_read_int(KEY_END_DATE_TIME_IN_SECS);
  }
  
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_timescale_layer));

  update_time();
  set_timescale();
}

static void main_window_unload(Window *window) {
  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_timescale_layer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

static void tap_handler(AccelAxisType axis, int32_t direction) {
  /* (void)axis; */
  /* (void)direction; */
  if (selected_timescale == TS_LOCAL_TIME || !shake_wrist_toggles_time) {
    return;
  }
  // return if Local Time or config doesn't allow shaking
  if (displayed_timescale == selected_timescale) {
    displayed_timescale = alternative_timescale;
  }
  else {
    displayed_timescale = selected_timescale;
  }
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
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
}

static void deinit() {
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}

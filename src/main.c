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
#define KEY_START_DATE 6
#define KEY_START_TIME 7
#define KEY_END_DATE 8
#define KEY_END_TIME 9
#define KEY_START_DATE_TIME_IN_SECS 10
#define KEY_END_DATE_TIME_IN_SECS 11
    
static Window *s_main_window;
static TextLayer *s_time_layer;
static TextLayer *s_timescale_layer;
static bool twenty_four_hour_format = false;
static bool shake_wrist_toggles_time;
static bool reverse_time = false;
static time_t start_date_time_in_secs = 0;
static time_t end_date_time_in_secs = 0;

/// TODO this needs to be an enum for all the timescales
// enum Timescale { Local_Time, etc. }
static bool is_local_time = true;
static char time_buffer[] = MM_TITLE "\nMMM 00 0000\n00:00:00 pm";
static char timescale_buffer[] = "   " LOCAL_TIME "   ";
static bool local_time_show_seconds = true;

static timescale selected_timescale = TS_LOCAL_TIME;
static timescale displayed_timescale = TS_LOCAL_TIME;

static void set_background_and_text_color(int color) {
#ifdef PBL_SDK_3
  GColor background_color = GColorFromHEX(color);
  window_set_background_color(s_main_window, background_color);
  text_layer_set_text_color(s_time_layer, gcolor_legible_over(background_color));
  text_layer_set_text_color(s_timescale_layer, gcolor_legible_over(background_color));
#endif
}

static bool local_time_update_with_secs() {
  return local_time_show_seconds;
}

static void update_time() {
  if (is_local_time) {
    time_t temp = time(NULL); 
    struct tm *tick_time = localtime(&temp);
    // Write the current hours and minutes into the buffer
    if (clock_is_24h_style() == twenty_four_hour_format) {
      if (local_time_update_with_secs()) {
	strftime(time_buffer, sizeof(time_buffer),
		 MM_TITLE DATE "\n%H:%M:%S", tick_time);
      } else {
	strftime(time_buffer, sizeof(time_buffer),
		 MM_TITLE DATE "\n%H:%M", tick_time);
      }
    } else {
      if (local_time_update_with_secs()) {
	strftime(time_buffer, sizeof(time_buffer),
		 MM_TITLE DATE "\n%l:%M:%S %p", tick_time);
      } else {
	strftime(time_buffer, sizeof(time_buffer),
		 MM_TITLE DATE "\n%l:%M %p", tick_time);
      }
    }
  } else {
    strcpy(time_buffer, "MorbidMeter\nTime\nSoon!");
  }
  text_layer_set_text(s_time_layer, time_buffer);
}

static void set_timescale() {
  is_local_time = (strcmp(timescale_buffer, LOCAL_TIME) == 0);
  text_layer_set_text(s_timescale_layer, timescale_buffer);
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
  }
  if (twenty_four_hour_format_t) {
    twenty_four_hour_format = twenty_four_hour_format_t->value->int8;
    persist_write_int(KEY_TWENTY_FOUR_HOUR_FORMAT, twenty_four_hour_format);
    update_time();
  }
  if (timescale_t) {
    strncpy(timescale_buffer, timescale_t->value->cstring, sizeof(timescale_buffer));
    persist_write_string(KEY_TIMESCALE, timescale_buffer);
    set_timescale();
  }
  if (local_time_show_seconds_t) {
    local_time_show_seconds = local_time_show_seconds_t->value->int8;
    persist_write_int(KEY_LOCAL_TIME_SHOW_SECONDS, local_time_show_seconds);
    update_time();
  }
  if (shake_wrist_toggles_time_t) {
    shake_wrist_toggles_time = shake_wrist_toggles_time_t->value->int8;
    persist_write_int(KEY_SHAKE_WRIST_TOGGLES_TIME, shake_wrist_toggles_time);
  }
  if (reverse_time_t) {
    reverse_time = reverse_time_t->value->int8;
    persist_write_int(KEY_REVERSE_TIME, reverse_time);
    // update timescale and reverse time
    // prepend '-' to timescale with reverse time?
  }
  if (start_date_time_in_secs_t) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "start_date_time_in_secs_t = %lu",
	    start_date_time_in_secs_t->value->uint32);
    char tmp_date_buf[30];
    start_date_time_in_secs = (time_t)start_date_time_in_secs_t->value->uint32;
    persist_write_int(KEY_START_DATE_TIME_IN_SECS, start_date_time_in_secs);
    struct tm *time_struct = localtime(&start_date_time_in_secs);
    strftime(tmp_date_buf, sizeof(tmp_date_buf),
	     DATE "\n%l:%M %p", time_struct);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "start_date_time_in_secs_t = %lu",
	    start_date_time_in_secs_t->value->uint32);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "clock_is_timezone_set = %s",
	    (clock_is_timezone_set() ? "true" : "false"));
    APP_LOG(APP_LOG_LEVEL_DEBUG, "start_date_time = %s", tmp_date_buf);
  }
  if (end_date_time_in_secs_t) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "end_date_time_in_secs_t = %lu",
	    end_date_time_in_secs_t->value->uint32);
    char tmp_date_buf[30];
    end_date_time_in_secs = (time_t)end_date_time_in_secs_t->value->uint32;
    persist_write_int(KEY_END_DATE_TIME_IN_SECS, end_date_time_in_secs);
    struct tm *time_struct = localtime(&end_date_time_in_secs);
    strftime(tmp_date_buf, sizeof(tmp_date_buf),
	     DATE "\n%l:%M %p", time_struct);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "end_date_time_in_secs_t = %lu",
	    end_date_time_in_secs_t->value->uint32);
    APP_LOG(APP_LOG_LEVEL_DEBUG, "end_date_time = %s", tmp_date_buf);
  }
    
}

static void set_selected_timescale() {
  selected_timescale = get_selected_timescale(timescale_buffer);
}

static void main_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  // Create time TextLayer
  s_time_layer = text_layer_create(GRect(0, 30, bounds.size.w, 100));
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorBlack);
  text_layer_set_text(s_time_layer, MM_TITLE "\n00:00:00");
  text_layer_set_font(s_time_layer, fonts_get_system_font(
  FONT_KEY_GOTHIC_28_BOLD));
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
  text_layer_set_font(s_timescale_layer, fonts_get_system_font(
  FONT_KEY_GOTHIC_18_BOLD));
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
    set_selected_timescale();
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
  is_local_time = !is_local_time;
  if (is_local_time) {
    displayed_timescale = selected_timescale;
  // need to actually change timescales here
  }
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

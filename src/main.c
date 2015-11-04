#include <pebble.h>

#define MM_TITLE "MorbidMeter"
#define DATE "%n%b %e %Y"
#define LOCAL_TIME "Local Time"

#define KEY_BACKGROUND_COLOR 0
#define KEY_TWENTY_FOUR_HOUR_FORMAT 1
#define KEY_TIMESCALE 2
    
static Window *s_main_window;
static TextLayer *s_time_layer;
static TextLayer *s_timescale_layer;
static bool twenty_four_hour_format = false;
static char time_buffer[] = MM_TITLE "\nMMM 00 0000\n00:00:00 pm";
static char timescale_buffer[] = LOCAL_TIME;

static void set_background_and_text_color(int color) {
#ifdef PBL_SDK_3
  GColor background_color = GColorFromHEX(color);
  window_set_background_color(s_main_window, background_color);
  text_layer_set_text_color(s_time_layer, gcolor_legible_over(background_color));
  text_layer_set_text_color(s_timescale_layer, gcolor_legible_over(background_color));
#endif
}

static void update_time() {
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);
  // Write the current hours and minutes into the buffer
  if(clock_is_24h_style() == twenty_four_hour_format) {
    strftime(time_buffer, sizeof(time_buffer),
             MM_TITLE DATE "\n%H:%M:%S", tick_time);
  } else {
    strftime(time_buffer, sizeof(time_buffer),
             MM_TITLE DATE "\n%I:%M:%S %p", tick_time);
  }
  text_layer_set_text(s_time_layer, time_buffer);
}

static void set_timescale() {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "timescale_buffer is still %s!", timescale_buffer);  
  text_layer_set_text(s_timescale_layer, timescale_buffer);
}

static void inbox_received_handler(DictionaryIterator *iter, void *context) {
  Tuple *background_color_t = dict_find(iter, KEY_BACKGROUND_COLOR);
  Tuple *twenty_four_hour_format_t = dict_find(iter, KEY_TWENTY_FOUR_HOUR_FORMAT);
  Tuple *timescale_t = dict_find(iter, KEY_TIMESCALE);

  APP_LOG(APP_LOG_LEVEL_DEBUG, "inbox config data received");

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
  APP_LOG(APP_LOG_LEVEL_DEBUG, "timescale_t = %d", (int)timescale_t);
  if (timescale_t) {
    strncpy(timescale_buffer, timescale_t->value->cstring, sizeof(timescale_buffer));
    APP_LOG(APP_LOG_LEVEL_DEBUG, "timescale_t received");
    persist_write_string(KEY_TIMESCALE, timescale_buffer);
    set_timescale();
  }
}

static void main_window_load(Window *window) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "main window loading");

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
  APP_LOG(APP_LOG_LEVEL_DEBUG, "timescale_buffer is %s", timescale_buffer);

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

  
static void init() {
  s_main_window = window_create();
 
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  window_stack_push(s_main_window, true);
  
  // Register with TickTimerService
  tick_timer_service_subscribe(SECOND_UNIT, tick_handler);

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


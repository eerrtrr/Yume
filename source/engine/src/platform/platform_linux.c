#include "platform.h"

#if PLATFORM_LINUX

#include "../utils/logger/logger.h"

#include <xcb/xcb.h>
#include <X11/keysym.h>
#include <X11/XKBlib.h>
#include <X11/Xlib.h>
#include <X11/Xlib-xcb.h>
#include <sys/time.h>

#if _POSIX_C_SOURCE >= 199309L
#include <time.h>  // nanosleep
#else
#include <unistd.h>  // usleep
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct internal_state {
    Display* display;
    xcb_connection_t* connection;
    xcb_window_t window;
    xcb_screen_t* screen;
    xcb_atom_t wm_protocols;
    xcb_atom_t wm_delete_win;
} internal_state;

bool platform_startup(platform_state *plat_state, const char *app_name, uint32_t x, uint32_t y, uint32_t width, uint32_t height){
    plat_state->internal_state = (internal_state*)malloc(sizeof(internal_state));
    internal_state *state = (internal_state *)plat_state->internal_state;

    //Connect to X server
    state->display = XOpenDisplay(NULL);

    //Turn off key repeat
    XAutoRepeatOff(state->display);
    //Get connection from X server
    state->connection = XGetXCBConnection(state->display);

    if(xcb_connection_has_error(state->connection)){
        LOG_FATAL("Failed to connect to X server");
        return false;
    }

    const struct xcb_setup_t *setup = xcb_get_setup(state->connection);
    //Loop through all screens
    xcb_screen_iterator_t iterator = xcb_setup_roots_iterator(setup);
    int32_t screen_count = 0;
    for(int32_t s=screen_count; s < screen_count; s--){
        xcb_screen_next(&iterator);
    }

    //Assign screen to state
    state->screen = iterator.data;

    //Id allocation
    state->window = xcb_generate_id(state->connection);

    //Event type registration
    uint32_t event_mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
    uint32_t event_mask_values = XCB_EVENT_MASK_BUTTON_PRESS | XCB_EVENT_MASK_BUTTON_RELEASE | 
                                 XCB_EVENT_MASK_KEY_PRESS | XCB_EVENT_MASK_KEY_RELEASE |
                                 XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_STRUCTURE_NOTIFY |
                                 XCB_EVENT_MASK_POINTER_MOTION;

    //Values for xcb window creation
    uint32_t value_list[2] = {state->screen->black_pixel, event_mask_values};

    //Create window
    xcb_void_cookie_t cookie = xcb_create_window(state->connection, XCB_COPY_FROM_PARENT, state->window, state->screen->root,
                        x, y, width, height, 0, XCB_WINDOW_CLASS_INPUT_OUTPUT, state->screen->root_visual,
                        event_mask, value_list);

    //Set window title
    xcb_change_property(state->connection, XCB_PROP_MODE_REPLACE, state->window, XCB_ATOM_WM_NAME, XCB_ATOM_STRING, 8,
                        strlen(app_name), app_name);

    //Window magement and window destroy
    xcb_intern_atom_cookie_t wm_delete_cookie = xcb_intern_atom(state->connection, 0, strlen("WM_PROTOCOLS"), "WM_PROTOCOLS");
    xcb_intern_atom_cookie_t wm_protocols_delete_cookie = xcb_intern_atom(state->connection, 0, strlen("WM_DELETE_WINDOW"), "WM_DELETE_WINDOW");

    xcb_intern_atom_reply_t *wm_delete_reply = xcb_intern_atom_reply(state->connection, wm_delete_cookie, NULL);
    xcb_intern_atom_reply_t *wm_protocol_reply = xcb_intern_atom_reply(state->connection, wm_protocols_delete_cookie, NULL);
    
    state->wm_delete_win = wm_delete_reply->atom;
    state->wm_protocols = wm_protocol_reply->atom;

    
    xcb_change_property(state->connection, XCB_PROP_MODE_REPLACE, state->window, state->wm_protocols, XCB_ATOM_ATOM, 32,
                        1, &state->wm_delete_win);

    //Map window to screen
    xcb_map_window(state->connection, state->window);

    //Flush stream to assure that everything is sent to X server
    int32_t xcb_flush_stream_result = xcb_flush(state->connection);
    if(xcb_flush_stream_result <= 0){
        LOG_FATAL("Failed to flush stream : %D", xcb_flush_stream_result);
        return false;
    }

    return true;

    UNUSED(cookie);
}


void platform_shutdown(platform_state *plat_state){
    internal_state *state = (internal_state*)plat_state->internal_state;

    //Turn on key repeat
    XAutoRepeatOn(state->display);

    //Destroy window
    xcb_destroy_window(state->connection, state->window);

    //Close connection to X server
    XCloseDisplay(state->display);

    free(state);
}

bool platform_pump_message(platform_state *plat_state){
    internal_state *state = (internal_state*)plat_state->internal_state;

    xcb_generic_event_t *event;
    xcb_client_message_event_t *cm;

    //Quit flag
    bool quit_flagged = false;

    //Poll all events
    while((event = xcb_poll_for_event(state->connection))){
        if(event == 0){
            break;
        }

        //Input event switch
        switch(event->response_type & ~0x80){
        case XCB_KEY_PRESS:
        case XCB_KEY_RELEASE: {
            // TODO : Key press and release events
            break;
        }
        case XCB_BUTTON_PRESS:
        case XCB_BUTTON_RELEASE: {
            // TODO : Button press and release events
            break;
        }
        
        case XCB_MOTION_NOTIFY: {
            // TODO : Mouse move events
            break;
        }

        case XCB_CONFIGURE_NOTIFY: {
            // TODO : Window resize events
            break;
        }

        case XCB_CLIENT_MESSAGE: {
            cm = (xcb_client_message_event_t*)event;
            if(cm->data.data32[0] == state->wm_protocols && cm->data.data32[1] == state->wm_delete_win){
                quit_flagged = true;
                break;
            }
            break;
        }

        default:
            break;
        }

        free(event);
    }

    return !quit_flagged;
}

void* platform_allocate(uint64_t size, bool aligned){
    UNUSED(aligned);
    return malloc(size);
}

void platform_free(void* block, bool aligned){
    UNUSED(aligned);
    free(block);
}

void* platform_zero_memory(void* block, uint64_t size){
    return memset(block, 0, size);
}

void* platform_copy_memory(void* dest, const void* source, uint64_t size){
    return memcpy(dest, source, size);
}

void* platform_set_memory(void* dest, int32_t value, uint64_t size){
    return memset(dest, value, size);
}

void platform_console_write(const char* message, uint8_t colour){
    //FATAL, ERROR, WARN, INFO, DEBUG
    const char* colours[] = {"0;41", "1;31", "1;33", "1;32", "1;34"};
    printf("\033[%sm%s\033[0m\n", colours[colour], message);
}

//Not used for the moment
//Used for another stream like stderr
void platform_console_write_error(const char* message, uint8_t colour){
    //FATAL, ERROR, WARN, INFO, DEBUG
    const char* colours[] = {"0;41", "1;31", "1;33", "1;32", "1;34"};
    printf("\033[%sm%s\033[0m\n", colours[colour], message);
}

float platform_get_absolute_time(){
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec * 0.000000001;
}

void platform_sleep(uint64_t ms){
#if _POSIX_C_SOURCE >= 199309L
    struct timespec ts;
    ts.tv_sec = ms / 1000;
    ts.tv_nsec = (ms % 1000) * 1000000;
    nanosleep(&ts, NULL);
#else
    if(ms >= 1000){
        sleep(ms / 1000);
    }
    usleep((ms % 1000) * 1000);
#endif
}

#endif
#include "utils/logger/logger.h"
#include "platform/platform.h"
#include <stdlib.h>

int main(){
    LOG_INFO("Starting application...");
    platform_state* state = malloc(sizeof(platform_state));
    platform_startup(state, "Yume", 0, 0, 500, 500);
    while(true){
        platform_pump_message(state);
    }
    platform_shutdown(state);
    LOG_INFO("Application stopped");
    return 0;
}
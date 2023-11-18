#pragma once

#include "../defines.h"
#include <stdbool.h>
#include <stdint.h>


typedef struct platform_state{
    void *internal_state;
} platform_state;

bool platform_startup(platform_state *state, const char *app_name, uint32_t x, uint32_t y, uint32_t width, uint32_t height);
void platform_shutdown(platform_state *state);

bool platform_pump_message(platform_state *state);

void* platform_allocate(uint64_t size, bool aligned);
void platform_free(void* block, bool aligned);
void* platform_zero_memory(void* block, uint64_t size);
void* platform_copy_memory(void* dest, const void* source, uint64_t size);
void* platform_set_memory(void* dest, int32_t value, uint64_t size);

void platform_console_write(const char* message, uint8_t colour);
void platform_console_write_error(const char* message, uint8_t colour);

float platform_get_absolute_time();

void platform_sleep(uint64_t ms);
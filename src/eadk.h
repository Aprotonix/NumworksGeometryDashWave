#ifndef EADK_H
#define EADK_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Types and constants

typedef uint16_t eadk_color_t;

static const eadk_color_t eadk_color_black = 0x0;
static const eadk_color_t eadk_color_white = 0xFFFF;
static const eadk_color_t eadk_color_red = 0xF800;
static const eadk_color_t eadk_color_green = 0x07E0;
static const eadk_color_t eadk_color_blue = 0x001F;

typedef struct
{
    uint16_t x;
    uint16_t y;
} eadk_point_t;

typedef struct
{
    uint16_t x;
    uint16_t y;
    uint16_t width;
    uint16_t height;
} eadk_rect_t;


// Display

void eadk_display_push_rect(eadk_rect_t rect, const eadk_color_t *pixels);
void eadk_display_push_rect_uniform(eadk_rect_t rect, eadk_color_t color);
void eadk_display_pull_rect(eadk_rect_t rect, eadk_color_t *pixels);
bool eadk_display_wait_for_vblank();
void eadk_display_draw_string(const char *text, eadk_point_t point, bool large_font, eadk_color_t text_color, eadk_color_t background_color);

// Timing

void eadk_timing_usleep(uint32_t us);
void eadk_timing_msleep(uint32_t ms);
uint64_t eadk_timing_millis();

// External data

extern const char *eadk_external_data;
extern size_t eadk_external_data_size;

// Misc

bool eadk_usb_is_plugged();
uint32_t eadk_random();

#endif

#ifndef VITA_SVG_H
#define VITA_SVG_H

/** VITA_SVG MODULE
    - 
*/

#include "../core/core.h"
#include "../container/str.h"

typedef struct VitaSVGCanvas {
    uint32_t width;
    uint32_t height;
    vt_str_t *surface;
} vt_svg_canvas_t;

extern vt_svg_canvas_t *vt_svg_canvas_create(const uint32_t width, const uint32_t height, struct VitaBaseAllocatorType *const alloctr);
extern void vt_svg_canvas_destroy(vt_svg_canvas_t *svg);

extern void vt_svg_canvas_save(const vt_svg_canvas_t *const svg, const char *const filename);
extern void vt_svg_canvas_undo(const vt_svg_canvas_t *const svg);
extern void vt_svg_canvas_clear(const vt_svg_canvas_t *const svg);

extern void vt_svg_canvas_draw_line(vt_svg_canvas_t *const svg, const int32_t x1, const int32_t y1, const int32_t x2, const int32_t y2, const uint32_t stroke_width, const char *const stroke_color);
extern void vt_svg_canvas_draw_circle(vt_svg_canvas_t *const svg, const int32_t x, const int32_t y, const uint32_t r, const uint32_t stroke_width, const char *const stroke_color, const char *const fill_color);
extern void vt_svg_canvas_draw_ellipse(vt_svg_canvas_t *const svg, const int32_t x, const int32_t y, const uint32_t rx, const uint32_t ry, const uint32_t stroke_width, const char *const stroke_color, const char *const fill_color);
extern void vt_svg_canvas_draw_rectangle(vt_svg_canvas_t *const svg, const int32_t x, const int32_t y, const uint32_t width, const uint32_t height, const uint32_t stroke_width, const uint32_t corner_radius, const char *const stroke_color, const char *const fill_color);
extern void vt_svg_canvas_draw_text(vt_svg_canvas_t *const svg, const char *const text, const int32_t x, const int32_t y, const char *const font_family, const uint32_t font_size, const char *const stroke_color, const char *const fill_color);
extern void vt_svg_canvas_fill(vt_svg_canvas_t *const svg, const char *const fill_color);

#endif // VITA_SVG_H



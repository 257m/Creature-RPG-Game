#ifndef UI_ELEMENTS_H
#define UI_ELEMENTS_H

#include "texture.h"

extern void draw_line(Tex_Rect* line, Tex_Color* color, float width);
extern void draw_filled_rect(Tex_Rect* rect, Tex_Color* color);
extern void draw_selection_box(Tex_Rect* rect);

#endif /* UI_ELEMENTS_H */

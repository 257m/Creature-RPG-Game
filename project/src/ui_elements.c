#include "texture.h"
#include <math.h>

void draw_line(Tex_Rect* line, Tex_Color* color, float width)
{
	f32 length = sqrt((line->x1-line->x2)*(line->x1-line->x2) + (line->y1-line->y2)*(line->y1-line->y2));
	f32 x = width*(line->y1-line->y2) / length;
	f32 y = -width*(line->x1-line->x2) / length;
	glBegin(GL_QUADS);
		glColor4ub(color->r, color->g, color->b, color->a);	glVertex2f(line->x1, line->y1);
		glColor4ub(color->r, color->g, color->b, color->a);	glVertex2f(line->x2, line->y2);
		glColor4ub(color->r, color->g, color->b, color->a);	glVertex2f(line->x2 + x, line->y2 + y);
		glColor4ub(color->r, color->g, color->b, color->a);	glVertex2f(line->x1 + x, line->y1 + y);
	glEnd();
}

void draw_filled_rect(Tex_Rect* rect, Tex_Color* color)
{
	glBegin(GL_QUADS);
		glColor4ub(color->r, color->g, color->b, color->a);	glVertex2f(rect->x, rect->y);
		glColor4ub(color->r, color->g, color->b, color->a);	glVertex2f(rect->x + rect->width, rect->y);
		glColor4ub(color->r, color->g, color->b, color->a);	glVertex2f(rect->x + rect->width, rect->y - rect->height);
		glColor4ub(color->r, color->g, color->b, color->a);	glVertex2f(rect->x, rect->y - rect->height);
	glEnd();
}

void draw_selection_box(Tex_Rect* rect)
{
	#define corner_size_x (5.0f/128.0f)
	#define corner_size_y (5.0f/96.0f)
	static Texture* Corners = NULL;
	if (!Corners)
		Corners = texture_generate("Images/Corners.png");
	Tex_Rect dest_rect = {0, 0, corner_size_x, corner_size_y};
	Tex_Rect source_rect = {0.0f, 0.0f, 0.5f, 0.5f};
	for (int i = 0; i < 4; i++) {
		source_rect.x = (i % 2) * 0.5f;
		source_rect.y = (i > 1) * 0.5f;
		dest_rect.x = rect->x + ((i % 2) * (rect->w - corner_size_x));
		dest_rect.y = rect->y - ((i > 1) * (rect->h - corner_size_y));
		texture_render(Corners, &source_rect, &dest_rect);
	}

	draw_filled_rect(&((Tex_Rect){rect->x + corner_size_x, rect->y, rect->w - corner_size_x*2, 1.0f/96.0f}), &((Tex_Color){144, 164, 174, 255}));
	Tex_Color color_save = {67, 87, 97, 255};
	draw_filled_rect(&((Tex_Rect){rect->x, rect->y - 2.0f/96.0f, 1.0f/128.0f, rect->h - 4.0f/96.0f}), &color_save);
	draw_filled_rect(&((Tex_Rect){rect->x + rect->w - 1.0f/128.0f, rect->y - 2.0f/96.0f, 1.0f/128.0f, rect->h - 4.0f/96.0f}), &color_save);
	draw_filled_rect(&((Tex_Rect){rect->x + corner_size_x, rect->y - rect->h + 1.0f/96.0f, rect->w - corner_size_x*2, 1.0f/96.0f}), &color_save);
	color_save = (Tex_Color){229, 229, 229, 255};
	draw_filled_rect(&((Tex_Rect){rect->x + corner_size_x, rect->y - 3.0f/96.0f, rect->w - corner_size_x*2, 1.0f/96.0f}), &color_save);
	draw_filled_rect(&((Tex_Rect){rect->x + corner_size_x, rect->y - rect->h + 4.0f/96.0f, rect->w - corner_size_x*2, 1.0f/96.0f}), &color_save);
	draw_filled_rect(&((Tex_Rect){rect->x + 3.0f/128.0f, rect->y - 5.0f/96.0f, 1.0f/128.0f, rect->h - corner_size_y*2}), &color_save);
	draw_filled_rect(&((Tex_Rect){rect->x + rect->w - 4.0f/128.0f, rect->y - 5.0f/96.0f, 1.0f/128.0f, rect->h - corner_size_y*2}), &color_save);

	color_save = (Tex_Color){96, 125, 139, 255};
	dest_rect.x = rect->x + 5.0f/128.0f;
	dest_rect.y = rect->y - 1.0f/96.0f;
	dest_rect.w = rect->w - 10.0f/128.0f;
	dest_rect.h = 2.0f/96.0f;
	draw_filled_rect(&dest_rect, &color_save);
	dest_rect.y = rect->y - rect->h + 3.0f/96.0f;
	draw_filled_rect(&dest_rect, &color_save);

	dest_rect.x = rect->x + 1.0f/128.0f;
	dest_rect.y = rect->y - 5.0f/96.0f;
	dest_rect.w = 2.0f/128.0f;
	dest_rect.h = rect->h - 10.0f/96.0f;
	draw_filled_rect(&dest_rect, &color_save);
	dest_rect.x = rect->x + rect->w - 3.0f/128.0f;
	draw_filled_rect(&dest_rect, &color_save);

	dest_rect.x = rect->x + 4.0f/128.0f;
	dest_rect.y = rect->y - 4.0f/96.0f;
	dest_rect.w = rect->w - 8.0f/128.0f;
	dest_rect.h = rect->h - 8.0f/96.0f;
	draw_filled_rect(&dest_rect, &((Tex_Color){255, 255, 255, 255}));
}
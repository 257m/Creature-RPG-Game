#ifndef TEXTURE_H
#define TEXTURE_H

#include <GLFW/glfw3.h>
#include "data_types.h"

typedef struct {
	GLuint id;
	i32 width, height;
	u8* data;
} Texture;

typedef struct {
	union {
		f32 x;
		f32 x1;
	};
	union {
		f32 y;
		f32 y1;
	};
	union {
		f32 w;
		f32 width;
		f32 x2;
	};
	union {
		f32 h;
		f32 height;
		f32 y2;
	};
} Tex_Rect;

typedef struct {
	u8 r, g, b, a;
} Tex_Color;

extern Texture* texture_generate(char* file_path);
extern void texture_free(Texture* tex);
extern void texture_render(Texture* tex, Tex_Rect* source, Tex_Rect* dest);

#endif /* TEXTURE_H */
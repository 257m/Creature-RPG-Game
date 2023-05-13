#include "texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

Texture* texture_generate(char* file_path)
{
	Texture* tex = malloc(sizeof(Texture));
	tex->data = stbi_load(file_path, &tex->width, &tex->height, &tex->id, 4);
	glGenTextures(1, &tex->id);
	glBindTexture(GL_TEXTURE_2D, tex->id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex->width, tex->height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, tex->data);
	return tex;
}

void texture_free(Texture* tex)
{
	free(tex->data);
	free(tex);
}

void texture_render(Texture* tex, Tex_Rect* source, Tex_Rect* dest)
{
	glBindTexture(GL_TEXTURE_2D, tex->id);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	if (source) {
		glBegin(GL_QUADS);
			glTexCoord2f(source->x + source->width, source->y);                 	glVertex2f(dest->x + dest->width, dest->y);
			glTexCoord2f(source->x + source->width, source->y + source->height);	glVertex2f(dest->x + dest->width, dest->y - dest->height);
			glTexCoord2f(source->x, source->y + source->height);                	glVertex2f(dest->x, dest->y - dest->height);
			glTexCoord2f(source->x, source->y);                                 	glVertex2f(dest->x, dest->y);
		glEnd();
		return;
	}
	glBegin(GL_QUADS);
		glTexCoord2f(1.0f, 0.0f);	glVertex2f(dest->x + dest->width, dest->y);
		glTexCoord2f(1.0f, 1.0f);	glVertex2f(dest->x + dest->width, dest->y - dest->height);
		glTexCoord2f(0.0f, 1.0f);	glVertex2f(dest->x, dest->y - dest->height);
		glTexCoord2f(0.0f, 0.0f);	glVertex2f(dest->x, dest->y);
	glEnd();
}
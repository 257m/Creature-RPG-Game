#include "state.h"
#include "texture.h"

void RetrieveUserMove(u8 party)
{
	Texture* background = texture_generate("Images/BattleBackground.png");
	Texture* textbox = texture_generate("Images/TextBox.png");
	while (1) {
		glClearColor(0, 0, 0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_TEXTURE_2D);
		Tex_Rect source_rect = {0.0f, 0.0f, 1.0f, 1.0f};
		Tex_Rect dest_rect = {-1.0f, 1.0f, 2.0f, 1.5f};
		texture_render(background, &source_rect, &dest_rect);
		dest_rect.x = -1.0f;
		dest_rect.y = -0.5f;
		dest_rect.width = 2.0f;
		dest_rect.height = 0.5f;
		texture_render(textbox, &source_rect, &dest_rect);
		glfwSwapBuffers(state->window);
	}
	return;
}
#include "state.h"
#include "texture.h"
#include "ui_elements.h"

void retrieve_user_move(u8 party)
{
	Texture* background = texture_generate("Images/BattleBackground.png");
	Texture* textbox = texture_generate("Images/TextBox.png");
	Texture* indicator = texture_generate("Images/Indicator.png");
	unsigned int cursor_position = 0;
	while (!glfwWindowShouldClose(state->window)) {
		glClearColor(0, 0, 0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_TEXTURE_2D);
		Event event;
		while (event_queue_poll(state->event_queue, &event)) {
			if (event.action == GLFW_PRESS) {
				switch (event.key) {
					case GLFW_KEY_ESCAPE:
						glfwSetWindowShouldClose(state->window, GL_TRUE);
						break;
					case GLFW_KEY_W:
					case GLFW_KEY_UP:
						cursor_position -= (cursor_position > 1)*2;
						break;
					case GLFW_KEY_S:
					case GLFW_KEY_DOWN:
						cursor_position += (cursor_position < 2)*2;
						break;
					case GLFW_KEY_D:
					case GLFW_KEY_RIGHT:
						cursor_position += !(cursor_position % 2);
						break;
					case GLFW_KEY_A:
					case GLFW_KEY_LEFT:
						cursor_position -= (cursor_position % 2);
						break;
					default:
						break;
					}
				}
		}
		Tex_Rect dest_rect = {-1.0f, 1.0f, 2.0f, 1.5f};
		texture_render(background, NULL, &dest_rect);
		dest_rect.x = -1.0f;
		dest_rect.y = -0.5f;
		dest_rect.width = 2.0f;
		dest_rect.height = 0.5f;
		texture_render(textbox, NULL, &dest_rect);
		dest_rect.x = 10.0f/128.0f;
		dest_rect.y = -50.0f/96.0f;
		dest_rect.w = 116.0f/128.0f;
		dest_rect.h = 44.0f/96.0f;
		draw_selection_box(&dest_rect);
		dest_rect.x = 16.0f/128.0f + ((cursor_position % 2) * 54.0f/128.0f);
		dest_rect.y = -58.0f/96.0f - ((cursor_position > 1) * 18.0f/96.0f);
		dest_rect.w = 8.0f/128.0f;
		dest_rect.h = 9.0f/96.0f;
		texture_render(indicator, NULL, &dest_rect);
		glfwSwapBuffers(state->window);
		glfwPollEvents();
	}
	texture_free(background);
	texture_free(textbox);
	return;
}
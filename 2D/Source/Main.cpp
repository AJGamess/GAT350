#include "Renderer.h"
#include "Framebuffer.h"
#include "MathUtils.h"
#include "Image.h"
#include "PostProcess.h"
#include "Model.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SDL.h>
#include <iostream>

int main(int argc, char* argv[])
{
	Renderer renderer;
	renderer.Initialize();
	renderer.CreateWindow("2D", 800, 600);

	Framebuffer framebuffer(renderer, 400, 300);

	// main loop
	bool quit = false;
	while (!quit)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				quit = true;
			}
			if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
			{
				quit = true;
			}
		}
		// clear screen
		//SDL_SetRenderDrawColor(renderer.m_renderer, 0, 0, 0, 0);
		//SDL_RenderClear(renderer.m_renderer);

		framebuffer.Clear(color_t{ 128,128,128,255 });

		Image image;
		image.Load("scenic.jpeg");


		Image alphaImage;
		alphaImage.Load("colors.png");
		SetBlendMode(BlendMode::Multiply);
		PostProcess::Alpha(alphaImage.m_buffer, 50);

		vertices_t vertices = { {-5,5,0}, {5,5,0},{-5,-5,0} };
		Model model(vertices, { 0,255,0,255 });

		for (int i = 0; i < 100; i++)
		{
			int x = rand() % framebuffer.m_width;
			int y = rand() % framebuffer.m_height;
			int x2 = rand() % framebuffer.m_width;
			int y2 = rand() % framebuffer.m_height;
			//framebuffer.DrawLine(x, y, x2, y2, { 255,255,255,255 });
			//framebuffer.DrawPoint(x, y, { 255,255,255,255 });
			//framebuffer.DrawRect(10, 10, 100, 100, { 0,255,0,255 });
		}

		int mx, my;
		SDL_GetMouseState(&mx, &my);

		framebuffer.DrawImage(1, 1, image);
		framebuffer.DrawImage(mx, my, alphaImage);
		//framebuffer.GenerateCircle(50, 50, 30, { 255,255,255,255 });
		//framebuffer.DrawLine(100, 100, 120, 120, { 255,255,255,255 });
		//framebuffer.DrawTriangle(150, 150, 200, 200, 100, 250, { 255,255,255,255 });

		//framebuffer.DrawLinearCurve(100, 100, 200, 200, { 255,0,255,255 });
		//framebuffer.DrawQuadraticCurve(100, 200, mx, my, 300, 200, { 255,0,0,255 });
		//framebuffer.DrawCubicCurve(100, 200, 100, 100, 200, 100, 200, 200, { 0,255,255,0 });

		int ticks = SDL_GetTicks();
		float time = ticks * 0.001f;
		float t = std::abs(std::sin(time));
		//int x, y;
		//CubicPoint(200, 300, 100, 50, mx, my, 500, 400, t, x, y);
		//framebuffer.DrawRect(x - 20, y - 20, 40, 40, { 255,0,0,255 });

#pragma region postprocess
		//PostProcess::Invert(framebuffer.m_buffer);
		//PostProcess::Monochrome(framebuffer.m_buffer);
		//PostProcess::Brightness(framebuffer.m_buffer, 200);
		//PostProcess::ColorBalance(framebuffer.m_buffer, 150, -50, -50);
		//PostProcess::Noise(framebuffer.m_buffer, 80);
		//PostProcess::Threshold(framebuffer.m_buffer, 150);
		//PostProcess::Posterize(framebuffer.m_buffer, 6);

		//PostProcess::BoxBlur(framebuffer.m_buffer, framebuffer.m_width, framebuffer.m_height);
		//PostProcess::GuassianBlur(framebuffer.m_buffer, framebuffer.m_width, framebuffer.m_height);
		//PostProcess::Sharpen(framebuffer.m_buffer, framebuffer.m_width, framebuffer.m_height);

		//PostProcess::Edge(framebuffer.m_buffer, framebuffer.m_width, framebuffer.m_height, 10);
		//PostProcess::Emboss(framebuffer.m_buffer, framebuffer.m_width, framebuffer.m_height);
#pragma endregion

		glm::mat4 modelMatrix = glm::mat4(1.0f);
		glm::mat4 translate = glm::translate(modelMatrix, glm::vec3(40.0f, 40.0f, 0.0f));
		glm::mat4 scale = glm::scale(modelMatrix, glm::vec3(5));
		glm::mat4 rotate = glm::rotate(modelMatrix, glm::radians(time * 1000), glm::vec3{ 1,1,1 });

		modelMatrix = translate * scale * rotate;

		model.Draw(framebuffer, modelMatrix);

		framebuffer.Update();
		renderer = framebuffer;

		//framebuffer.Clear(color_t(255,0,0,255))
		// show screen
		SDL_RenderPresent(renderer.m_renderer);

	}


	return 0;
}
#include "Renderer.h"
#include "Framebuffer.h"
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

		framebuffer.Clear(color_t{ 0,0,0,255 });

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
		framebuffer.GenerateCircle(50, 50, 30, { 255,255,255,255 });
		framebuffer.DrawLine(100, 100, 120, 120, { 255,255,255,255 });
		framebuffer.DrawTriangle(150, 150, 200, 200, 100, 250, { 255,255,255,255 });
	
		framebuffer.Update();
		renderer = framebuffer;

		//framebuffer.Clear(color_t(255,0,0,255))
		// show screen
		SDL_RenderPresent(renderer.m_renderer);

	}


	return 0;
}
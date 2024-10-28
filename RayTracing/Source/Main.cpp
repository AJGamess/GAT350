#include "Renderer.h"
#include "Framebuffer.h"
#include "MathUtils.h"
#include "Image.h"
#include "PostProcess.h"
#include "Model.h"
#include "Transform.h"
#include "ETime.h"
#include "Input.h"
#include "Camera.h"
#include "Actor.h"
#include "Random.h"
#include "Tracer.h"
#include "Scene.h"
#include "Plane.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SDL.h>
#include <iostream>
#include <memory>


int main(int argc, char* argv[])
{
	//initailze
	Time time;

	Tracer tracer;

	Renderer renderer;
	renderer.Initialize();
	renderer.CreateWindow("RayTracing", 800, 600);

	SetBlendMode(BlendMode::Normal);

	Framebuffer framebuffer(renderer, 800, 600);

	Camera camera{ 70.0f, framebuffer.m_width / (float)framebuffer.m_height };
	camera.SetView({ 0,0,-20 }, { 0,0,0 });

	Scene scene;

	std::shared_ptr<Material> green = std::make_shared<Material>(color3_t{ 0,1,0 });
	std::shared_ptr<Material> gray = std::make_shared<Material>(color3_t{ 0.5f });
	std::shared_ptr<Material> red = std::make_shared<Material>(color3_t{ 1, 0, 0 });
	std::shared_ptr<Material> blue = std::make_shared<Material>(color3_t{ 0, 0, 1 });

	std::vector<std::shared_ptr<Material>> materials;

	materials.push_back(green);
	//materials.push_back(gray);
	materials.push_back(red);
	materials.push_back(blue);


	/*
	auto sphere = std::make_unique<Sphere>(glm::vec3{ 0,0,-40 }, 2.0f, blue);
	scene.AddObject(std::move(sphere));
	*/
	/*
	auto plane = std::make_unique<Plane>(glm::vec3{ 0, 1, 0 }, glm::vec3{ 0.4f, 0.4f, 0 }, green);
	scene.AddObject(std::move(plane));
	*/

	for (int i = 0; i < 20; i++) {
		
		std::shared_ptr<Material> random_material = materials[random(0, materials.size())];

		//for debug purposes
		if (random_material) {
			color3_t color = random_material->GetColor();
			std::cout << "Selected Material Color: (" << color.r << ", " << color.g << ", " << color.b << ")\n";
		}
		else {
			std::cout << "random_material is null.\n";
		}

		float random_radius = randomf(0.0f, 2.0f);

		auto object = std::make_unique<Sphere>(random(glm::vec3{ -10 }, glm::vec3{ 10 }), random_radius, random_material);
		scene.AddObject(std::move(object));
	}
	auto plane = std::make_unique<Plane>(glm::vec3{ 0, -5, 0 }, glm::vec3{ 0, 1, 0 }, gray);
	scene.AddObject(std::move(plane));

	// main loop
	bool quit = false;
	while (!quit)
	{
		time.Tick();

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

		framebuffer.Clear(ColorConvert(color4_t{ 0,1,0,1 }));

		scene.Render(framebuffer, camera);

		framebuffer.Update();
		renderer = framebuffer;

		//framebuffer.Clear(color_t(255,0,0,255))
		// show screen
		SDL_RenderPresent(renderer.m_renderer);

	}


	return 0;
}
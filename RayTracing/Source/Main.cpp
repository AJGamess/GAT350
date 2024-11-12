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
#include "Triangle.h"
#include "Material.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SDL.h>
#include <iostream>
#include <memory>

void CornellBox(Scene& scene) {
	//Cube object material works with Emissive so this will do as light source
	std::shared_ptr<Material> lightsource = std::make_shared<Emissive>(color3_t{ 1,1,1 }, 3.0f);
	
	//Use for spheres
	std::shared_ptr<Material> dielectricwhite = std::make_shared<Dielectric>(color3_t{ 1, 1, 1 }, 1.5f);
	std::shared_ptr<Material> metalblue = std::make_shared<Metal>(color3_t{ 0, 0, 1 }, 0.1f);
	std::shared_ptr<Material> yellow = std::make_shared<Lambertian>(color3_t{ 1, 1, 0 });

	//Use for the planes
	std::shared_ptr<Material> green = std::make_shared<Lambertian>(color3_t{ 0,1,0 });
	std::shared_ptr<Material> white = std::make_shared<Lambertian>(color3_t{ 1,1,1 });
	std::shared_ptr<Material> red = std::make_shared<Lambertian>(color3_t{ 1, 0, 0 });

	std::vector<std::shared_ptr<Material>> sphere_materials;

	sphere_materials.push_back(dielectricwhite);
	sphere_materials.push_back(metalblue);
	sphere_materials.push_back(yellow);

	auto backwall = std::make_unique<Plane>(Transform{ glm::vec3{ 20, 0, 60 }, glm::vec3{ 0, -90, 90 } }, white);
	scene.AddObject(std::move(backwall));
	
	auto floor = std::make_unique<Plane>(Transform{ glm::vec3{ 0, -20, 0 }, glm::vec3{ 0, 0, 0 } }, white);
	scene.AddObject(std::move(floor));
	
	auto ceiling = std::make_unique<Plane>(Transform{ glm::vec3{ 0, 20, 0 }, glm::vec3{ 0, 0, 0 } }, white);
	scene.AddObject(std::move(ceiling));
	
	auto rightwall = std::make_unique<Plane>(Transform{ glm::vec3{ 20, 0, 0 }, glm::vec3{ 0, 0, 90 } }, green);
	scene.AddObject(std::move(rightwall));
	
	auto leftwall = std::make_unique<Plane>(Transform{ glm::vec3{ -20, 0, 0 }, glm::vec3{ 0, 0, -90 } }, red);
	scene.AddObject(std::move(leftwall));

	auto model = std::make_unique<Model>(Transform{ glm::vec3{ 0,25,30 }, glm::vec3{ 0, 0, 0 },	glm::vec3{ 9 } }, lightsource);
	model->Load("Model/cube.obj");
	scene.AddObject(std::move(model));

	for (int i = 0; i < 3; i++) {

		std::shared_ptr<Material> sphere = sphere_materials[i];

		float random_radius = randomf(1.0f, 2.0f);

		auto object = std::make_unique<Sphere>(Transform{ glm::vec3{ random(glm::vec3{ -5.0f }, glm::vec3{ 4.0f })} }, randomf(1.0f, 3.0f), sphere);
		scene.AddObject(std::move(object));
	}
}

void InItScene(Scene& scene) {
	//scene.SetSky(HSVtoRGB(240, 1, 0.2f), HSVtoRGB(240, 1, 1));

	std::shared_ptr<Material> green = std::make_shared<Lambertian>(color3_t{ 0,1,0 });
	std::shared_ptr<Material> purple = std::make_shared<Emissive>(color3_t{ 1,0,1 }, 3.0f);
	std::shared_ptr<Material> white = std::make_shared<Lambertian>(color3_t{ 1,1,1 });
	std::shared_ptr<Material> lightsource = std::make_shared<Emissive>(color3_t{ 1,1,1 }, 3.0f);
	std::shared_ptr<Material> gray = std::make_shared<Lambertian>(color3_t{ 0.5f });
	std::shared_ptr<Material> red = std::make_shared<Lambertian>(color3_t{ 1, 0, 0 });
	std::shared_ptr<Material> metalred = std::make_shared<Metal>(color3_t{ 1, 0, 0 }, 0.3f);
	std::shared_ptr<Material> blue = std::make_shared<Lambertian>(color3_t{ 0, 0, 1 });
	std::shared_ptr<Material> metalblue = std::make_shared<Metal>(color3_t{ 0, 0, 1 }, 0.1f);
	std::shared_ptr<Material> dielectricwhite = std::make_shared<Dielectric>(color3_t{ 1, 1, 1 }, 1.5f);

	std::vector<std::shared_ptr<Material>> materials;

	materials.push_back(green);
	//materials.push_back(gray);
	//materials.push_back(red);
	materials.push_back(metalred);
	//materials.push_back(blue);
	materials.push_back(metalblue);
	materials.push_back(dielectricwhite);
	materials.push_back(purple);

	auto model = std::make_unique<Model>(Transform{ glm::vec3{ 0 }, glm::vec3{ 0, 40, 0 },	glm::vec3{ 3 } }, lightsource);
	model->Load("Model/cube.obj");
	scene.AddObject(std::move(model));

	/*
	auto sphere = std::make_unique<Sphere>(glm::vec3{ 0,0,-40 }, 2.0f, blue);
	scene.AddObject(std::move(sphere));
	*/
	/*
	auto plane = std::make_unique<Plane>(glm::vec3{ 0, 1, 0 }, glm::vec3{ 0.4f, 0.4f, 0 }, green);
	scene.AddObject(std::move(plane));
	*/


	auto plane = std::make_unique<Plane>(Transform{ glm::vec3{ 0, -2, 0 }, glm::vec3{ 0, 0, 0 } }, gray);
	scene.AddObject(std::move(plane));



	for (int i = 0; i < 3; i++) {

		std::shared_ptr<Material> random_material = materials[random(0, materials.size())];

		//for debug purposes
		if (random_material) {
			color3_t color = random_material->GetColor();
			std::cout << "Selected Material Color: (" << color.r << ", " << color.g << ", " << color.b << ")\n";
		}
		else {
			std::cout << "random_material is null.\n";
		}

		float random_radius = randomf(1.0f, 2.0f);

		auto object = std::make_unique<Sphere>(Transform{ glm::vec3{ random(glm::vec3{ -10.0f }, glm::vec3{ 10.0f })} }, randomf(1.0f, 3.0f), materials[random(5)]);
		scene.AddObject(std::move(object));
	}

	//auto triangle = std::make_unique<Triangle>(glm::vec3{ -3, 0, 0 }, glm::vec3{ 0, 3, 0 }, glm::vec3{ 3, 0, 0 }, green);
	//scene.AddObject(std::move(triangle));

}

void InitSceneFinal(Scene& scene, Camera& camera)
{
	camera.SetFOV(40.0f);
	camera.SetView({ 15, 5, 4 }, { 0, 0, 0 });

	scene.SetSky(HSVtoRGB(40, 1, 1), HSVtoRGB(0, 0, 1));

	auto ground_material = std::make_shared<Lambertian>(color3_t(0.5f));
	scene.AddObject(std::make_unique<Plane>(Transform{ glm::vec3{ 0 } }, ground_material));

	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			auto choose_mat = randomf();
			glm::vec3 center(a + 0.9 * randomf(), 0.2, b + 0.9 * randomf());

			if ((center - glm::vec3(4, 0.2, 0)).length() > 0.9) {
				std::shared_ptr<Material> sphere_material;

				if (choose_mat < 0.8) {
					// diffuse
					auto albedo = HSVtoRGB(randomf(0, 360), 1.0f, 1.0f);
					sphere_material = std::make_shared<Lambertian>(albedo);
					scene.AddObject(std::make_unique<Sphere>(Transform{ center }, 0.2f, sphere_material));
				}
				else if (choose_mat < 0.95) {
					// metal
					auto albedo = HSVtoRGB(randomf(0, 360), 1.0f, 1.0f);
					auto fuzz = randomf(0.0f, 0.5f);
					sphere_material = std::make_shared<Metal>(albedo, fuzz);
					scene.AddObject(std::make_unique<Sphere>(Transform{ center }, 0.2f, sphere_material));
				}
				else {
					// glass
					sphere_material = std::make_shared<Dielectric>(color3_t{ 1 }, 1.5f);
					scene.AddObject(std::make_unique<Sphere>(Transform{ center }, 0.2f, sphere_material));
				}
			}
		}
	}

	auto material1 = std::make_shared<Dielectric>(color3_t{ 1 }, 1.5f);
	scene.AddObject(std::make_unique<Sphere>(Transform{ glm::vec3{ 0, 1, 4 } }, 1.5f, material1));

	auto material2 = std::make_shared<Lambertian>(color3_t(0.47f, 0.2f, 0.5f));
	scene.AddObject(std::make_unique<Sphere>(Transform{ glm::vec3{ -7, 1, 0 } }, 0.7f, material2));

	auto material3 = std::make_shared<Metal>(color3_t(0.2f, 0.1f, 0.8f), 0.0f);
	scene.AddObject(std::make_unique<Sphere>(Transform{ glm::vec3{ 4, 1, 0 } }, 1.0f, material3));
}

int main(int argc, char* argv[])
{
	srand((unsigned int)time(NULL));
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
	//InItScene(scene);
	InitSceneFinal(scene, camera);
	//CornellBox(scene);

	scene.Update();
	scene.Render(framebuffer, camera, 80, 6);
	framebuffer.Update();

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

		//framebuffer.Clear(ColorConvert(color4_t{ 0,1,0,1 }));

		renderer = framebuffer;

		//framebuffer.Clear(color_t(255,0,0,255))
		// show screen
		SDL_RenderPresent(renderer.m_renderer);

	}


	return 0;
}


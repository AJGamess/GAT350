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

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SDL.h>
#include <iostream>
#include <memory>


int main(int argc, char* argv[])
{
	//initailze
	Time time;
	Input input;
	input.Initialize();

	Renderer renderer;
	renderer.Initialize();
	renderer.CreateWindow("2D", 800, 600);

	SetBlendMode(BlendMode::Normal);

	Camera camera(renderer.m_width, renderer.m_height);
	camera.SetView(glm::vec3{ 0,0,-50 }, glm::vec3{ 0 });
	camera.SetProjection(120.0f, 800.0f / 600, 0.1f, 200.0f);
	Transform cameraTransform = glm::vec3{ 0,0,-20 };

	Framebuffer framebuffer(renderer, 800, 600);

	//vertices_t vertices = { {-5,5,0}, {5,5,0},{-5,-5,0} };
	//Model model(vertices, { 0,255,0,255 });

	//Making a shared pointer for the models
	std::shared_ptr<Model> torus = std::make_shared<Model>();
	std::shared_ptr<Model> teapot = std::make_shared<Model>();
	std::shared_ptr<Model> sphere = std::make_shared<Model>();
	
	//Loading Models
	torus->Load("torus.obj");
	teapot->Load("teapot.obj");
	sphere->Load("sphere.obj");

	//Setting colors
	torus->SetColor({255, 0, 0, 255});
	teapot->SetColor({ 0, 255, 255, 255 });
	sphere->SetColor({ 0, 0, 255, 255 });

	//Initial positions of the models
	Transform transform{ {0,0,0},glm::vec3{0,0,45},glm::vec3{3} };
	Transform transform2{ {0,0,0},glm::vec3{0,0,45},glm::vec3{3} };
	Transform transform3{ {0,0,0},glm::vec3{0,0,45},glm::vec3{3} };

	//Setting actors
	Actor actor(transform, torus);
	Actor actor2(transform2, teapot);
	Actor actor3(transform3, sphere);

	//Main Actor for all models
	std::vector<std::unique_ptr<Actor>> actors;

	//Adding multiple models
	for (int i = 0; i < 3; i++) {
		//Spreading out the models in different positions
		Transform torusTransform{ {randomf(-40.0f, 40.0f), randomf(-40.0f, 40.0f), randomf(-40.0f, 40.0f)} , glm::vec3{0,0,0}, glm::vec3{3} };
		Transform teapotTransform{ {randomf(-60.0f, 60.0f), randomf(-60.0f, 60.0f), randomf(-60.0f, 60.0f)} , glm::vec3{0,0,0}, glm::vec3{3} };
		Transform sphereTransform{ {randomf(-40.0f, 40.0f), randomf(-40.0f, 40.0f), randomf(-40.0f, 40.0f)} , glm::vec3{0,0,0}, glm::vec3{3} };
		
		//setting the actors with their new positions
		std::unique_ptr<Actor> actor = std::make_unique<Actor>(torusTransform, torus);
		std::unique_ptr<Actor> actor2 = std::make_unique<Actor>(teapotTransform, teapot);
		std::unique_ptr<Actor> actor3 = std::make_unique<Actor>(sphereTransform, sphere);

		//adding the model actors to put into the main actor
		actors.push_back(move(actor));
		actors.push_back(move(actor2));
		actors.push_back(move(actor3));
	}

	//Setting the color blend mode
	SetBlendMode(BlendMode::Normal);

	// main loop
	bool quit = false;
	while (!quit)
	{
		time.Tick();
		input.Update();

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
		
		//Loading background
		Image image;
		image.Load("galaxy.jpg");


		//Image alphaImage;
		//alphaImage.Load("colors.png");
		//PostProcess::Alpha(alphaImage.m_buffer, 50);
		/*
		for (int i = 0; i < 1; i++)
		{
			int x = rand() % framebuffer.m_width;
			int y = rand() % framebuffer.m_height;
			int x2 = rand() % framebuffer.m_width;
			int y2 = rand() % framebuffer.m_height;
			//framebuffer.DrawLine(x, y, x2, y2, { 255,255,255,255 });
			//framebuffer.DrawPoint(x, y, { 255,255,255,255 });
			//framebuffer.DrawRect(10, 10, 100, 100, { 0,255,0,255 });
		}
		*/

		//Detecting Mouse
		int mx, my;
		SDL_GetMouseState(&mx, &my);

		//Drawing image to the screen
		framebuffer.DrawImage(1, 1, image);
		
		//framebuffer.DrawImage(1, 1, alphaImage);
		//framebuffer.GenerateCircle(50, 50, 30, { 255,255,255,255 });
		//framebuffer.DrawLine(100, 100, 120, 120, { 255,255,255,255 });
		//framebuffer.DrawTriangle(150, 150, 200, 200, 100, 250, { 255,255,255,255 });

		//framebuffer.DrawLinearCurve(100, 100, 200, 200, { 255,0,255,255 });
		//framebuffer.DrawQuadraticCurve(100, 200, mx, my, 300, 200, { 255,0,0,255 });
		//framebuffer.DrawCubicCurve(100, 200, 100, 100, 200, 100, 200, 200, { 0,255,255,0 });


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
#pragma region model
		//Setting up camera
		if (input.GetMouseButtonDown(2))
		{
			input.SetRelativeMode(true);
			//Camera Controls
			glm::vec3 direction{ 0 };
			if (input.GetKeyDown(SDL_SCANCODE_RIGHT)) direction.x = 1;
			if (input.GetKeyDown(SDL_SCANCODE_LEFT)) direction.x = -1;
			if (input.GetKeyDown(SDL_SCANCODE_UP)) direction.y = 1;
			if (input.GetKeyDown(SDL_SCANCODE_DOWN)) direction.y = -1;
			if (input.GetKeyDown(SDL_SCANCODE_K)) direction.z = 1;
			if (input.GetKeyDown(SDL_SCANCODE_L)) direction.z = -1;

			//changing positions and rotation of camera
			cameraTransform.rotation.x = input.GetMousePosition().x * 0.5f;
			cameraTransform.rotation.y = input.GetMousePosition().y * 0.5f;
			glm::vec3 offset = cameraTransform.GetMatrix() * glm::vec4{ direction, 0 };
			cameraTransform.position += offset * 70.0f * time.GetDeltaTime();
		}
		else {
			input.SetRelativeMode(false);
		}
		camera.SetView(cameraTransform.position, cameraTransform.position + cameraTransform.GetForward());

		//Drawing the models to the screen
		for (auto& actor : actors) 
		{
			actor->Draw(framebuffer , camera);
		}

#pragma endregion

		//updating screen
		framebuffer.Update();
		renderer = framebuffer;

		//framebuffer.Clear(color_t(255,0,0,255))
		// show screen
		SDL_RenderPresent(renderer.m_renderer);

	}


	return 0;
}
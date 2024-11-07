#include "Scene.h"
#include "Framebuffer.h"
#include "Camera.h"
#include "Tracer.h"
#include "Random.h"
#include "ETime.h"
#include <iostream>

void Scene::Update()
{
	for (auto& object : m_objects)
	{
		object->Update();
	}
}

void Scene::Render(class Framebuffer& framebuffer, const class Camera& camera, int numSamples, int depth)
{
	Time frameTimer;
	Time scanlineTimer;

	for (int y = 0; y < framebuffer.m_height; y++)
	{
		scanlineTimer.Reset();
		for (int x = 0; x < framebuffer.m_width; x++)
		{
			color3_t color{ 0 };

			for (int i = 0; i < numSamples; i++)
			{
				glm::vec2 pixel{ x,y };
				pixel += glm::vec2{ random(0,1),random(0,1) };
				glm::vec2 point = pixel / glm::vec2{ framebuffer.m_width, framebuffer.m_height };

				point.y = 1 - point.y;

				ray_t ray = camera.GetRay(point);

				color += Tracer::Trace(*this, ray, 0.001f, 100.0f, depth);

			}
			color /= static_cast<float>(numSamples);
			
			framebuffer.DrawPoint(x, y, ColorConvert(color));
			
		}
			std::cout << "y: " << y << " - " << scanlineTimer.GetElaspedTime() << std::endl;
	}
	std::cout << "Frame Time: " << frameTimer.GetElaspedTime() << std::endl;
}

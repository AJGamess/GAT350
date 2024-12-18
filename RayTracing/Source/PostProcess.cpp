#include "PostProcess.h"
#include "MathUtils.h"
#include <algorithm>
#include <cmath>

namespace PostProcess
{
	void Invert(std::vector<color_t>& buffer)
	{
		for (auto& color : buffer)
		{
			color.r = 255 - color.r;
			color.g = 255 - color.g;
			color.b = 255 - color.b;
		}
		/*
		std::for_each(buffer.begin(), buffer.end(), [](auto& c)
			{
				c.r = 255 - c.r;
				c.g = 255 - c.g;
				c.b = 255 - c.b;
			});
		*/
	}

	void Monochrome(std::vector<color_t>& buffer)
	{
		std::for_each(buffer.begin(), buffer.end(), [](auto& c)
			{
				uint8_t average = static_cast<uint8_t>((c.r + c.g + c.b) / 3);

				c.r = average;
				c.g = average;
				c.b = average;
			});
	}

	void Brightness(std::vector<color_t>& buffer, int brightness)
	{
		std::for_each(buffer.begin(), buffer.end(), [brightness](auto& c)
			{
				c.r = static_cast<uint8_t>(Clamp(c.r + brightness, 0, 255));
				c.g = static_cast<uint8_t>(Clamp(c.g + brightness, 0, 255));
				c.b = static_cast<uint8_t>(Clamp(c.b + brightness, 0, 255));
			});
	}

	void ColorBalance(std::vector<color_t>& buffer, int ro, int go, int bo)
	{
		std::for_each(buffer.begin(), buffer.end(), [ro, go, bo](auto& c)
			{
				c.r = ro + c.r;
				c.g = go + c.g;
				c.b = bo + c.b;
			});
	}

	void Noise(std::vector<color_t>& buffer, uint8_t noise)
	{
		std::for_each(buffer.begin(), buffer.end(), [noise](auto& c)
			{
				int offset = (rand() % ((noise * 2) + 1)) - noise;

				c.r = static_cast<uint8_t>(Clamp(c.r + offset, 0, 255));
				c.g = static_cast<uint8_t>(Clamp(c.g + offset, 0, 255));
				c.b = static_cast<uint8_t>(Clamp(c.b + offset, 0, 255));
			});
	}

	void Threshold(std::vector<color_t>& buffer, uint8_t threshold)
	{
		std::for_each(buffer.begin(), buffer.end(), [threshold](auto& c)
			{

				uint8_t average = static_cast<uint8_t>((c.r + c.g + c.b) / 3);

				if (average < threshold)
				{
					c.r = 0;
					c.g = 0;
					c.b = 0;
				}
				else {
					c.r = 255;
					c.g = 255;
					c.b = 255;
				}
			});
	}

	void Posterize(std::vector<color_t>& buffer, uint8_t levels)
	{
		uint8_t level = 255 / levels;

		std::for_each(buffer.begin(), buffer.end(), [levels, level](auto& c)
			{
			
				c.r = (c.r / level) * level;
				c.g = (c.g / level) * level;
				c.b = (c.b / level) * level;
			});
	}

	void Alpha(std::vector<color_t>& buffer, uint8_t alpha)
	{
		std::for_each(buffer.begin(), buffer.end(), [alpha](auto& c)
			{
				c.a = alpha;
			});
	}


	void BoxBlur(std::vector<color_t>& buffer, int width, int height)
	{
		std::vector<color_t> source = buffer;

		int k[3][3] =
		{
			{1,1,1},
			{1,1,1},
			{1,1,1}
		};

		for (int i = 0; i < buffer.size(); i++) {
			// & 5 : 1 2 3 4 5 6 7 8 9 10
			//		 1 2 3 4 0 1 2 3 4 0
			int x = i % width;
			int y = i / width;

			if (x < 1 || x + 1 >= width || y < 1 || y + 1 >= height) continue;

			int r = 0;
			int g = 0;
			int b = 0;

			for (int iy = 0; iy < 3; iy++)
			{
				for (int ix = 0; ix < 3; ix++)
				{
					color_t pixel = source[(x + ix - 1) + (y + iy - 1) * width];
					int weight = k[iy][ix];

					r += pixel.r * weight;
					g += pixel.g * weight;
					b += pixel.b * weight;
				}
			}
			color_t& color = buffer[i];
			color.r = static_cast<uint8_t>(r / 9);
			color.g = static_cast<uint8_t>(g / 9);
			color.b = static_cast<uint8_t>(b / 9);
		}
	}
	void GuassianBlur(std::vector<color_t>& buffer, int width, int height)
	{
		std::vector<color_t> source = buffer;

		int k[3][3] =
		{
			{1,2,1},
			{2,4,2},
			{1,2,1}
		};

		for (int i = 0; i < buffer.size(); i++) {
			// & 5 : 1 2 3 4 5 6 7 8 9 10
			//		 1 2 3 4 0 1 2 3 4 0
			int x = i % width;
			int y = i / width;

			if (x < 1 || x + 1 >= width || y < 1 || y + 1 >= height) continue;

			int r = 0;
			int g = 0;
			int b = 0;

			for (int iy = 0; iy < 3; iy++)
			{
				for (int ix = 0; ix < 3; ix++)
				{
					color_t pixel = source[(x + ix - 1) + (y + iy - 1) * width];
					int weight = k[iy][ix];

					r += pixel.r * weight;
					g += pixel.g * weight;
					b += pixel.b * weight;
				}
			}
			color_t& color = buffer[i];
			color.r = static_cast<uint8_t>(r / 16);
			color.g = static_cast<uint8_t>(g / 16);
			color.b = static_cast<uint8_t>(b / 16);
		}
	}
	void Sharpen(std::vector<color_t>& buffer, int width, int height)
	{
		std::vector<color_t> source = buffer;

		int k[3][3] =
		{
			{0,-1,0},
			{-1,5,-1},
			{0,-1,0}
		};

		for (int i = 0; i < buffer.size(); i++) {
			// & 5 : 1 2 3 4 5 6 7 8 9 10
			//		 1 2 3 4 0 1 2 3 4 0
			int x = i % width;
			int y = i / width;

			if (x < 1 || x + 1 >= width || y < 1 || y + 1 >= height) continue;

			int r = 0;
			int g = 0;
			int b = 0;

			for (int iy = 0; iy < 3; iy++)
			{
				for (int ix = 0; ix < 3; ix++)
				{
					color_t pixel = source[(x + ix - 1) + (y + iy - 1) * width];
					int weight = k[iy][ix];

					r += pixel.r * weight;
					g += pixel.g * weight;
					b += pixel.b * weight;
				}
			}
			color_t& color = buffer[i];
			color.r = static_cast<uint8_t>(Clamp(r, 0, 255));
			color.g = static_cast<uint8_t>(Clamp(g, 0, 255));
			color.b = static_cast<uint8_t>(Clamp(b, 0, 255));
		}
	}
	void Edge(std::vector<color_t>& buffer, int width, int height, int threshold)
	{
		std::vector<color_t> source = buffer;

		int hk[3][3] =
		{
			{1,0,-1},
			{2,0,-2},
			{1,0,-1}
		};

		int vk[3][3] =
		{
			{-1,-2,-1},
			{0,0,0},
			{1,2,1}
		};

		for (int i = 0; i < buffer.size(); i++) {
			// & 5 : 1 2 3 4 5 6 7 8 9 10
			//		 1 2 3 4 0 1 2 3 4 0
			int x = i % width;
			int y = i / width;

			if (x < 1 || x + 1 >= width || y < 1 || y + 1 >= height) continue;

			int h = 0;
			int v = 0;

			for (int iy = 0; iy < 3; iy++)
			{
				for (int ix = 0; ix < 3; ix++)
				{
					color_t pixel = source[(x + ix - 1) + (y + iy - 1) * width];

					h += pixel.r * hk[iy][ix];
					v += pixel.r * vk[iy][ix];
				}
			}

			int m = static_cast<int>(std::sqrt(h * h + v * v));
			m = (m >= threshold) ? m : 0;

			uint8_t c = std::clamp(m, 0, 255);

			color_t& color = buffer[i];
			color.r = c;
			color.g = c;
			color.b = c;
			/*
			color.r = static_cast<uint8_t>(Clamp(r, 0, 255));
			color.g = static_cast<uint8_t>(Clamp(g, 0, 255));
			color.b = static_cast<uint8_t>(Clamp(b, 0, 255));
			*/
		}
	}
	void Emboss(std::vector<color_t>& buffer, int width, int height)
	{
		std::vector<color_t> source = buffer;

		// Emboss kernel (3x3)
		int k[3][3] =
		{
			{ -2, -1, 0 },
			{ -1,  1, 1 },
			{  0,  1, 2 }
		};

		for (int i = 0; i < buffer.size(); i++) {
			int x = i % width;
			int y = i / width;

			// Ignore the edges of the image
			if (x < 1 || x + 1 >= width || y < 1 || y + 1 >= height) continue;

			int r = 0;
			int g = 0;
			int b = 0;

			// Apply the 3x3 convolution with the emboss kernel
			for (int iy = 0; iy < 3; iy++) {
				for (int ix = 0; ix < 3; ix++) {
					// Get the current neighboring pixel
					color_t pixel = source[(x + ix - 1) + (y + iy - 1) * width];

					// Get the corresponding kernel weight
					int weight = k[iy][ix];

					// Multiply the pixel values by the kernel weight
					r += pixel.r * weight;
					g += pixel.g * weight;
					b += pixel.b * weight;
				}
			}

			// Normalize by adding 128 to shift to the middle-gray range
			r = r + 128;
			g = g + 128;
			b = b + 128;

			// Clamp the result to be within valid RGB range [0, 255]
			color_t& color = buffer[i];
			color.r = static_cast<uint8_t>(Clamp(r, 0, 255));
			color.g = static_cast<uint8_t>(Clamp(g, 0, 255));
			color.b = static_cast<uint8_t>(Clamp(b, 0, 255));
		}
	}

}

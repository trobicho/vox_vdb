#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <iostream>

int	main(int ac, char **av)
{
	uint32_t	pixels[1920 * 1080];
	for (int y = 0; y < 1080 / 16; ++y)
	{
		for (int x = 0; x < 1920 / 16; ++x)
		{
			for (int ry = 0; ry < 16; ++ry)
			{
				for (int rx = 0; rx < 16; ++rx)
				{
					int r = (((x % 4) == 0)? 1 : 0) * (((y + 1) * (255 / 16)) % 256);
					int g = (((x % 4) == 1)? 1 : 0) * (((y + 1) * (255 / 16)) % 256);
					int b = (((x % 4) == 2)? 1 : 0) * (((y + 1) * (255 / 16)) % 256);
					int a = (((x % 4) == 3)? 1 : 0) * (((y + 1) * (255 / 16)) % 256);
					if (a != 0) {
						r = 255;
						g = 255;
						b = 255;
					}
					a = 255 - a;
					std::cout << "[" << r << ", " << g << ", " << b << ", " << a << "]" << std::endl;
					uint32_t	color = (uint32_t)(
						(a & 255) * (1 << 24)
						+ (b & 255) * (1 << 16)
						+ (g & 255) * (1 << 8)
						+ r);
					pixels[x * 16 + rx + (y * 16 + ry) * 1920] = color;
				}
			}
		}
	}
	stbi_write_png("./test.png", 1920, 1080, 4, (unsigned char*)pixels, 1920 * 4);
	return (EXIT_SUCCESS);
}

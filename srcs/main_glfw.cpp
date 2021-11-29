/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_glfw.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trobicho <trobicho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/15 06:25:54 by trobicho          #+#    #+#             */
/*   Updated: 2021/11/29 23:08:23 by trobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "backend/map_manager/Map_manager.hpp"
#include "frontend/map_generator/Basic_terrain.hpp"
#include "GLFW/glfw3.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

static void save_img(Map_manager &map_manager)
{
	std::cout << "print image" << std::endl;
	uint32_t* pixels;
	unsigned char* pixels_diff;
	std::lock_guard<std::mutex> guard(map_manager.get_screen(&pixels));
	for (int xy = 0; xy < 1280 * 720; ++xy)
	{
		if (pixels[xy] == 0)
			pixels[xy] |= (uint32_t)(255 << 24);
	}
	stbi_write_png("./chunk_map.png", 1280, 720, 4, (unsigned char*)pixels, 1280 * 4);
}

static void	main_loop(GLFWwindow *win)
{
	while(!glfwWindowShouldClose(win))
	{
		glfwPollEvents();
	}
}

void key_callback(GLFWwindow* win, int key, int scancode, int action, int mods)
{
	auto	map_manager = reinterpret_cast<Map_manager*>(glfwGetWindowUserPointer(win));
	if (key == GLFW_KEY_P && (action == GLFW_PRESS))
	{
		save_img(*map_manager);
	}
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(win, GL_TRUE);
	}
}

int	main(int ac, char **av)
{
	Basic_terrain		basic_terrain;
	Map_manager			map_manager(basic_terrain);

	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
	GLFWwindow *win = glfwCreateWindow(1920, 1080, "Vox_vdb", NULL, NULL);

	glfwSetWindowUserPointer(win, &map_manager);
	glfwSetKeyCallback(win, key_callback);
	map_manager.init();

	main_loop(win);
	return (EXIT_SUCCESS);
}

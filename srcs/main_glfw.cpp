/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_glfw.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trobicho <trobicho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/15 06:25:54 by trobicho          #+#    #+#             */
/*   Updated: 2021/11/22 19:22:55 by trobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "backend/map_manager/Map_manager.hpp"
#include "frontend/map_generator/Basic_terrain.hpp"
#include "GLFW/glfw3.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

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
		std::cout << "print image" << std::endl;
		std::mutex mutex;
		uint32_t* pixels = map_manager->get_screen(&mutex);
		std::lock_guard<std::mutex> guard(mutex);
		stbi_write_png("./height_map.png", 1920, 1080, 4, pixels, 1920);
	}
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(win, GL_TRUE);
	}
}

int	main(int ac, char **av)
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
	GLFWwindow *win = glfwCreateWindow(1920, 1080, "Vox_vdb", NULL, NULL);

	Basic_terrain		basic_terrain;
	Map_manager			map_manager(basic_terrain);

	glfwSetWindowUserPointer(win, &map_manager);
	map_manager.init();

	glfwSetKeyCallback(win, key_callback);
	main_loop(win);
	return (EXIT_SUCCESS);
}

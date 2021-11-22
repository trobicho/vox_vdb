/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trobicho <trobicho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/15 06:25:54 by trobicho          #+#    #+#             */
/*   Updated: 2021/11/22 12:58:10 by trobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "backend/map_manager/Map_manager.hpp"
#include "frontend/map_generator/Basic_terrain.hpp"
#include "GLFW/glfw3.h"

static void	main_loop(GLFWwindow *win)
{
	while(!glfwWindowShouldClose(win))
	{
		glfwPollEvents();
	}
}

int	main(int ac, char **av)
{
	Basic_terrain		basic_terrain;
	Map_manager			map_manager(basic_terrain);

	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
	GLFWwindow *win = glfwCreateWindow(800, 600, "Vox_vdb"
		, NULL, NULL);
	map_manager.init();
	map_manager.lunch();
	main_loop(win);

	return (EXIT_SUCCESS);
}

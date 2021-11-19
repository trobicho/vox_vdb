/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trobicho <trobicho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/15 06:25:54 by trobicho          #+#    #+#             */
/*   Updated: 2021/11/17 14:29:22 by trobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "backend/map_manager/Map_manager.hpp"
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
	Map_manager			map_manager;

	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	GLFWwindow *win = glfwCreateWindow(800, 600, "Vox_vdb"
		, glfwGetPrimaryMonitor(), NULL);
	map_manager.init();
	map_manager.lunch();

	return (EXIT_SUCCESS);
}

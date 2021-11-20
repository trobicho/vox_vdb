/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trobicho <trobicho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/20 08:00:59 by trobicho          #+#    #+#             */
/*   Updated: 2021/11/20 12:49:28 by trobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <glm/glm.hpp>

struct	s_vec3i
{
	s_vec3i(){}
	s_vec3i(int xa, int ya, int za) : x(xa), y(ya), z(za){};

	glm::vec3	glm_vec3f(){return (glm::vec3((float)x, (float)y, (float)z));}
	int	x;
	int	y;
	int	z;
};

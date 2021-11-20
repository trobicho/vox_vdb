/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Chunk.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trobicho <trobicho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/16 06:05:04 by trobicho          #+#    #+#             */
/*   Updated: 2021/11/19 16:02:22 by trobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "backend/vdb_tree/Vdb_test.h"

class	Chunk
{
	public:
		Chunk(){};
		Chunk(s_vec3i a_origin): origin(a_origin){};
		~Chunk(){};

		s_vec3i		origin;
		bool			need_remesh = false;
		bool			need_unload = false;
};

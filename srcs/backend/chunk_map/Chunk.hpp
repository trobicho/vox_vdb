/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Chunk.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trobicho <trobicho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/16 06:05:04 by trobicho          #+#    #+#             */
/*   Updated: 2021/11/22 13:18:26 by trobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "backend/vdb_tree/Vdb_test.h"

class	Chunk
{
	public:
		Chunk(){};
		Chunk(s_vec3i a_origin, Generate_node *a_chunk_node = nullptr):
			origin(a_origin), chunk_node(a_chunk_node){};
		~Chunk(){};

		s_vec3i				origin;
		bool					need_remesh = false;
		bool					need_unload = false;
		Generate_node *chunk_node;
		uint32_t			height_buffer[1 << 4 * 1 << 4];
};

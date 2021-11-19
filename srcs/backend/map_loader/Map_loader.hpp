/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Map_loader.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trobicho <trobicho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/17 14:35:28 by trobicho          #+#    #+#             */
/*   Updated: 2021/11/19 09:43:10 by trobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <thread>
#include <mutex>
#include <glm/glm.hpp>
#include "backend/map_manager/Map_manager.hpp"

#define		CHUNK_LOG_X	(4)
#define		CHUNK_LOG_Y	(8)
#define		CHUNK_LOG_Z	(4)

class	Map_loader
{
	public:
		Map_loader(Chunk_map &chunk_map): m_chunk_map(chunk_map){};
		~Map_loader(){};

		int			search_new_chunk(glm::vec3 player_pos);

	private:
		int			next_chunk_in_radius(s_vec3i center, s_vec3i &pos, int radius);

		ChunkMap	m_chunk_map;
};

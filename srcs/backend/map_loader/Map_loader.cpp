/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Map_loader.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trobicho <trobicho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/17 14:35:33 by trobicho          #+#    #+#             */
/*   Updated: 2021/11/19 09:39:57 by trobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Map_loader.hpp"

static uint32_t	taxicab_chunk_dist(s_vec3i chunkA, s_vec3i chunkB)
{
	int dx = abs((chunkA.x >> CHUNK_LOG_X) - (chunkB.x >> CHUNK_LOG_X));
	int dz = abs((chunkA.z >> CHUNK_LOG_Z) - (chunkB.z >> CHUNK_LOG_Z));

	return (dx + dz);
}


int		Map_loader::search_new_chunk(glm::vec3 player_pos)
{
	glm::ivec2	i_pos = glm::ivec3((int)player_pos.x
		, (int)player_pos.y, (int)player_pos.z);
	glm::ivec3 	pos = i_pos;
	int			update = 0;

	auto chunk = m_chunk.find(
			std::make_pair((pos.x >> CHUNK_LOG_X) << CHUNK_LOG_X
					, (pos.z >> CHUNK_LOG_Z) << CHUNK_LOG_Z));
	if (chunk == m_chunk.end())
		update += load_pos(pos);
	while (next_chunk_in_radius(i_pos, pos, m_meshing_radius) && !m_update)
	{
		auto chunk = m_chunk.find(
				std::make_pair((pos.x >> CHUNK_LOG_X) << CHUNK_LOG_X
					, (pos.z >> CHUNK_LOG_Z) << CHUNK_LOG_Z));
		if (chunk == m_chunk.end())
		{
			std::lock_guard<std::mutex> guard(m_list_mutex);
			m_chunk_update_list.push_back(pos);
			update++;
		}
		if (update > 100)
			break;
	}
	return (update);
}

int		Map_loader::next_chunk_in_radius(s_vec3i center
			, s_vec3i &pos, int radius)
{
	int dx = (pos.x >> CHUNK_LOG_X) - (center.x >> CHUNK_LOG_X);
	int dz = (pos.z >> CHUNK_LOG_Z) - (center.z >> CHUNK_LOG_Z);

	if (dz == 0 && dx >= 0)
	{
		pos.z -= (1 << CHUNK_LOG_Z);
		if (taxicab_chunk_dist(center, pos) > radius)
			return (0);
	}
	else
	{
		if (dx > 0)
			pos.z -= (1 << CHUNK_LOG_Z);
		else if (dx == 0)
			pos.z += (dz > 0 ? -(1 << CHUNK_LOG_Z) : (1 << CHUNK_LOG_Z));
		else
			pos.z += (1 << CHUNK_LOG_Z);
		if (dz > 0)
			pos.x += (1 << CHUNK_LOG_X);
		else if (dz == 0)
			pos.x += (dx > 0 ? -(1 << CHUNK_LOG_X) : (1 << CHUNK_LOG_X));
		else
			pos.x -= (1 << CHUNK_LOG_X);
	}
	return (1);
}

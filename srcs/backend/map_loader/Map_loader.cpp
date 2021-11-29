/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Map_loader.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trobicho <trobicho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/17 14:35:33 by trobicho          #+#    #+#             */
/*   Updated: 2021/11/29 21:02:02 by trobicho         ###   ########.fr       */
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
	s_vec3i		i_pos = s_vec3i((int)player_pos.x
		, (int)player_pos.y, (int)player_pos.z);
	s_vec3i 	pos = i_pos;
	int			update = 0;

	do
	{
		s_vec3i	chunk_pos(
			(pos.x >> CHUNK_LOG_X) << CHUNK_LOG_X
			, 0
			, (pos.z >> CHUNK_LOG_Z) << CHUNK_LOG_Z
		);
		auto chunk = m_chunk_map.find(std::make_pair(chunk_pos.x, chunk_pos.z));
		if (chunk == m_chunk_map.end())
		{
			Generate_node &root_node = dynamic_cast<Generate_node&>(m_vdb.get_root_node());
			Generate_node *chunk_node = root_node.init_chunk_generation(chunk_pos);
			m_chunk_map.insert({std::make_pair(chunk_pos.x, chunk_pos.z)
				, Chunk(chunk_pos, chunk_node)
			});
			auto new_chunk = m_chunk_map.find(std::make_pair(chunk_pos.x, chunk_pos.z));
			std::cout << "chunk_in_radius: {" 
				<< chunk_pos.x << ", " << chunk_pos.z << "}" << std::endl;
			m_chunk_event_list->push(s_chunk_update_event(
				chunk_pos
				, &new_chunk->second
				, CHUNK_UPDATE_EVENT_GENERATE
			));
			update++;
		}
		if (update > 100)
			break;
	}while (next_chunk_in_radius(i_pos, pos, m_radius_generate) && !m_update);
	m_update = false;
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

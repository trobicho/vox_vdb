/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Map_manager.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trobicho <trobicho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/16 07:57:43 by trobicho          #+#    #+#             */
/*   Updated: 2021/11/29 23:07:57 by trobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Map_manager.hpp"
#include <chrono>

Map_manager::Map_manager(Map_sampler &map_sampler): 
	m_map_node(0, 0, 0)
	, m_map_sampler(map_sampler)
	, m_vdb(m_map_node)
	, m_map_loader(m_chunk_map, m_vdb)
	, m_chunk_manager(m_map_sampler)
{
	m_map_loader.set_chunk_event_list(&m_chunk_manager.get_chunk_event_list());
}

Map_manager::~Map_manager()
{
	m_state = MAP_MANAGER_STATE_QUITING;
	m_manager_thread.join();
}

void	Map_manager::init()
{
	m_map_node = Internal_L1(0, 0, 0);
	m_chunk_manager.lunch();
	m_state = MAP_MANAGER_STATE_GENERATING;
	m_manager_thread = std::thread(&Map_manager::lunch, this);  
}

void	Map_manager::lunch()
{
	m_state |= MAP_MANAGER_STATE_WORKING;
	glm::vec3		player_pos(1300.0f, 0.0f, 1300.0f);
	while (m_state != MAP_MANAGER_STATE_QUITING)
	{
		m_map_loader.search_new_chunk(player_pos);
		//------------------------
		{
			std::lock_guard<std::mutex> guard(m_mutex_screen);
			s_vec3i	chunk_pos;
			s_vec3i	pos;
			for (int y = 0; y < 720 / 16; ++y)
			{
				pos.z = (int)player_pos.z - 720 / 2 + y * 16;
				for (int x = 0; x < 1280 / 16; ++x)
				{
					pos.x = (int)player_pos.x - 1280 / 2 + x * 16;
					s_vec3i	chunk_pos(
						(pos.x >> 4) << 4
						, 0
						, (pos.z >> 4) << 4
					);
					auto new_chunk = m_chunk_map.find(std::make_pair(chunk_pos.x, chunk_pos.z));
					if (new_chunk != m_chunk_map.end())// && new_chunk->second.loaded)
					{
						Chunk	&chunk = new_chunk->second;
						for (int cz = 0; cz < 1 << 4; ++cz)
						{
							for (int cx = 0; cx < 1 << 4; ++cx)
							{
								uint32_t	r, g, b, a;
								r = ((uint32_t)(chunk.surface_buffer[cx + cz * 16].block.x * (255)));
								g = ((uint32_t)(chunk.surface_buffer[cx + cz * 16].block.y * (255)));
								b = ((uint32_t)(chunk.surface_buffer[cx + cz * 16].block.z * (255)));
								a = ((uint32_t)(chunk.surface_buffer[cx + cz * 16].block.w * (255)));

								uint32_t	color =
									+ (a & 255) * (1 << 24)
									+ (b & 255) * (1 << 16)
									+ (g & 255) * (1 << 8)
									+ r;
								m_screen[x * 16  + cx + (y * 16 + cz) * 1280] = color;
							}
						}
					}
				}
			}
		}
		//------------------------
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	m_chunk_manager.quit();
}

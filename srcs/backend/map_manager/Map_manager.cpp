/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Map_manager.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trobicho <trobicho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/16 07:57:43 by trobicho          #+#    #+#             */
/*   Updated: 2021/11/22 13:37:42 by trobicho         ###   ########.fr       */
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
}

void	Map_manager::init()
{
	m_map_node = Internal_L1(0, 0, 0);
	m_chunk_manager.lunch();
	m_state = MAP_MANAGER_STATE_GENERATING;
}

void	Map_manager::lunch()
{
	m_state |= MAP_MANAGER_STATE_WORKING;
	glm::vec3		player_pos(15000.0f, 0.0f, 15000.0f);
	while (m_state != MAP_MANAGER_STATE_QUITING)
	{
		m_map_loader.search_new_chunk(player_pos);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	m_chunk_manager.quit();
}

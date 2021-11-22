/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Chunk_manager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trobicho <trobicho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/15 06:32:36 by trobicho          #+#    #+#             */
/*   Updated: 2021/11/22 13:33:17 by trobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Chunk_manager.hpp"
#include <chrono>
#include <iostream>

Chunk_manager::Chunk_manager(Map_sampler &map_sampler, int nb_thread) : 
	m_map_sampler(map_sampler), m_nb_thread(nb_thread)
{
}

Chunk_manager::~Chunk_manager()
{
	m_quit = true;
	//for (auto& thread : m_thread_buffer)
		//thread.thread.join();
	m_manager_thread.join();
}

void	Chunk_manager::lunch()
{
	m_manager_thread = std::thread(&Chunk_manager::manager, this);
}

void	Chunk_manager::thread_chunk_event()
{
	while (!m_quit)
	{
		s_chunk_update_event	event;
		if (m_chunk_event_list.pop(&event))
		{
			std::cout << "chunk = (" << event.pos.x << ", " << event.pos.z << ")" << std::endl;
			event.chunk->chunk_node->generate_one_chunk(m_map_sampler, event.pos);
			for (int z = 0; z < 1 << 4; ++z)
			{
				for (int x = 0; x < 1 << 4; ++x)
				{
					event.chunk->height_buffer[x + z * (1 << 4)] =
						m_map_sampler.get_slice_sample(Sampler(), s_vec3i(
							event.pos.x + x
							, 0	
							, event.pos.z + z
						)).height;
				}
			}
		}
		else
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

void	Chunk_manager::manager()
{
	for (int i = 0; i < m_nb_thread; ++i)
	{
		m_thread_buffer.push_back(s_stats_thread());
		m_thread_buffer.back().thread = 
			std::thread(&Chunk_manager::thread_chunk_event, this);
	}
	while(!m_quit)
	{
		int	index = 0;
		for (auto& thread : m_thread_buffer)
		{
			auto stats = thread.get_stats();
			/*
			std::cout << "thread [" << index << "]:" << std::endl;
			std::cout << "    nb chunk generated: " << stats.nb_chunk_generated << std::endl;
			std::cout << "    nb chunk meshed : " << stats.nb_chunk_meshed << std::endl;
			*/
			index++;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Chunk_manager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trobicho <trobicho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/15 06:32:36 by trobicho          #+#    #+#             */
/*   Updated: 2021/11/29 21:01:33 by trobicho         ###   ########.fr       */
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
			auto	*chunk_node = dynamic_cast<Node_v*>(event.chunk->chunk_node);
			//-------------------------------
			for (int z = 0; z < 1 << 4; ++z)
			{
				for (int x = 0; x < 1 << 4; ++x)
				{
					auto sample = m_map_sampler.get_slice_sample(Sampler(), s_vec3i(
						event.pos.x + x
						, 0	
						, event.pos.z + z
					));
					uint32_t height = (sample.height > sample.water_height)
							? sample.height : sample.water_height;
					int y = 0;
					event.chunk->surface_buffer[x + z * (1 << 4)].generated_height = height;
					for (y = height - 1; y >= 0; --y)
					{
						event.chunk->surface_buffer[x + z * (1 << 4)].height = y + 1;
						glm::vec4 color = m_map_sampler.get_color_from_block_type(
							chunk_node->get_vox(event.pos.x + x, y, event.pos.z + z)
						);
						float alpha = color.w;
						color *= (float)((y / 128.0f) > 1.0f ? 1.0f : (y / 128.0f));
						color.w = alpha;
						event.chunk->surface_buffer[x + z * (1 << 4)].block = color;
						if (color.w >= 0.3)
							break;
					}
				}
			}
			event.chunk->loaded = true;
			//-------------------------------
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Chunk_manager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trobicho <trobicho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/15 06:32:36 by trobicho          #+#    #+#             */
/*   Updated: 2021/11/19 16:10:42 by trobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Chunk_manager.hpp"
#include <chrono>
#include <iostream>

Chunk_manager::Chunk_manager(int nb_thread) : m_nb_thread(nb_thread)
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
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
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

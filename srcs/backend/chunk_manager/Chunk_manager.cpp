/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Chunk_manager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trobicho <trobicho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/15 06:32:36 by trobicho          #+#    #+#             */
/*   Updated: 2021/11/17 14:25:51 by trobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Chunk_manager.hpp"
#include <chrono>
#include <iostream>

Chunk_manager::Chunk_manager(int nb_thread) : m_nb_thread(nb_thread);
{
}

Chunk_manager::~Chunk_manager()
{
	m_quit = true;
	for (auto& thread : m_thread_buffer)
		thread.thread.join();
	m_manager_thread.join();
}

void	Chunk_manager::lunch()
{
	m_manager_thread = std::thread(&Chunk_manager::manager(), this);
}

void	Chunk_manager::manager()
{
	for (int i = 0; i < m_nb_thread; ++i)
	{
		m_thread_buffer.push_back(utils::thread::thread_wrapper<s_stats_type>);
		m_thread_buffer.end().thread =
			std::thread(&Map_loader::thread_list_update, this);
	}
	while(!m_quit)
	{
		int	index = 0;
		for (auto& thread : m_thread_buffer)
		{
			auto state = thread.get_state();
			std::cout << "thread [" << index << "]:"
			std::cout << "    nb chunk generated: " << state.nb_chunk_generated << std::endl;
			std::cout << "    nb chunk meshed : " << state.nb_chunk_meshed << std::endl;
			index++;
		}
		usleep(100000);
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Map_manager.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trobicho <trobicho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/16 05:34:02 by trobicho          #+#    #+#             */
/*   Updated: 2021/11/22 16:36:55 by trobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "backend/chunk_manager/Chunk_manager.hpp"
#include "backend/map_loader/Map_loader.hpp"
#include "backend/chunk_map/Chunk_map.hpp"
#include <mutex>

#define		MAP_MANAGER_STATE_QUITING			0x0000
#define		MAP_MANAGER_STATE_WORKING			0x0001
#define		MAP_MANAGER_STATE_GENERATING	0x0002
#define		MAP_MANAGER_STATE_MESHING			0x0004
#define		MAP_MANAGER_STATE_SEARCHING		0x0008
#define		MAP_MANAGER_STATE_UNLOADING		0x0016

#define		MAP_MANAGER_STATE_FULL_WORK		0x0031

class	Map_manager
{
	public:
		Map_manager(Map_sampler &map_sampler);
		~Map_manager();

		void		init();
		void		lunch();
		void		quit() {m_state &= MAP_MANAGER_STATE_QUITING;}
	
		unsigned char*	get_screen(std::mutex *mutex){
			mutex = &m_mutex_screen;
			return (m_screen);
		}

	private:
		std::thread			m_manager_thread;
		Map_sampler			&m_map_sampler;
		Internal_L1			m_map_node;
		Vdb_test				m_vdb;
		Chunk_map				m_chunk_map;
		Chunk_manager		m_chunk_manager;
		Map_loader			m_map_loader;

		unsigned char		m_screen[1920 * 1080];
		std::mutex			m_mutex_screen;

		uint32_t				m_state;
};

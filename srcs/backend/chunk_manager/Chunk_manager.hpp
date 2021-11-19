/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Chunk_manager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trobicho <trobicho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/15 06:32:34 by trobicho          #+#    #+#             */
/*   Updated: 2021/11/18 23:38:34 by trobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "backend/vdb_tree/Vdb_test.h"
#include "backend/utils.hpp"

#include <vector>
#include <list>

#define	CHUNK_MANAGER_STATE_WORKING			0x0001
#define	CHUNK_MANAGER_STATE_GENERATING	0x0002
#define CHUNK_MANAGER_STATE_MESHING			0x0004

#define CHUNK_MANAGER_STATE_FULL_WORK		0x0007

#define	CHUNK_UPDATE_EVENT_MESH					0x0001
#define	CHUNK_UPDATE_EVENT_GENERATE			0x0002
#define	CHUNK_UPDATE_EVENT_REMESH				0x0004
#define	CHUNK_UPDATE_EVENT_REGENERATE		0x0008
#define	CHUNK_UPDATE_EVENT_BLOCK_CHANGE	0x0008

struct	s_chunk_update_event
{
	s_vec3i			pos;
	uint32_t		new_block_type;
	uint32_t		flags;
};

struct	s_stats_type
{
	uint32_t	nb_chunk_generated = 0;
	uint32_t	nb_chunk_meshed = 0;
};

class	Chunk_manager
{
	public:
		Chunk_manager(int nb_thread = 20);
		~Chunk_manager();

		void		lunch();
		void		manager();
		void		quit() {m_quit = true;}

	private:
		bool								m_quit = false;
		//Chunk_generator			&m_mapper;

		std::thread					m_manager_thread;
		utils::thread::event_list<s_chunk_update_event>					m_chunk_update_list;
		std::vector<utils::thread::thread_wrapper<s_stats_type>>	m_thread_buffer;
		int									m_nb_thread;
		uint32_t						m_state;
};

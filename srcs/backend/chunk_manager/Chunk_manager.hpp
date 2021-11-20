/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Chunk_manager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trobicho <trobicho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/15 06:32:34 by trobicho          #+#    #+#             */
/*   Updated: 2021/11/19 15:45:32 by trobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "backend/vdb_tree/Vdb_test.h"
#include "backend/utils.hpp"
#include "backend/chunk_map/Chunk.hpp"

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
	s_chunk_update_event(){};
	s_chunk_update_event(s_vec3i a_pos, Chunk* a_chunk, uint32_t a_flags):
		pos(a_pos), chunk(a_chunk), flags(a_flags){};
	s_vec3i			pos;
	Chunk				*chunk;
	uint32_t		flags;
	uint32_t		new_block_type;
};

struct	s_stats_type
{
	uint32_t	nb_chunk_generated = 0;
	uint32_t	nb_chunk_meshed = 0;
};

using s_chunk_event_list = utils::thread::event_list<s_chunk_update_event>;
using s_stats_thread = utils::thread::thread_wrapper<s_stats_type>;

class	Chunk_manager
{
	public:
		Chunk_manager(int nb_thread = 20);
		~Chunk_manager();

		void		lunch();
		void		manager();
		s_chunk_event_list&
						get_chunk_event_list(){return (m_chunk_event_list);}
		void		quit() {m_quit = true;}

	private:
		void		thread_chunk_event();

		bool								m_quit = false;
		//Chunk_generator			&m_mapper;

		std::thread					m_manager_thread;
		s_chunk_event_list	m_chunk_event_list;
		std::vector<s_stats_thread>
												m_thread_buffer;
		int									m_nb_thread;
		uint32_t						m_state;
};

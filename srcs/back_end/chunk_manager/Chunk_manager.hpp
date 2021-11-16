/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Chunk_manager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trobicho <trobicho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/15 06:32:34 by trobicho          #+#    #+#             */
/*   Updated: 2021/11/16 06:06:42 by trobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "back_end/vdb_tree/Vdb_test.h"
#include "back_end/utils.hpp"

#include <vector>
#include <list>

#define	CHUNK_UPDATE_EVENT_MESH					0x0001
#define	CHUNK_UPDATE_EVENT_GENERATE			0x0002
#define	CHUNK_UPDATE_EVENT_REMESH				0x0004
#define	CHUNK_UPDATE_EVENT_REGENERATE		0x0008
#define	CHUNK_UPDATE_EVENT_BLOCK_CHANGE	0x0008

struct	s_chunk_update_event
{
	gml::ivec3	pos;
	uint32_t		new_block_type;
	uint32_t		flags;
};

struct	s_state_type
{
	uint32_t	nb_chunk_generated = 0;
	uint32_t	nb_chunk_meshed = 0;
};

class	Chunk_manager
{
	public:
		Chunk_mamager(int nb_thread = 20);
		~Chunk_mamager();

		void		quit() {m_quit = true;}

	private:
		bool								m_quit = false;
		Chunk_generator			&m_mapper;

		t_chunk_map					m_chunk_map;

		std::thread					m_manager_thread;
		utils::thread_safe::event_list<s_chunk_update_event>		m_chunk_update_list;
		std::vector<utils::thread::thread_wrapper<stats_type>>	m_thread_buffer;
		int									m_nb_thread;
		glm::vec3						m_player_pos
};

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Map_manager.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trobicho <trobicho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/16 05:34:02 by trobicho          #+#    #+#             */
/*   Updated: 2021/11/19 00:04:39 by trobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "backend/vdb_tree/Vdb_test.h"
#include "Chunk.hpp"
#include "backend/chunk_manager/Chunk_manager.hpp"
#include "backend/map_loader/Map_loader.hpp"
#include <unordered_map>

#define		MAP_MANAGER_STATE_QUITING			0x0000
#define		MAP_MANAGER_STATE_WORKING			0x0001
#define		MAP_MANAGER_STATE_GENERATING	0x0002
#define		MAP_MANAGER_STATE_MESHING			0x0004
#define		MAP_MANAGER_STATE_SEARCHING		0x0008
#define		MAP_MANAGER_STATE_UNLOADING		0x0016

#define		MAP_MANAGER_STATE_FULL_WORK		0x0031

using		t_chunk_key = std::pair<uint32_t, uint32_t>;
struct	s_key_hash
{
	size_t operator()(const t_chunk_key& key) const
	{
		return (key.first | (key.second << 10));
	}
};
using		Chunk_map	= std::unordered_map<t_chunk_key, Chunk, s_key_hash>;

class	Map_manager
{
	public:
		Map_manager();
		~Map_manager();

		void		init();
		void		lunch();
		void		quit() {m_state &= MAP_MANAGER_STATE_QUITING;}

	private:
		Chunk_manager		m_chunk_manager;
		Map_loader			m_map_loader;
		Vdb_test				m_vdb;
		Chunk_map				m_chunk_map;
		Internal_L1			m_map_node = Internal_L1(0, 0, 0);

		uint32_t				m_state;
};

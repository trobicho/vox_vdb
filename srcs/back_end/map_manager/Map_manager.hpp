/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Map_manager.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trobicho <trobicho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/16 05:34:02 by trobicho          #+#    #+#             */
/*   Updated: 2021/11/16 06:43:24 by trobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "back_end/vdb_tree/Vdb_test.h"
#include "Chunk.hpp"
#include <unordered_map>

#define		CHUNK_LOG_X	(4)
#define		CHUNK_LOG_Y	(8)
#define		CHUNK_LOG_Z	(4)

using		t_chunk_key = std::pair<uint32_t, uint32_t>
struct	s_key_hash
{
	size_t operator()(const ChunkKey& key) const
	{
		return (key.first | (key.second << 10));
	}
};
using		Chunk_map	std::unordered_map<t_chunk_key, Chunk, Key_hash>

class	Map_manager
{
	public:
		Map_manager();
		~Map_manager();

	private:
		Vdb_test				m_vdb;
		Chunk_manager		m_chunk_manager;
		Map_loader			m_map_loader;
		Chunk_map				m_chunk_map;
};

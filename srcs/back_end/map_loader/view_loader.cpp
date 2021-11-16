/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   view_loader.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trobicho <trobicho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/15 12:22:55 by trobicho          #+#    #+#             */
/*   Updated: 2021/11/16 05:17:26 by trobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int		Chunk_manager::search_new_chunk
{
	glm::ivec2	i_pos = glm::ivec3((int)m_player_pos.x
		, (int)m_player_pos.y, (int)m_player_pos.z);
	glm::ivec3 	pos = i_pos;
	int			update = 0;

	auto chunk = m_chunk.find(
			std::make_pair((pos.x >> CHUNK_LOG_X) << CHUNK_LOG_X
					, (pos.z >> CHUNK_LOG_Z) << CHUNK_LOG_Z));
	if (chunk == m_chunk.end())
		update += load_pos(pos);
	while (next_chunk_in_radius(i_pos, pos, m_meshing_radius) && !m_update)
	{
		auto chunk = m_chunk.find(
				std::make_pair((pos.x >> CHUNK_LOG_X) << CHUNK_LOG_X
					, (pos.z >> CHUNK_LOG_Z) << CHUNK_LOG_Z));
		if (chunk == m_chunk.end())
		{
			std::lock_guard<std::mutex> guard(m_list_mutex);
			m_chunk_update_list.push_back(pos);
			update++;
		}
		if (update > 100)
			break;
	}
	return (update);
}

int		Map_loader::next_chunk_in_radius(s_vec3i center
			, s_vec3i &pos, int radius)
{
	int dx = (pos.x >> CHUNK_LOG_X) - (center.x >> CHUNK_LOG_X);
	int dz = (pos.z >> CHUNK_LOG_Z) - (center.z >> CHUNK_LOG_Z);

	if (dz == 0 && dx >= 0)
	{
		pos.z -= (1 << CHUNK_LOG_Z);
		if (taxicab_chunk_dist(center, pos) > radius)
			return (0);
	}
	else
	{
		if (dx > 0)
			pos.z -= (1 << CHUNK_LOG_Z);
		else if (dx == 0)
			pos.z += (dz > 0 ? -(1 << CHUNK_LOG_Z) : (1 << CHUNK_LOG_Z));
		else
			pos.z += (1 << CHUNK_LOG_Z);
		if (dz > 0)
			pos.x += (1 << CHUNK_LOG_X);
		else if (dz == 0)
			pos.x += (dx > 0 ? -(1 << CHUNK_LOG_X) : (1 << CHUNK_LOG_X));
		else
			pos.x -= (1 << CHUNK_LOG_X);
	}
	return (1);
}

int		Map_loader::load_pos(s_vec3i pos)
{
	uint32_t		value;
	s_vbox			box;
	const Node_v	*node;

	if ((node = m_vdb.get_interresting_node(s_vec3i(pos.x, 0, pos.z), value))
			!= nullptr && node->get_child_slog().x >= CHUNK_LOG_X)
	{
		box.len = s_vec3i(1 << CHUNK_LOG_X, 1 << CHUNK_LOG_Y
				, 1 << CHUNK_LOG_Z);
		box.origin = s_vec3i((pos.x >> CHUNK_LOG_X) << CHUNK_LOG_X
				, 0, (pos.z >> CHUNK_LOG_Z) << CHUNK_LOG_Z);
		generate_one_chunck(box);
		s_vbox	box2 = box;
		box2.origin.x += (1 << CHUNK_LOG_X);
		generate_one_chunck(box2);
		box2.origin.z += (1 << CHUNK_LOG_Z);
		generate_one_chunck(box2);
		box2.origin.x -= (1 << CHUNK_LOG_X);
		generate_one_chunck(box2);
		box2.origin.x -= (1 << CHUNK_LOG_X);
		generate_one_chunck(box2);
		box2.origin.z -= (1 << CHUNK_LOG_Z);
		generate_one_chunck(box2);
		box2.origin.z -= (1 << CHUNK_LOG_Z);
		generate_one_chunck(box2);
		box2.origin.x += (1 << CHUNK_LOG_X);
		generate_one_chunck(box2);
		box2.origin.x += (1 << CHUNK_LOG_X);
		generate_one_chunck(box2);
		std::lock_guard<std::mutex> guard(m_mesh_mutex);
		if (m_update)
			return (0);
		if (mesh_one_chunck(box, m_chunk.insert(
				{std::make_pair(box.origin.x, box.origin.z)
				, s_chunk(m_moore_access)}).first->second) == -1)
				return (0);
		m_nb_chunk++;
		return (1);
	}
	else
	{
		box.len = s_vec3i(1 << CHUNK_LOG_X, 1 << CHUNK_LOG_Y
				, 1 << CHUNK_LOG_Z);
		box.origin = s_vec3i((pos.x >> CHUNK_LOG_X) << CHUNK_LOG_X
				, 0, (pos.z >> CHUNK_LOG_Z) << CHUNK_LOG_Z);
		auto chunk = m_chunk.find(std::make_pair(box.origin.x, box.origin.z));
		if (chunk != m_chunk.end())
		{
			if (chunk->second.need_unload)
				chunk->second.need_unload = false;
				return (0);
		}
		if (m_update)
			return (0);
		s_vbox	box2 = box;
		box2.origin.x += (1 << CHUNK_LOG_X);
		generate_one_chunck(box2);
		box2.origin.z += (1 << CHUNK_LOG_Z);
		generate_one_chunck(box2);
		box2.origin.x -= (1 << CHUNK_LOG_X);
		generate_one_chunck(box2);
		box2.origin.x -= (1 << CHUNK_LOG_X);
		generate_one_chunck(box2);
		box2.origin.z -= (1 << CHUNK_LOG_Z);
		generate_one_chunck(box2);
		box2.origin.z -= (1 << CHUNK_LOG_Z);
		generate_one_chunck(box2);
		box2.origin.x += (1 << CHUNK_LOG_X);
		generate_one_chunck(box2);
		box2.origin.x += (1 << CHUNK_LOG_X);
		generate_one_chunck(box2);
		std::lock_guard<std::mutex> guard(m_mesh_mutex);
		if (mesh_one_chunck(box, m_chunk.insert(
				{std::make_pair(box.origin.x, box.origin.z)
				, s_chunk(m_moore_access)}).first->second) == -1)
				return (0);
		m_nb_chunk++;
		return (1);
	}
	return (0);
}

int		Map_loader::generate_one_chunck(s_vbox &box)
{
	const Node_v	*node;
	uint32_t		value;

	if ((node = m_vdb.get_interresting_node(box.origin, value))
			!= nullptr && node->get_child_slog().x >= CHUNK_LOG_X)
	{
		if (m_map.generate(m_vdb, box))
			return (1);
		/*
		auto	time = std::chrono::high_resolution_clock::now();
		if (m_map.generate(m_vdb, box))
			return (1);
		auto	gtime = std::chrono::high_resolution_clock::now();
		auto	time_gen = std::chrono::duration<float
			, std::chrono::seconds::period>(gtime - time).count();
		time = std::chrono::high_resolution_clock::now();
		*/
		//std::cout << "time to generate = " << time_gen << std::endl;
	}
	return (0);
}

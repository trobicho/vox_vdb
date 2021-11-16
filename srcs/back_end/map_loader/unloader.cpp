/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unloader.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trobicho <trobicho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/16 05:13:25 by trobicho          #+#    #+#             */
/*   Updated: 2021/11/16 05:17:24 by trobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

void	Chunk_manager::unload_far_chunk()
{
	int i = 0;
	t_chunk_cont:: iterator	chunk_it;

	while (!m_quit)
	{
		if (m_need_unload > 10)
		{
			for (chunk_it = m_chunk.begin(); chunk_it != m_chunk.end()
					&& m_need_unload > 0;)
			{
				if (chunk_it->second.has_unload)
				{
					if (chunk_it->second.in_vbo)
						chunk_it->second.unload(m_vulk);
					else
						chunk_it->second.mesh.reset();
					chunk_it = m_chunk.erase(chunk_it);
					m_nb_chunk--;
					m_need_unload--;
				}
				else
					++chunk_it;
			}
		}
		if (m_need_unload < 0)
			m_need_unload = 0;
		usleep(100000);
	}
}

void	Map_loader::tag_unload_far_chunk()
{
	glm::vec3	p_pos = m_player.get_cam_pos();
	s_vec3i		i_pos = s_vec3i((int)p_pos.x, (int)p_pos.y, (int)p_pos.z);
	int			i = 0;

	t_chunk_cont:: iterator	chunk_it;
	for (chunk_it = m_chunk.begin(); chunk_it != m_chunk.end(); ++chunk_it)
	{
		if (taxicab_chunk_dist(i_pos, chunk_it->second.origin)
			> m_unload_meshing_radius)
			chunk_it->second.need_unload = true;
		if (m_update)
			break;
	}
}


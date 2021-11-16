/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Vdb_test.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trobicho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/02 21:06:27 by trobicho          #+#    #+#             */
/*   Updated: 2019/12/24 19:57:09 by trobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Vdb_test.h"

Vdb_test::Vdb_test(Node_v &root): m_root_static(root), m_min(0, 0, 0)
{
	m_max.x = (1 << m_root_static.get_slog().x);
	m_max.y = (1 << m_root_static.get_slog().y);
	m_max.z = (1 << m_root_static.get_slog().z);
	std::cout << m_max.x << ", " << m_max.y << ", " << m_max.z << std::endl;
}

void		Vdb_test::set_vox(uint32_t value, s_vec3i v)
{
	if (v.x >= m_min.x && v.y >= m_min.y && v.z >= m_min.z
		&& v.x < m_max.x && v.y < m_max.y && v.z < m_max.z)
	{
		m_root_static.set_vox(value, v.x, v.y, v.z);
	}
}

void		Vdb_test::unset_vox(s_vec3i v)
{
	if (v.x >= m_min.x && v.y >= m_min.y && v.z >= m_min.z
		&& v.x < m_max.x && v.y < m_max.y && v.z < m_max.z)
	{
		m_root_static.unset_vox(v.x, v.y, v.z);
	}
}

uint32_t	Vdb_test::get_vox(s_vec3i v) const
{
	if (v.x >= m_min.x && v.y >= m_min.y && v.z >= m_min.z
		&& v.x < m_max.x && v.y < m_max.y && v.z < m_max.z)
	{
		return (m_root_static.get_vox(v.x, v.y, v.z));
	}
	return (0);
}

int			Vdb_test::remove_node_by_slog(s_vec3i node_pos, uint32_t slog)
{
	if (node_pos.x >= m_min.x && node_pos.y >= m_min.y && node_pos.z >= m_min.z
		&& node_pos.x < m_max.x && node_pos.y < m_max.y && node_pos.z < m_max.z)
	{
		return (m_root_static.remove_node_by_slog(node_pos, slog));
	}
	return (0);
}

const Node_v
			*Vdb_test::get_interresting_node(s_vec3i v, uint32_t &value) const
{
	value = 0;
	if (v.x >= m_min.x && v.y >= m_min.y && v.z >= m_min.z
		&& v.x < m_max.x && v.y < m_max.y && v.z < m_max.z)
	{
		return (m_root_static.get_interresting_node(v, value));
	}
	return (nullptr);
}

void		Vdb_test::pruning()
{
	m_root_static.pruning();
}

void		Vdb_test::mesh(Mesh_interface &mesh) const
{
	m_root_static.mesh(mesh);
}

void		Vdb_test::mesh(Mesh_interface &mesh, const s_vbox &box) const
{
	
	if (box.origin.x >= m_min.x
		&& box.origin.y >= m_min.y 
		&& box.origin.z >= m_min.z
		&& box.origin.x + box.len.x <= m_max.x
		&& box.origin.y + box.len.y <= m_max.y 
		&& box.origin.z + box.len.z <= m_max.z)
	{
		m_root_static.mesh(mesh, box);
	}
}

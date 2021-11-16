/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mesh.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trobicho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/11 06:56:37 by trobicho          #+#    #+#             */
/*   Updated: 2021/11/13 12:54:20 by trobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"
#include <vector>
#include <bitset>
#include "Vertex.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Mesh_interface.h"

class	Moore_accessor;

template <class Vertex>
class	Mesh: public Mesh_interface
{
	public:
		Mesh(Moore_accessor &moore_access);
		~Mesh(){};

		Mesh& operator=(Mesh other)
		{
			this->vertex_buffer = other.vertex_buffer;
			this->index_buffer = other.index_buffer;
			return (*this);
		}

		void		reset();
		uint32_t	get_nb_vertex() const {return(vertex_buffer.size());}
		uint32_t	get_nb_index() const {return(index_buffer.size());}
		void		remove_vertex(uint32_t offset, uint32_t size);
		void		add_big_cube_from_node(s_vec3i v
						, uint32_t type, void *node_ptr);
		void		add_cube_from_node(s_vec3i v
						, uint32_t type, void *node_ptr);

		std::vector<Vertex>		vertex_buffer;
		std::vector<uint32_t>	index_buffer;
		std::vector<uint32_t>	index_buffer_blend;

	private:
		int			add_vertex(Vertex v, uint32_t idx, bool is_opaque = true);
		int			add_vertex_with_basic_index(Vertex v
						, bool is_opaque = true);
		void		add_index(uint32_t idx, bool is_opaque = true);
		int			add_vertex_with_no_index(Vertex v, uint8_t ao = 3);
		void		get_needed_face(std::bitset<6> &f_b
							, s_vec3i v, void *node_ptr, uint32_t type);
		void		get_needed_vertex(std::bitset<8> &v_b);
		void		add_needed_vertex(Vertex v, uint32_t l
						, std::bitset<8> &v_b, uint32_t v_idx[8]);
		uint8_t		get_ao(int idx);
		uint8_t		vertex_ao(bool side1, bool side2, bool corner)
		{
			if(side1 && side2)
				return (0);
			return (3 - (side1 + side2 + corner));
		}

		Moore_accessor		&m_moore_access;
};

#include "Moore_accessor.h"

template <class Vertex>
Mesh<Vertex>::Mesh(Moore_accessor &moore_access): m_moore_access(moore_access)
{
}

template <class Vertex>
void	Mesh<Vertex>::reset()
{
	vertex_buffer.clear();
	index_buffer.clear();
	index_buffer_blend.clear();
}

template <class Vertex>
int		Mesh<Vertex>::add_vertex(Vertex v, uint32_t idx, bool is_opaque)
{
	vertex_buffer.push_back(v);
	if (is_opaque)
		index_buffer.push_back(idx);
	else
		index_buffer_blend.push_back(idx);
	return (vertex_buffer.size() - 1);
}

template <class Vertex>
int		Mesh<Vertex>::add_vertex_with_basic_index(Vertex v, bool is_opaque)
{
	vertex_buffer.push_back(v);
	if (is_opaque)
		index_buffer.push_back(vertex_buffer.size() - 1);
	else
		index_buffer_blend.push_back(vertex_buffer.size() - 1);
	return (vertex_buffer.size() - 1);
}

template <class Vertex>
int		Mesh<Vertex>::add_vertex_with_no_index(Vertex v, uint8_t ao)
{
	v.ao = ao;
	vertex_buffer.push_back(v);
	return (vertex_buffer.size() - 1);
}

template <class Vertex>
void	Mesh<Vertex>::add_index(uint32_t idx, bool is_opaque)
{
	if (is_opaque)
		index_buffer.push_back(idx);
	else
		index_buffer_blend.push_back(idx);
}

template <class Vertex>
void	Mesh<Vertex>::remove_vertex(uint32_t offset, uint32_t size)
{
	bool	del;

	if (offset + size >= vertex_buffer.size())
		return ;
	vertex_buffer.erase(vertex_buffer.begin() + offset
			, vertex_buffer.begin() + offset + size);
	for (int t = 0; t < index_buffer.size();)
	{
		del = false;
		for (int i = 0; i < 3; i++)
		{
			if (index_buffer[t + i] >= offset
				&& index_buffer[t + i] < offset + size)
			{
				index_buffer.erase(index_buffer.begin() + t
					, index_buffer.begin() + t + 3);
				del = true;
				break;
			}
			else if (index_buffer[t + i] >= offset + size)
				index_buffer[t + i] -= size;
		}
		if (!del)
			t += 3;
	}
}

template <class Vertex>
void	Mesh<Vertex>::get_needed_face(std::bitset<6> &f_b, s_vec3i v
			, void *node_ptr, uint32_t type)
{
	const Vdb_test	&my_vdb = m_moore_access.get_vdb_ref();
	const Node_v	*node = (Node_v*)node_ptr;
	const Node_v	*moore_node;
	s_vec3i			neigh_v;
	uint32_t		value;
	s_vec3i			node_slog = node->get_child_slog();

	f_b.reset();

	neigh_v = (s_vec3i){v.x, v.y + (1 << node_slog.y) , v.z};
	moore_node = my_vdb.get_interresting_node(neigh_v, value);
	if (!value || (block_is_opaque(type) && !block_is_opaque(value))
		|| moore_node->get_child_slog().x < node_slog.x)
		f_b.set(0);
	neigh_v = (s_vec3i){v.x, v.y - 1, v.z};
	moore_node = my_vdb.get_interresting_node(neigh_v, value);
	if (!value || (block_is_opaque(type) && !block_is_opaque(value))
		|| moore_node->get_child_slog().x < node_slog.x)
		f_b.set(1);
	neigh_v = (s_vec3i){v.x, v.y, v.z + (1 << node_slog.z)};
	moore_node = my_vdb.get_interresting_node(neigh_v, value);
	if (!value || (block_is_opaque(type) && !block_is_opaque(value))
		|| moore_node->get_child_slog().x < node_slog.x)
		f_b.set(2);
	neigh_v = (s_vec3i){v.x, v.y, v.z - 1};
	moore_node = my_vdb.get_interresting_node(neigh_v, value);
	if (!value || (block_is_opaque(type) && !block_is_opaque(value))
		|| moore_node->get_child_slog().x < node_slog.x)
		f_b.set(3);
	neigh_v = (s_vec3i){v.x + (1 << node_slog.x), v.y, v.z};
	moore_node = my_vdb.get_interresting_node(neigh_v, value);
	if (!value || (block_is_opaque(type) && !block_is_opaque(value))
		|| moore_node->get_child_slog().x < node_slog.x)
		f_b.set(4);
	neigh_v = (s_vec3i){v.x - 1, v.y, v.z};
	moore_node = my_vdb.get_interresting_node(neigh_v, value);
	if (!value || (block_is_opaque(type) && !block_is_opaque(value))
		|| moore_node->get_child_slog().x < node_slog.x)
		f_b.set(5);
}

template <class Vertex>
void	Mesh<Vertex>::get_needed_vertex(std::bitset<8> &v_b)
{
	v_b.reset();
	if (!m_moore_access[MOORE_UP])
		v_b |= ((1 << 4) - 1);
	if (!m_moore_access[MOORE_DOWN])
		v_b |= ((1 << 4) - 1) << 4;
	if (!m_moore_access[MOORE_FRONT])
	{
		v_b |= ((((1 << 2) - 1) << 4) | ((1 << 2) - 1));
	}
	if (!m_moore_access[MOORE_BACK])
	{
		v_b |= ((((1 << 2) - 1) << 6) | (((1 << 2) - 1) << 2));
	}
	if (!m_moore_access[MOORE_RIGHT])
	{
		v_b |= ((((1 << 2) - 1) << 5) | (((1 << 2) - 1) << 1));
	}
	if (!m_moore_access[MOORE_LEFT])
	{
		v_b |= ~((((1 << 2) - 1) << 5) | (((1 << 2) - 1) << 1));
	}
}

template <class Vertex>
void	Mesh<Vertex>::add_needed_vertex(Vertex v, uint32_t l, std::bitset<8> &v_b
			, uint32_t v_idx[8])
{
	Vertex vc = v;

	if (v_b[0])
		v_idx[0] = add_vertex_with_no_index(vc, get_ao(0));
	vc.pos.x += l;
	if (v_b[1])
		v_idx[1] = add_vertex_with_no_index(vc, get_ao(1));
	vc.pos.z += l;
	if (v_b[2])
		v_idx[2] = add_vertex_with_no_index(vc, get_ao(2));
	vc.pos.x -= l;
	if (v_b[3])
		v_idx[3] = add_vertex_with_no_index(vc, get_ao(3));

	vc = v;
	vc.pos.y += l;
	if (v_b[4])
		v_idx[4] = add_vertex_with_no_index(vc, get_ao(4));
	vc.pos.x += l;
	if (v_b[5])
		v_idx[5] = add_vertex_with_no_index(vc, get_ao(5));
	vc.pos.z += l;
	if (v_b[6])
		v_idx[6] = add_vertex_with_no_index(vc, get_ao(6));
	vc.pos.x -= l;
	if (v_b[7])
		v_idx[7] = add_vertex_with_no_index(vc, get_ao(7));
}

template <class Vertex>
void	Mesh<Vertex>::add_big_cube_from_node(s_vec3i v, uint32_t type, void *node_ptr)
{
	std::bitset<6>	f_b;
	uint32_t		v_idx[8];
	Node_v			*node = (Node_v*)node_ptr;
	s_vec3i			node_slog = node->get_child_slog();
	s_vec3i			v_add;

	get_needed_face(f_b, v, node_ptr, type);
	if (f_b[0])
	{
		v_add = v;
		v_add.y = v.y + (1 << node_slog.y) - 1;
		for (int x = 0; x < 1 << node_slog.x; x++)
		{
			v_add.x = v.x + x;
			for (int z = 0; z < 1 << node_slog.z; z++)
			{
				v_add.z = v.z + z;
				add_cube_from_node(v_add, type, node_ptr);
			}
		}
	}
	if (f_b[1])
	{
		v_add = v;
		for (int x = 0; x < 1 << node_slog.x; x++)
		{
			v_add.x = v.x + x;
			for (int z = 0; z < 1 << node_slog.z; z++)
			{
				v_add.z = v.z + z;
				add_cube_from_node(v_add, type, node_ptr);
			}
		}
	}
	if (f_b[2])
	{
		v_add = v;
		v_add.z = v.z + (1 << node_slog.z) - 1;
		for (int x = 0; x < 1 << node_slog.x; x++)
		{
			v_add.x = v.x + x;
			for (int y = f_b[1] ? 1 : 0; y < (1 << node_slog.y) - (f_b[0] ? 1 : 0); y++)
			{
				v_add.y = v.y + y;
				add_cube_from_node(v_add, type, node_ptr);
			}
		}
	}
	if (f_b[3])
	{
		v_add = v;
		for (int x = 0; x < 1 << node_slog.x; x++)
		{
			v_add.x = v.x + x;
			for (int y = f_b[1] ? 1 : 0; y < (1 << node_slog.y) - (f_b[0] ? 1 : 0); y++)
			{
				v_add.y = v.y + y;
				add_cube_from_node(v_add, type, node_ptr);
			}
		}
	}
	if (f_b[4])
	{
		v_add = v;
		v_add.x = v.x + (1 << node_slog.x) - 1;
		for (int z = f_b[3] ? 1 : 0; z < (1 << node_slog.z) - (f_b[2] ? 1 : 0); z++)
		{
			v_add.z = v.z + z;
			for (int y = f_b[1] ? 1 : 0; y < (1 << node_slog.y) - (f_b[0] ? 1 : 0); y++)
			{
				v_add.y = v.y + y;
				add_cube_from_node(v_add, type, node_ptr);
			}
		}
	}
	if (f_b[5])
	{
		v_add = v;
		for (int z = f_b[3] ? 1 : 0; z < (1 << node_slog.z) - (f_b[2] ? 1 : 0); z++)
		{
			v_add.z = v.z + z;
			for (int y = f_b[1] ? 1 : 0; y < (1 << node_slog.y) - (f_b[0] ? 1 : 0); y++)
			{
				v_add.y = v.y + y;
				add_cube_from_node(v_add, type, node_ptr);
			}
		}
	}
}

template <class Vertex>
void	Mesh<Vertex>::add_cube_from_node(s_vec3i v, uint32_t type, void *node_ptr)
{
	std::bitset<8>	v_b;
	uint32_t		v_idx[8];
	Vertex			vertex(glm::vec3((float)v.x, (float)v.y, (float)v.z)
						, type);
	Node_v			*node = (Node_v*)node_ptr;
	bool			is_opaque = block_is_opaque(type);

	m_moore_access.find_neigh(v, node);
	get_needed_vertex(v_b);
	add_needed_vertex(vertex, 1, v_b, v_idx); // weird
	if (!m_moore_access[MOORE_UP])
	{
		if (vertex_buffer[v_idx[1]].ao + vertex_buffer[v_idx[3]].ao
			> vertex_buffer[v_idx[2]].ao + vertex_buffer[v_idx[0]].ao)
		{
			add_index(v_idx[1], is_opaque);
			add_index(v_idx[0], is_opaque);
			add_index(v_idx[3], is_opaque);

			add_index(v_idx[3], is_opaque);
			add_index(v_idx[2], is_opaque);
			add_index(v_idx[1], is_opaque);
		}
		else
		{
			add_index(v_idx[2], is_opaque);
			add_index(v_idx[1], is_opaque);
			add_index(v_idx[0], is_opaque);

			add_index(v_idx[0], is_opaque);
			add_index(v_idx[3], is_opaque);
			add_index(v_idx[2], is_opaque);
		}
	}
	if (!m_moore_access[MOORE_DOWN])
	{
		if (vertex_buffer[v_idx[7]].ao + vertex_buffer[v_idx[5]].ao
			> vertex_buffer[v_idx[4]].ao + vertex_buffer[v_idx[6]].ao)
		{
			add_index(v_idx[7], is_opaque);
			add_index(v_idx[4], is_opaque);
			add_index(v_idx[5], is_opaque);

			add_index(v_idx[5], is_opaque);
			add_index(v_idx[6], is_opaque);
			add_index(v_idx[7], is_opaque);
		}
		else
		{
			add_index(v_idx[4], is_opaque);
			add_index(v_idx[5], is_opaque);
			add_index(v_idx[6], is_opaque);

			add_index(v_idx[6], is_opaque);
			add_index(v_idx[7], is_opaque);
			add_index(v_idx[4], is_opaque);
		}
	}
	if (!m_moore_access[MOORE_FRONT])
	{
		if (vertex_buffer[v_idx[4]].ao + vertex_buffer[v_idx[1]].ao
			> vertex_buffer[v_idx[0]].ao + vertex_buffer[v_idx[5]].ao)
		{
			add_index(v_idx[4], is_opaque);
			add_index(v_idx[0], is_opaque);
			add_index(v_idx[1], is_opaque);

			add_index(v_idx[1], is_opaque);
			add_index(v_idx[5], is_opaque);
			add_index(v_idx[4], is_opaque);
		}
		else
		{
			add_index(v_idx[0], is_opaque);
			add_index(v_idx[1], is_opaque);
			add_index(v_idx[5], is_opaque);

			add_index(v_idx[5], is_opaque);
			add_index(v_idx[4], is_opaque);
			add_index(v_idx[0], is_opaque);
		}
	}
	if (!m_moore_access[MOORE_BACK])
	{
		if (vertex_buffer[v_idx[7]].ao + vertex_buffer[v_idx[2]].ao
			> vertex_buffer[v_idx[6]].ao + vertex_buffer[v_idx[3]].ao)
		{
			add_index(v_idx[7], is_opaque);
			add_index(v_idx[6], is_opaque);
			add_index(v_idx[2], is_opaque);

			add_index(v_idx[2], is_opaque);
			add_index(v_idx[3], is_opaque);
			add_index(v_idx[7], is_opaque);
		}
		else
		{
			add_index(v_idx[6], is_opaque);
			add_index(v_idx[2], is_opaque);
			add_index(v_idx[3], is_opaque);

			add_index(v_idx[3], is_opaque);
			add_index(v_idx[7], is_opaque);
			add_index(v_idx[6], is_opaque);
		}
	}
	if (!m_moore_access[MOORE_RIGHT])
	{
		if (vertex_buffer[v_idx[6]].ao + vertex_buffer[v_idx[1]].ao
			> vertex_buffer[v_idx[5]].ao + vertex_buffer[v_idx[2]].ao)
		{
			add_index(v_idx[6], is_opaque);
			add_index(v_idx[5], is_opaque);
			add_index(v_idx[1], is_opaque);

			add_index(v_idx[1], is_opaque);
			add_index(v_idx[2], is_opaque);
			add_index(v_idx[6], is_opaque);
		}
		else
		{
			add_index(v_idx[5], is_opaque);
			add_index(v_idx[1], is_opaque);
			add_index(v_idx[2], is_opaque);

			add_index(v_idx[2], is_opaque);
			add_index(v_idx[6], is_opaque);
			add_index(v_idx[5], is_opaque);
		}
	}
	if (!m_moore_access[MOORE_LEFT])
	{
		if (vertex_buffer[v_idx[4]].ao + vertex_buffer[v_idx[3]].ao
			> vertex_buffer[v_idx[7]].ao + vertex_buffer[v_idx[0]].ao)
		{
			add_index(v_idx[4], is_opaque);
			add_index(v_idx[7], is_opaque);
			add_index(v_idx[3], is_opaque);

			add_index(v_idx[3], is_opaque);
			add_index(v_idx[0], is_opaque);
			add_index(v_idx[4], is_opaque);
		}
		else
		{
			add_index(v_idx[7], is_opaque);
			add_index(v_idx[3], is_opaque);
			add_index(v_idx[0], is_opaque);

			add_index(v_idx[0], is_opaque);
			add_index(v_idx[4], is_opaque);
			add_index(v_idx[7], is_opaque);
		}
	}
}

template <class Vertex>
uint8_t		Mesh<Vertex>::get_ao(int idx)
{
	switch (idx)
	{
		case 0:	
			return (vertex_ao(m_moore_access[10], m_moore_access[9]
					, m_moore_access[12]));
		case 1:	
			return (vertex_ao(m_moore_access[10], m_moore_access[11]
					, m_moore_access[14]));
		case 2:	
			return (vertex_ao(m_moore_access[16], m_moore_access[17]
					, m_moore_access[14]));
		case 3:	
			return (vertex_ao(m_moore_access[16], m_moore_access[15]
					, m_moore_access[12]));
		case 4:	
			return (vertex_ao(m_moore_access[21], m_moore_access[19]
					, m_moore_access[18]));
		case 5:	
			return (vertex_ao(m_moore_access[19], m_moore_access[23]
					, m_moore_access[20]));
		case 6:	
			return (vertex_ao(m_moore_access[25], m_moore_access[23]
					, m_moore_access[26]));
		case 7:	
			return (vertex_ao(m_moore_access[25], m_moore_access[21]
					, m_moore_access[24]));
		default:
			return (3);
	}
}

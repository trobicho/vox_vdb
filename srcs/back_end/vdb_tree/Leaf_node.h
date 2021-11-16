/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Leaf_node.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trobicho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/02 20:38:57 by trobicho          #+#    #+#             */
/*   Updated: 2020/06/27 21:42:44 by trobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <cstdint>
#include <bitset>
#include "Block.h"
#include "Node.h"

template <class Value, int Log2X, int Log2Y = Log2X, int Log2Z = Log2Y>
class Leaf_node: public Node<Value>
{
	public:
		Leaf_node(int32_t x, int32_t y, int32_t z, int value = 0);
		~Leaf_node(){};

		void		do_set_vox(Value v, int32_t x, int32_t y, int32_t z);
		void		do_unset_vox(int32_t x, int32_t y, int32_t z);
		Value		do_get_vox(int32_t x, int32_t y, int32_t z) const;
		Value		do_pruning();
		int			do_remove_node_by_slog(s_vec3i node_pos, uint32_t slog);
		const Node<Value>
					*do_get_interresting_node(s_vec3i v, Value &value) const;
		void		do_mesh(Mesh_interface &mesh) const;
		void		do_mesh(Mesh_interface &mesh, const s_vbox &b) const {do_mesh(mesh);}

		static const int sSize = 1 << (Log2X + Log2Y + Log2Z);
		static const int sLog2X = Log2X, sLog2Y = Log2Y, sLog2Z = Log2Z;

	private:
		inline bool			do_is_leaf() const {
			return (true);
		}
		inline s_vec3i		do_get_log() const {
			return (s_vec3i(Log2X, Log2Y, Log2Z));
		}
		inline s_vec3i		do_get_slog() const {
			return (s_vec3i(sLog2X, sLog2Y, sLog2Z));
		}
		inline s_vec3i		do_get_child_slog() const {
			return (s_vec3i(0, 0, 0));
		}
		inline const int	do_get_sSize() const {
			return (sSize);
		}
		inline s_vec3i		get_pos_from_offset(unsigned int i) const
		{
			s_vec3i	v;

			uint32_t	x_of = (i >> (Log2Y + Log2Z));
			uint32_t	y_of = (i >> (Log2Z)) & ((1 << Log2Y) - 1);
			uint32_t	z_of = (i) & ((1 << Log2Z) - 1);
			v.x = m_x + x_of;
			v.y = m_y + y_of;
			v.z = m_z + z_of;
			return (v);
		}

		Value				m_leaf_data[sSize];	//direct access table

		std::bitset<sSize>	m_value_mask;		//active states
		//[BitMask<sSize>	mInsideMask];		//optional for LS
		//uint64_t			mFlags;				//64 bit flags
		int32_t				m_x, m_y, m_z;	//origin of node
};

template <class Value, int Log2X, int Log2Y, int Log2Z>
Leaf_node<Value, Log2X, Log2Y, Log2Z>
	::Leaf_node(int32_t x, int32_t y, int32_t z, int value): m_x(x), m_y(y), m_z(z)
{
	if (value > 0)
	{
		for (int i = 0; i < sSize; ++i)
		{
			m_value_mask.set(i);
			m_leaf_data[i] = value;
		}
	}
	else
		m_value_mask.reset();
}

template <class Value, int Log2X, int Log2Y, int Log2Z>
void	Leaf_node<Value, Log2X, Log2Y, Log2Z>
		::do_set_vox(Value value, int32_t x, int32_t y, int32_t z)
{
	unsigned int	leaf_offset = ((x & (1 << sLog2X)-1) << (Log2Y + Log2Z))
		+ ((y & (1 << sLog2Y)-1) << Log2Z) + (z & (1 << sLog2Z) - 1);

	m_leaf_data[leaf_offset] = value;
	m_value_mask.set(leaf_offset);
}

template <class Value, int Log2X, int Log2Y, int Log2Z>
void	Leaf_node<Value, Log2X, Log2Y, Log2Z>
		::do_unset_vox(int32_t x, int32_t y, int32_t z)
{
	unsigned int	leaf_offset = ((x & (1 << sLog2X)-1) << (Log2Y + Log2Z))
		+ ((y & (1 << sLog2Y)-1) << Log2Z) + (z & (1 << sLog2Z) - 1);

	m_leaf_data[leaf_offset] = 0;
	m_value_mask.reset(leaf_offset);
}

template <class Value, int Log2X, int Log2Y, int Log2Z>
Value	Leaf_node<Value, Log2X, Log2Y, Log2Z>
		::do_get_vox(int32_t x, int32_t y, int32_t z) const
{
	unsigned int	leaf_offset = ((x & (1 << sLog2X)-1) << (Log2Y + Log2Z))
		+ ((y & (1 << sLog2Y)-1) << Log2Z) + (z & (1 << sLog2Z) - 1);

	if (m_value_mask[leaf_offset])
		return (m_leaf_data[leaf_offset]);
	return (0);
}

template <class Value, int Log2X, int Log2Y, int Log2Z>
int		Leaf_node<Value, Log2X, Log2Y, Log2Z>
	::do_remove_node_by_slog(s_vec3i node_pos, uint32_t slog)
{
	if (slog >= sLog2X)
	{
		m_value_mask.reset();
		return (sLog2X);
	}
	unsigned int	leaf_offset =
		((node_pos.x & (1 << sLog2X) - 1) << (Log2Y + Log2Z))
		+ ((node_pos.y & (1 << sLog2Y) - 1) << Log2Z)
		+ (node_pos.z & (1 << sLog2Z) - 1);

	m_value_mask[leaf_offset] = false;
	return (1);
}

template <class Value, int Log2X, int Log2Y, int Log2Z>
const Node<Value>	*Leaf_node<Value, Log2X, Log2Y, Log2Z>
		::do_get_interresting_node(s_vec3i v, Value &value) const
{
	unsigned int	leaf_offset = ((v.x & (1 << sLog2X)-1) << (Log2Y + Log2Z))
		+ ((v.y & (1 << sLog2Y)-1) << Log2Z) + (v.z & (1 << sLog2Z) - 1);

	if (m_value_mask[leaf_offset])
		value = m_leaf_data[leaf_offset];
	return (this);
}

template <class Value, int Log2X, int Log2Y, int Log2Z>
Value		Leaf_node<Value, Log2X, Log2Y, Log2Z>
	::do_pruning()
{
	Value	val;

	if (m_value_mask.all())
	{
		for (int i = 0; i < sSize; ++i)
		{
			if (m_leaf_data[i] != m_leaf_data[0])
				return (0);
		}
		return (m_leaf_data[0]);
	}
	return (0);
}

template <class Value, int Log2X, int Log2Y, int Log2Z>
void		Leaf_node<Value, Log2X, Log2Y, Log2Z>
	::do_mesh(Mesh_interface &mesh) const
{
	for (int i = 0; i < sSize; i++)
	{
		if (m_value_mask[i]) 
		{
			mesh.add_cube_from_node(get_pos_from_offset(i)
					, m_leaf_data[i], (void*)this);
		}
	}
}

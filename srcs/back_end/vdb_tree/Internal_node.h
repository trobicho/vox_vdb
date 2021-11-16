/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Internal_node.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trobicho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/02 20:38:22 by trobicho          #+#    #+#             */
/*   Updated: 2020/06/27 21:43:09 by trobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <cstdint>
#include <bitset>
#include <iostream>
#include "Block.h"
#include "Node.h"

template <class Value, class Child
	, int Log2X, int Log2Y = Log2X, int Log2Z = Log2Y>
class Internal_node: public Node<Value>
{
	public:
		Internal_node(int32_t x, int32_t y, int32_t z, int value = 0);
		~Internal_node();

		void		do_set_vox(Value v, int32_t x, int32_t y, int32_t z);
		void		do_unset_vox(int32_t x, int32_t y, int32_t z);
		Value		do_get_vox(int32_t x, int32_t y, int32_t z) const;
		Value		do_pruning();
		int			do_remove_node_by_slog(s_vec3i node_pos, uint32_t slog);
		const Node<Value>
					*do_get_interresting_node(s_vec3i v, Value &value) const;
		void		do_mesh(Mesh_interface &mesh) const;
		void		do_mesh(Mesh_interface &mesh, const s_vbox &box) const;

		static const int sLog2X = Log2X + Child::sLog2X,
			sLog2Y = Log2Y + Child::sLog2Y,
			sLog2Z = Log2Z + Child::sLog2Z,
			sSize = 1 << (Log2X + Log2Y + Log2Z);
	private:
		inline bool			do_is_leaf() const {
			return (false);
		}
		inline s_vec3i		do_get_log() const {
			return s_vec3i(Log2X, Log2Y, Log2Z);
		}
		inline s_vec3i		do_get_slog() const {
			return s_vec3i(sLog2X, sLog2Y, sLog2Z);
		}
		inline s_vec3i		do_get_child_slog() const {
			return s_vec3i(Child::sLog2X, Child::sLog2Y, Child::sLog2Z);
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
			v.x = m_x + (x_of << Child::sLog2X);
			v.y = m_y + (y_of << Child::sLog2Y);
			v.z = m_z + (z_of << Child::sLog2Z);
			return (v);
		}

		union u_internal_data 
		{
			Child*	child;					//child node pointer
			Value	value;					//tile value
		} m_internal_data[sSize];

		std::bitset<sSize>	m_value_mask;	//active states
		std::bitset<sSize>	m_child_mask;	//node topology
		int32_t				m_x, m_y, m_z;	//origin of node
};

template <class Value, class Child, int Log2X, int Log2Y, int Log2Z>
Internal_node<Value, Child, Log2X, Log2Y, Log2Z>
	::Internal_node(int32_t x, int32_t y, int32_t z, int value): m_x(x), m_y(y), m_z(z)
{
	if (value > 0)
	{
		for (int i = 0; i < sSize; ++i)
		{
			m_value_mask.set(i);
			m_internal_data[i].value = value;
		}
	}
	else
		m_value_mask.reset();
	m_child_mask.reset();
	for (int i = 0; i < sSize; ++i)
	{
		m_internal_data[i].child = nullptr;
	}
}

template <class Value, class Child, int Log2X, int Log2Y, int Log2Z>
Internal_node<Value, Child, Log2X, Log2Y, Log2Z>::~Internal_node()
{
	for (int i = 0; i < sSize; ++i)
	{
		if (m_child_mask[i])
			delete m_internal_data[i].child;
		m_internal_data[i].child = nullptr;
	}
}

template <class Value, class Child, int Log2X, int Log2Y, int Log2Z>
void	Internal_node<Value, Child, Log2X, Log2Y, Log2Z>
		::do_set_vox(Value value, int32_t x, int32_t y, int32_t z)
{
	unsigned int	internal_offset =
		(((x & (1 << sLog2X) - 1) >> Child::sLog2X) << (Log2Y + Log2Z))
		+ (((y & (1 << sLog2Y) - 1) >> Child::sLog2Y) << Log2Z)
		+ ((z & (1 << sLog2Z) - 1) >> Child::sLog2Z);

	if (m_child_mask[internal_offset])
		m_internal_data[internal_offset].child->set_vox(value, x, y, z);
	else
	{
		m_internal_data[internal_offset].child = new Child(
			(x >> Child::sLog2X) << Child::sLog2X
			, (y >> Child::sLog2Y) << Child::sLog2Y
			, (z >> Child::sLog2Z) << Child::sLog2Z);
		m_internal_data[internal_offset].child->set_vox(value, x, y, z);
		m_child_mask.set(internal_offset);
	}
}

template <class Value, class Child, int Log2X, int Log2Y, int Log2Z>
void	Internal_node<Value, Child, Log2X, Log2Y, Log2Z>
		::do_unset_vox(int32_t x, int32_t y, int32_t z)
{
	unsigned int	internal_offset =
		(((x & (1 << sLog2X) - 1) >> Child::sLog2X) << (Log2Y + Log2Z))
		+ (((y & (1 << sLog2Y) - 1) >> Child::sLog2Y) << Log2Z)
		+ ((z & (1 << sLog2Z) - 1) >> Child::sLog2Z);

	if (m_child_mask[internal_offset])
		m_internal_data[internal_offset].child->unset_vox(x, y, z);
	else if (m_value_mask[internal_offset])
	{
		m_internal_data[internal_offset].child = new Child(
			(x >> Child::sLog2X) << Child::sLog2X
			, (y >> Child::sLog2Y) << Child::sLog2Y
			, (z >> Child::sLog2Z) << Child::sLog2Z
			, m_internal_data[internal_offset].value);
		m_child_mask[internal_offset] = true;
		m_value_mask[internal_offset] = false;
		m_internal_data[internal_offset].child->unset_vox(x, y, z);
	}
}

template <class Value, class Child, int Log2X, int Log2Y, int Log2Z>
Value	Internal_node<Value, Child, Log2X, Log2Y, Log2Z>
	::do_get_vox(int32_t x, int32_t y, int32_t z) const
{
	unsigned int	internal_offset =
		(((x & (1 << sLog2X) - 1) >> Child::sLog2X) << (Log2Y + Log2Z))
		+ (((y & (1 << sLog2Y) - 1) >> Child::sLog2Y) << Log2Z)
		+ ((z & (1 << sLog2Z) - 1) >> Child::sLog2Z);

	if (m_value_mask[internal_offset])
		return (m_internal_data[internal_offset].value);
	else if (m_child_mask[internal_offset])
		return (m_internal_data[internal_offset].child->get_vox(x, y, z));
	return (0);
}

template <class Value, class Child, int Log2X, int Log2Y, int Log2Z>
int		Internal_node<Value, Child, Log2X, Log2Y, Log2Z>
	::do_remove_node_by_slog(s_vec3i node_pos, uint32_t slog)
{
	
	if (slog >= sLog2X)
	{
		m_child_mask.reset();
		m_value_mask.reset();
		return (sLog2X);
	}
	unsigned int	internal_offset =
		(((node_pos.x & (1 << sLog2X) - 1) >> Child::sLog2X) << (Log2Y + Log2Z))
		+ (((node_pos.y & (1 << sLog2Y) - 1) >> Child::sLog2Y) << Log2Z)
		+ ((node_pos.z & (1 << sLog2Z) - 1) >> Child::sLog2Z);

	if (m_value_mask[internal_offset])
	{
		m_value_mask[internal_offset] = false;
		return (m_internal_data[internal_offset].value);
	}
	else if (m_child_mask[internal_offset])
		return (m_internal_data[internal_offset].child
				->do_remove_node_by_slog(node_pos, slog));
	return (0);
}

template <class Value, class Child, int Log2X, int Log2Y, int Log2Z>
const Node<Value>	*Internal_node<Value, Child, Log2X, Log2Y, Log2Z>
	::do_get_interresting_node(s_vec3i v, Value &value) const
{
	unsigned int	internal_offset =
		(((v.x & (1 << sLog2X) - 1) >> Child::sLog2X) << (Log2Y + Log2Z))
		+ (((v.y & (1 << sLog2Y) - 1) >> Child::sLog2Y) << Log2Z)
		+ ((v.z & (1 << sLog2Z) - 1) >> Child::sLog2Z);

	if (m_value_mask[internal_offset])
	{
		value = m_internal_data[internal_offset].value;
		return (this);
	}
	else if (m_child_mask[internal_offset])
		return (m_internal_data[internal_offset].child
			->get_interresting_node(v, value));
	return (this);
}

template <class Value, class Child, int Log2X, int Log2Y, int Log2Z>
Value	Internal_node<Value, Child, Log2X, Log2Y, Log2Z>::do_pruning()
{
	Value	val;

	for (int i = 0; i < sSize; i++)
	{
		if (m_child_mask[i])
		{
			if ((val = m_internal_data[i].child->do_pruning()) != 0)
			{
				m_child_mask.set(i, false);
				m_value_mask.set(i, true);
				delete(m_internal_data[i].child);
				m_internal_data[i].value = val;
			}
		}
	}
	if (m_value_mask.all())
	{
		for (int i = 0; i < sSize; ++i)
		{
			if (m_internal_data[i].value != m_internal_data[0].value)
				return (0);
		}
		return (m_internal_data[0].value);
	}
	return (0);
}

template <class Value, class Child, int Log2X, int Log2Y, int Log2Z>
void	Internal_node<Value, Child, Log2X, Log2Y, Log2Z>
	::do_mesh(Mesh_interface &mesh) const
{
	for (int i = 0; i < sSize; i++)
	{
		if (m_child_mask[i])
		{
			m_internal_data[i].child->do_mesh(mesh);
		}
		else if (m_value_mask[i]) 
		{
			mesh.add_cube_from_node(get_pos_from_offset(i)
				, m_internal_data[i].value, (void*)this);
		}
	}
}
		
template <class Value, class Child, int Log2X, int Log2Y, int Log2Z>
void	Internal_node<Value, Child, Log2X, Log2Y, Log2Z>
	::do_mesh(Mesh_interface &mesh, const s_vbox &box) const
{
	unsigned int	x_off = (box.origin.x & (1 << sLog2X) - 1) >> Child::sLog2X;
	unsigned int	y_off;
	unsigned int	z_off;
	unsigned int	off;
	
	for (; x_off <= ((box.origin.x + box.len.x - 1) 
		& (1 << sLog2X) - 1) >> Child::sLog2X; x_off++)
	{
		off = x_off << (Log2Y + Log2Z);
		if(off >= sSize)
			break;
		z_off = (box.origin.z & (1 << sLog2Z) - 1) >> Child::sLog2Z;
		for (; z_off <= ((box.origin.z + box.len.z - 1)
			& (1 << sLog2Z) - 1) >> Child::sLog2Z; z_off++)
		{
			y_off = (box.origin.y & (1 << sLog2Y) - 1) >> Child::sLog2Y;
			off = (x_off << (Log2Y + Log2Z)) + (y_off << Log2Z) + z_off;
			if (off >= sSize)
				break ;
			for (; y_off <= ((box.origin.y + box.len.y - 1) 
				& (1 << sLog2Y) - 1) >> Child::sLog2Y; y_off++)
			{
				off = (x_off << (Log2Y + Log2Z)) + (y_off << Log2Z) + z_off;
				if (off >= sSize)
					break ;
				if (m_child_mask[off]) 
				{
					m_internal_data[off].child->do_mesh(mesh, box);
				}
				else if (m_value_mask[off]) 
				{
					mesh.add_big_cube_from_node(get_pos_from_offset(off)
							, m_internal_data[off].value
							, (void*)this);
				}
			}
		}
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mesh_node.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trobicho <trobicho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/17 14:50:23 by trobicho          #+#    #+#             */
/*   Updated: 2021/11/17 15:34:22 by trobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Node.h"

template <class Value>
class	Mesh_node: public Node<Value>
{
	public:
		Mesh_node(){};
		void		mesh(Mesh_interface &mesh) const;
		void		mesh(Mesh_interface &mesh, const s_vbox &box) const;

	private:
		virtual void		do_mesh(Mesh_interface &mesh) const = 0;
		virtual void		do_mesh(Mesh_interface &mesh, const s_vbox &box) const = 0;
};

template <class Value, class Child
	, int Log2X, int Log2Y = Log2X, int Log2Z = Log2Y>
class	Mesh_internal_node
{
	public:
		Mesh_internal_node(int32_t x, int32_t y, int32_t z, int value = 0):
			Internal_node(int32_t x, int32_t y, int32_t z, int value = 0);
};

template <class Value, class Child, int Log2X, int Log2Y, int Log2Z>
void	Mesh_internal_node<Value, Child, Log2X, Log2Y, Log2Z>
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
void	Mesh_internal_node<Value, Child, Log2X, Log2Y, Log2Z>
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

template <class Value, int Log2X, int Log2Y = Log2X, int Log2Z = Log2Y>
class	Mesh_leaf_node: public Leaf<Value, Log2X, Log2Y, Log2Z>
{
	public:
		Mesh_leaf_node(int32_t x, int32_t y, int32_t z, int value = 0):
			Leaf_node(x, y, z, value);
		~Mesh_leaf_node(){};

	private:
		void		do_mesh(Mesh_interface &mesh) const;
		void		do_mesh(Mesh_interface &mesh, const s_vbox &b) const {do_mesh(mesh);}
};

template <class Value, int Log2X, int Log2Y, int Log2Z>
void		Mesh_leaf_node<Value, Log2X, Log2Y, Log2Z>
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

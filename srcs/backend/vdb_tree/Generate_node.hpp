/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Generate_node.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trobicho <trobicho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/17 15:07:04 by trobicho          #+#    #+#             */
/*   Updated: 2021/11/18 17:36:03 by trobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Node.h"
#include "Internal_node.h"
#include "Leaf_node.h"

template <class Value, class Child
	, int Log2X, int Log2Y = Log2X, int Log2Z = Log2Y>
class	Generate_pre_chunk_node : public Internal_node<Value, Child
			, Log2X, Log2Y, Log2Z>
{
	public:
		Generate_pre_chunk_node(int32_t x, int32_t y, int32_t z, int value = 0):
			Internal_node(int32_t x, int32_t y, int32_t z, int value = 0);

	private:
		virtual void		do_init_chunk_generation(s_vec3i chunk) const = 0;
};

template <class Value, class Child, int Log2X, int Log2Y, int Log2Z>
void	Generate_pre_chunk_node<Value, Child, Log2X, Log2Y, Log2Z>
	::do_init_chunk_generation(s_vec3i chunk) const
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
		m_child_mask.set(internal_offset);
		Generate_pre_chunk_node	*ptr;
		if ((Generate_pre_chunk_node *ptr = dynamic_cast<Generate_pre_chunk_node>
				m_internal_data[internal_offset].child))
			m_internal_data[internal_offset]->do_init_chunk_generation(chunk);
	}
}

template <class Value, class Child
	, int Log2X, int Log2Y = Log2X, int Log2Z = Log2Y>
class	Generate_chunk_node : public Internal_node<Value, Child
			, Log2X, Log2Y, Log2Z>
{
	public:
		Generate_internal_node(int32_t x, int32_t y, int32_t z, int value = 0):
			Internal_node(int32_t x, int32_t y, int32_t z, int value = 0);

	private:
		virtual void		do_generate_one_chunk(Generate_interface &generate,
			, s_vec3i chunk) const = 0;
};

template <class Value, class Child, int Log2X, int Log2Y, int Log2Z>
void	Generate_chunk_node<Value, Child, Log2X, Log2Y, Log2Z>
	::do_generate_one_chunk(Generate_interface &generate) const
{
	for (int i = 0; i < sSize; i++)
	{
		if (m_child_mask[i])
		{
			m_internal_data[i].child->do_generate(generate);
		}
		else if (m_value_mask[i]) 
		{
			generate.add_cube_from_node(get_pos_from_offset(i)
				, m_internal_data[i].value, (void*)this);
		}
	}
}

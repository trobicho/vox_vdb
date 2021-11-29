/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Generate_node.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trobicho <trobicho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/17 15:07:04 by trobicho          #+#    #+#             */
/*   Updated: 2021/11/28 14:28:37 by trobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Node.h"
#include "Internal_node.h"
#include "Leaf_node.h"
#include "backend/front_interfaces/Map_sampler.hpp"

class	Generate_node
{
	public:
		Generate_node(){};

		inline void						generate_one_chunk(Map_sampler &map_sampler, s_vec3i chunk)
		{
			do_generate_one_chunk(map_sampler, chunk);
		}
		inline Generate_node*	init_chunk_generation(s_vec3i chunk)
		{
			return (do_init_chunk_generation(chunk));
		}

	private:
		virtual void						do_generate_one_chunk(Map_sampler &map_sampler, s_vec3i chunk) = 0;
		virtual Generate_node*	do_init_chunk_generation(s_vec3i chunk) = 0;
};

template <class Value, class Child
	, int Log2X, int Log2Y = Log2X, int Log2Z = Log2Y>
class	Generate_chunk_node: public Internal_node<Value, Child, Log2X, Log2Y, Log2Z>
	, public Generate_node 
{
	public:
		Generate_chunk_node(int32_t x, int32_t y, int32_t z, int value = 0):
			Internal_node<Value, Child, Log2X, Log2Y, Log2Z>(x, y, z, value){};

	protected:
		Generate_node*	do_init_chunk_generation(s_vec3i chunk){return (this);}
		void						do_generate_one_chunk(Map_sampler &map_sampler, s_vec3i chunk);
};

template <class Value, class Child, int Log2X, int Log2Y, int Log2Z>
void	Generate_chunk_node<Value, Child, Log2X, Log2Y, Log2Z>
	::do_generate_one_chunk(Map_sampler &map_sampler, s_vec3i chunk)
{
	Sampler	sampler(0, 0, 0);
	s_vec3i	pos(0, 0, 0);

	chunk.x = (chunk.x >> Child::sLog2X) << Child::sLog2X;
	chunk.z = (chunk.z >> Child::sLog2Z) << Child::sLog2Z;
	chunk.y = (chunk.y >> Child::sLog2Y) << Child::sLog2Y;
	chunk.y = 0;
	for (int x = 0; x < 1 << this->sLog2X; ++x)
	{
		pos.x = chunk.x + x;
		for (int z = 0; z < 1 << this->sLog2Z; ++z)
		{
			pos.z = chunk.z + z;
			s_slice_sample sample = map_sampler.get_slice_sample(sampler, pos);
			int height = (sample.height > sample.water_height)
				? sample.height : sample.water_height;
			for (int y = 0; y < (height  & ((1 << this->sLog2Y) - 1)); ++y)
			{
				pos.y = chunk.y + y;
				uint32_t value = 2;
				if (y < sample.height)
					value = map_sampler.get_block(sampler, sample, pos);
				uint32_t internal_offset = this->get_internal_offset(x, y, z);
				if (!this->m_child_mask[internal_offset])
				{
					this->m_internal_data[internal_offset].child = new Child(
						(x >> Child::sLog2X) << Child::sLog2X
						, (y >> Child::sLog2Y) << Child::sLog2Y
						, (z >> Child::sLog2Z) << Child::sLog2Z);
					this->m_child_mask.set(internal_offset);
				}
				this->m_internal_data[internal_offset].child
					->set_vox(value, pos.x, pos.y, pos.z);
				if (this->m_internal_data[internal_offset].child
					->get_vox(pos.x, pos.y, pos.z) != value)
						std::cout << "houlla" << std::endl;
			}
		}
	}
}

template <class Value, class Child
	, int Log2X, int Log2Y = Log2X, int Log2Z = Log2Y>
class	Generate_pre_chunk_node:
	public Internal_node<Value, Child, Log2X, Log2Y, Log2Z>
	, public Generate_node
{
	public:
		Generate_pre_chunk_node(int32_t x, int32_t y, int32_t z, int value = 0):
			Internal_node<Value, Child, Log2X, Log2Y, Log2Z>(x, y, z, value){};

	protected:
		Generate_node*	do_init_chunk_generation(s_vec3i chunk);
		void						do_generate_one_chunk(Map_sampler &map_sampler, s_vec3i chunk);
};

template <class Value, class Child, int Log2X, int Log2Y, int Log2Z>
Generate_node*	Generate_pre_chunk_node<Value, Child, Log2X, Log2Y, Log2Z>
	::do_init_chunk_generation(s_vec3i chunk)
{
	unsigned int	internal_offset = this->get_internal_offset(chunk.x, chunk.y, chunk.z);

	if (this->m_child_mask[internal_offset])
		return (this->m_internal_data[internal_offset].child->init_chunk_generation(chunk));
	else
	{
		this->m_internal_data[internal_offset].child = new Child(
			(chunk.x >> Child::sLog2X) << Child::sLog2X
			, (chunk.y >> Child::sLog2Y) << Child::sLog2Y
			, (chunk.z >> Child::sLog2Z) << Child::sLog2Z);
		this->m_child_mask.set(internal_offset);
	}
	return (this->m_internal_data[internal_offset].child->init_chunk_generation(chunk));
}

template <class Value, class Child, int Log2X, int Log2Y, int Log2Z>
void	Generate_pre_chunk_node<Value, Child, Log2X, Log2Y, Log2Z>
	::do_generate_one_chunk(Map_sampler &map_sampler, s_vec3i chunk)
{
	unsigned int	internal_offset = this->get_internal_offset(chunk.x, chunk.y, chunk.z);

	if (this->m_child_mask[internal_offset])
		this->m_internal_data[internal_offset].child->generate_one_chunk(map_sampler, chunk);
}

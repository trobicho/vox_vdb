/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Map_sampler.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trobicho <trobicho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/19 16:51:49 by trobicho          #+#    #+#             */
/*   Updated: 2021/11/22 18:24:42 by trobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "backend/vec.hpp"

using		Block_type = uint32_t;

struct	s_biome_info
{
	double	precipitation;
	double	temperature;
	int			type;
};

struct	s_slice_sample
{
	int						height;
	int						water_height;
	s_biome_info	biome_info;
};

struct Sampler
{
	Sampler(){};
	Sampler(int32_t x, int32_t y, int32_t z):
		xlog(x), ylog(y), zlog(z){};
	int32_t	xlog = 0;
	int32_t	ylog = 0;
	int32_t	zlog = 0;
};

class	Map_sampler
{
	public:
		Map_sampler(){};
		~Map_sampler(){};

		inline s_slice_sample	get_slice_sample(const Sampler &sampler, s_vec3i pos)
		{
			return (do_get_slice_sample(sampler, pos));
		}
		inline Block_type			get_block(const Sampler &sampler, s_slice_sample &slice_sample
				, s_vec3i pos)
		{
			return (do_get_block(sampler, slice_sample, pos));
		}
		inline glm::vec4			get_color_from_block_type(Block_type block) const
		{
			return (do_get_color_from_block_type(block));
		}
		inline bool						block_is_opaque(Block_type block) const
		{
			return (do_block_is_opaque(block));
		}

	private:
		virtual s_slice_sample	do_get_slice_sample(const Sampler &sampler, s_vec3i pos) = 0;
		virtual Block_type			do_get_block(const Sampler &sampler
															, s_slice_sample &slice_sample, s_vec3i pos) = 0;
		virtual glm::vec4				do_get_color_from_block_type(Block_type block) const = 0;
		virtual bool						do_block_is_opaque(Block_type block) const = 0;
};

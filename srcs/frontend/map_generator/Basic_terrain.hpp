/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Basic_terrain.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trobicho <trobicho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/20 08:22:11 by trobicho          #+#    #+#             */
/*   Updated: 2021/11/22 12:43:37 by trobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "frontend/noise/perlin.h"
#include "backend/front_interfaces/Map_sampler.hpp"

#define WATER_HEIGHT	64
#define	CLOUD_HEIGHT	180
#define	WORLD_HEIGHT	256

enum e_biome_type
{
	bt_tundra,
	bt_borreal_forest,
	bt_grassland,
	bt_shrubland,
	bt_temperate_forest,
	bt_temperate_rainforest,
	bt_subtropical_desert,
	bt_savanna,
	bt_tropical_rainforest,
};

enum	e_block_type
{
	bl_air = 0,
	bl_stone = 1,
	bl_bedrock,
	bl_dirt,
	bl_water,
	bl_grass,
	bl_ice,
	bl_sand,
	bl_wood,
	bl_leaf,
};

class	Basic_terrain: public Map_sampler
{
	public:
		Basic_terrain(uint32_t seed): m_seed(seed)
		{
			m_noise.shuffle();
		}
		Basic_terrain(){};

		s_slice_sample	do_get_slice_sample(const Sampler &sampler, s_vec3i pos);
		Block_type			do_get_block(const Sampler &sampler
				, s_slice_sample &slice_sample, s_vec3i pos);
		glm::vec4				do_get_color_from_block_type(Block_type block) const;
		bool						do_block_is_opaque(Block_type block) const
		{
			switch ((e_block_type)block)
			{
				case bl_water:
					return (false);
				default:
					return (true);
			}
		}

	private:
		double					get_height(const Sampler &sampler, glm::vec3 pos);
		void						get_biome_info(s_biome_info &biome_info, glm::vec3 pos);
		e_biome_type		get_biome_type(s_biome_info	&biome_info) const;
		double					get_density_cave(glm::vec3 pos);
		uint32_t				get_block_type(s_biome_info &biome_info, int y) const;

		uint32_t				m_seed;
		Perlin_noiser		m_noise;
};

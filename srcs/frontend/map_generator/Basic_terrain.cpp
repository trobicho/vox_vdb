/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Basic_terrain.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trobicho <trobicho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/20 08:44:49 by trobicho          #+#    #+#             */
/*   Updated: 2021/11/29 15:39:43 by trobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Basic_terrain.hpp"
#include <iostream>

/*
double	Map::combine_test(double x, double z)
{
	double	scalar_n1 = 5000.0;
	double	scalar_n2 = 1000.0;
	double	n1 = m_noise.perlin2d(8, 1., 0.6
				, (double)x / scalar_n1
				, (double)z / scalar_n1);
	scalar_n2 /= (n1 + 0.1) * 10;
	double	n2 = m_noise.perlin2d(8, 1., 0.5
				, (double)x / scalar_n2
				, (double)z / scalar_n2);
	return (n2);
}
*/

s_slice_sample	Basic_terrain::do_get_slice_sample(
	const Sampler &sampler, s_vec3i	pos)
{
	s_slice_sample	slice_sample;
	glm::vec3			pos_glm = pos.glm_vec3f();

	slice_sample.height = (int)get_height(sampler, pos_glm);
	slice_sample.water_height = WATER_HEIGHT;
	get_biome_info(slice_sample.biome_info, pos_glm);
	return (slice_sample);
}

double				Basic_terrain::get_height(const Sampler &sampler, glm::vec3 pos)
{
	double				scalar_elev = 2000.0;
	double				scalar_height = 500.0;

	struct	s_perlin_params
	{
		s_perlin_params(){};
		s_perlin_params(int p_octave, double p_freq, double p_persi
			, double p_scalar, double p_bias = 0.0):
				octave(p_octave), freq(p_freq), persistence(p_persi)
				, scalar(p_scalar), bias(p_bias){};
		int			octave;
		double	freq;
		double	persistence;
		double	scalar;
		double	bias;
	};
	s_perlin_params	pe, ph;

	pe = s_perlin_params(4, 4.0, 0.8, 1500.0, -0.15);
	ph = s_perlin_params(4, 4.0, 0.6, 400.0, 0.0);
	double	d_e = m_noise.perlin2d(pe.octave, pe.freq, pe.persistence
				, (double)pos.x / pe.scalar
				, (double)pos.z / pe.scalar) + pe.bias;
	double	d_h = m_noise.perlin2d(ph.octave, ph.freq, ph.persistence
			, (double)pos.z /	ph.scalar 
			, (double)pos.x / ph.scalar) + pe.bias;
	double	height = (d_e * WATER_HEIGHT)
					+ (d_h * (CLOUD_HEIGHT - WATER_HEIGHT)) + 1.0;
	return (height);
	//return (120);
}

double		Basic_terrain::get_density_cave(glm::vec3 pos)
{
	double	scalar_cave = 75.0;
	double	d = m_noise.perlin3d(5, 1., 0.6
			, (double)pos.x / scalar_cave
			, (double)pos.z / scalar_cave
			, (double)pos.y / scalar_cave);
	return (d);
}

void				Basic_terrain::get_biome_info(
	s_biome_info &biome_info, glm::vec3 pos)
{
	double	scalar_temp = 10000.0;
	double	scalar_prec = 5000.0;

	biome_info.temperature = m_noise.perlin2d(3, 1.5, 0.8
			, (double)pos.x / scalar_temp
			, (double)pos.z / scalar_temp);
	biome_info.precipitation = m_noise.perlin2d(5, 2., 0.8
			, (double)(pos.x + 10000) / scalar_prec
			, (double)pos.z / scalar_prec);
	biome_info.type = get_biome_type(biome_info);
}

uint32_t		Basic_terrain::get_block_type(s_biome_info &biome_info, int y) const
{
	double 				temp = biome_info.temperature - abs(y - WATER_HEIGHT) / 25;
	e_biome_type	biome_type = (e_biome_type)biome_info.type;

	if (biome_type == bt_tundra || temp < 0.3)
		return (bl_ice);
	if (y < WATER_HEIGHT)
		return (bl_stone);
	if (biome_type == bt_grassland
		|| biome_type == bt_shrubland || biome_type == bt_savanna)
		return (bl_sand);
	return (bl_stone);
}

e_biome_type	Basic_terrain::get_biome_type(s_biome_info	&biome_info) const
{
	e_biome_type	type;

	if (biome_info.temperature < 0.250)
		type = bt_tundra;
	else if (biome_info.temperature <= 0.45)
	{
		if (biome_info.precipitation > 0.3)
			type = bt_borreal_forest;
		else if (biome_info.precipitation < 0.2)
			type = bt_grassland;
		else
			type = bt_shrubland;
	}
	else if (biome_info.temperature < 0.75)
	{
		if (biome_info.precipitation > 0.8)
			type = bt_temperate_rainforest;
		else if (biome_info.precipitation > 0.45)
			type = bt_temperate_forest;
		else if (biome_info.precipitation < 0.1 && biome_info.temperature > 0.60)
			type = bt_subtropical_desert;
		else if (biome_info.precipitation < 0.22)
			type = bt_grassland;
		else
			type = bt_shrubland;
	}
	else
	{
		if (biome_info.precipitation < 0.2)
			type = bt_subtropical_desert;
		else if (biome_info.precipitation < 0.65)
			type = bt_savanna;
		else
			type = bt_tropical_rainforest;
	}
	return (type);
}

Block_type		Basic_terrain::do_get_block(const Sampler &sampler
	, s_slice_sample &slice_sample, s_vec3i pos) 
{
	Block_type		block_type = 1;
	s_biome_info	biome_info;
	double				d;

	if (pos.y == 0)
		block_type = bl_bedrock;
	else
		block_type = get_block_type(biome_info, pos.y);
	if (block_type == bl_dirt) // && pos.y >= box.len.y * d - 1)
		block_type = bl_grass;
	return (block_type);
}

glm::vec4			Basic_terrain::do_get_color_from_block_type(Block_type block) const
{
	switch ((e_block_type)block)
	{
		case 0:
			return (glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
			break;
		case bl_stone:
			return (glm::vec4(0.35f, 0.35f, 0.35f, 1.0f));
			break;
		case bl_bedrock:
			return (glm::vec4(0.5f, 0.137f, 0.027f, 1.0f));
			break;
		case bl_dirt:
			return (glm::vec4(0.48f, 0.35f, 0.25f, 1.0f));
			break;
		case bl_grass:
			return (glm::vec4(0.1f, 0.8f, 0.1f, 1.0f));
			break;
		case bl_water:
			return (glm::vec4(0.2f, 0.2f, 0.8f, 0.5f));
			break;
		case bl_ice:
			return (glm::vec4(0.65f, 0.65f, 0.8f, 1.0f));
			break;
		case bl_sand:
			return (glm::vec4(0.9f, 0.9f, 0.0f, 1.0f));
			break;
		case bl_wood:
			return (glm::vec4(0.48f, 0.35f, 0.25f, 1.0f));
			break;
		case bl_leaf:
			return (glm::vec4(0.1f, 0.8f, 0.1f, 1.0f));
			break;
		default:
			return (glm::vec4(0.7f, 0.1f, 1.0f, 1.0f));
			break;
	}
}

/*
int			Map::generate(Vdb_test &vdb, s_vbox box)
{
	double			cave_thres = 0.25;
	double			cave_thres_d;
	int				lerp_mod = 8;
	double			d_cave_prec, d_cave_next, d_cave;
	int				block_type = 1;
	s_biome_info	biome_info;
	double			d;
	int				y;

	for (int z = 0; z < box.len.z; ++z)
	{
		for (int x = 0; x < box.len.x; ++x)
		{
			get_biome_info(biome_info, x + box.origin.x, z + box.origin.z);
			int height = get_height(biome_info
							, x + box.origin.x, z + box.origin.z);
			d_cave = 1.0;
			for (y = 0; y < height; ++y)
			{
				s_vec3i	vox(x, y, z);
				if ((y - 1) % lerp_mod == 0)
				{
					d_cave = get_density_cave(x + box.origin.x
							, y + box.origin.y, z + box.origin.z);
					d_cave_prec = d_cave;
					d_cave_next = get_density_cave(x + box.origin.x
							, y + box.origin.y + lerp_mod, z + box.origin.z);
				}
				else if (y > 0)
				{
					d_cave = Noise::lerp(d_cave_prec, d_cave_next
								, ((y - 1) % lerp_mod) / (double)lerp_mod);
				}
				cave_thres_d = (1.0 - (double)y / box.len.y) - cave_thres;
				if (y > 0 && d_cave < cave_thres)
					continue ;
				if (y == 0)
					block_type = bl_bedrock;
				else
					block_type = get_block_type(biome_info, y);
				if (block_type == bl_dirt && y >= box.len.y * d - 1)
					block_type = bl_grass;
				vox.x += box.origin.x;
				vox.y += box.origin.y;
				vox.z += box.origin.z;
				vdb.set_vox(block_type, vox);
				m_nb_vox++;
			}
			for (; y <= WATER_HEIGHT; ++y)
			{
				s_vec3i	vox(x, y, z);
				vox.x += box.origin.x;
				vox.y += box.origin.y;
				vox.z += box.origin.z;
				vdb.set_vox((uint32_t)((e_block_type)bl_water), vox);
			}
		}
	}
	return (0);
}
*/

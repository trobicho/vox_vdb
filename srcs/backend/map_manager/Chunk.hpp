/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Chunk.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trobicho <trobicho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/16 06:05:04 by trobicho          #+#    #+#             */
/*   Updated: 2021/11/17 14:12:04 by trobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

class	Chunk
{
	public:
		Chunk(){};
		~Chunk(){};

		s_vec3i		origin;
		bool			need_remesh = false;
		bool			need_unload = false;
};

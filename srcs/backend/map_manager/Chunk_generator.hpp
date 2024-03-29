/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Chunk_generator.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trobicho <trobicho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/15 06:47:15 by trobicho          #+#    #+#             */
/*   Updated: 2021/11/19 16:29:07 by trobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

class	Chunk_generator
{
	public:
		virtual	Chunk_generator() = 0;

		virtual	void	generate_chunk() = 0;
}

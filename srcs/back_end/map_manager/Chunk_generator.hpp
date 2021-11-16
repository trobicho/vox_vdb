/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Chunk_generator.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trobicho <trobicho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/15 06:47:15 by trobicho          #+#    #+#             */
/*   Updated: 2021/11/15 08:24:00 by trobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

class	Chunk_generator
{
	public:
		virtual	Chunk_generator() = 0;

		virtual	void	generate_chunk();
}

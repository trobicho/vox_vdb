/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Chunk_map.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trobicho <trobicho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/19 09:58:10 by trobicho          #+#    #+#             */
/*   Updated: 2021/11/19 10:00:09 by trobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Chunk.hpp"
#include <unordered_map>

using		t_chunk_key = std::pair<uint32_t, uint32_t>;
struct	s_key_hash
{
	size_t operator()(const t_chunk_key& key) const
	{
		return (key.first | (key.second << 10));
	}
};
using		Chunk_map	= std::unordered_map<t_chunk_key, Chunk, s_key_hash>;

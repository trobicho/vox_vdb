/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Map_loader.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trobicho <trobicho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/17 14:35:28 by trobicho          #+#    #+#             */
/*   Updated: 2021/11/20 13:30:37 by trobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <thread>
#include <mutex>
#include <glm/glm.hpp>
#include "backend/chunk_map/Chunk_map.hpp"
#include "backend/chunk_manager/Chunk_manager.hpp"

#define		CHUNK_LOG_X	(4)
#define		CHUNK_LOG_Y	(8)
#define		CHUNK_LOG_Z	(4)

class	Map_loader
{
	public:
		Map_loader(Chunk_map &chunk_map): m_chunk_map(chunk_map){};
		~Map_loader(){};

		int			search_new_chunk(glm::vec3 player_pos);
		void		update(){m_update = true;}
		void		set_chunk_event_list(s_chunk_event_list *list){m_chunk_event_list = list;}
		void		unset_chunk_event_list(){m_chunk_event_list = nullptr;}

	private:
		int			next_chunk_in_radius(s_vec3i center, s_vec3i &pos, int radius);

		Chunk_map						m_chunk_map;
		s_chunk_event_list	*m_chunk_event_list = nullptr;
		int									m_radius_generate = 20;
		int									m_radius_mesh = 10;
		int									m_radius_unload = 30;
		bool								m_update = true;
};

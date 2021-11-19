/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Map_manager.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trobicho <trobicho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/16 07:57:43 by trobicho          #+#    #+#             */
/*   Updated: 2021/11/19 00:14:04 by trobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Map_manager.hpp"

Map_manager::Map_manager() : m_vdb(m_map_node)
{
}

Map_manager::~Map_manager()
{
}

void	Map_manager::init()
{
	m_map_node = Internal_L1(0, 0, 0);
	m_chunk_manager.lunch();
	m_state = MAP_MANAGER_STATE_GENERATING;
}

void	Map_manager::lunch()
{
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Vdb_test.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trobicho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/02 21:06:37 by trobicho          #+#    #+#             */
/*   Updated: 2021/11/16 09:44:40 by trobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "libvdb.h"

using	Leaf = Leaf_node<uint32_t, 2>;
using	Chunk_part_node = Internal_node<uint32_t, Leaf, 2, 2, 2>;
using	Chunk_node = Internal_node<uint32_t, Leaf, 0, 3, 0>;
using	Internal_L2 = Internal_node<uint32_t, Chunk_node , 8, 1, 8>;
using	Internal_L1 = Internal_node<uint32_t, Internal_L2, 8, 1, 8>;
using	Node_v = Node<uint32_t>;

class	Vdb_test
{
	public:
		Vdb_test(Node_v &root);
		~Vdb_test(){};

		void		set_vox(uint32_t value, s_vec3i v);
		void		unset_vox(s_vec3i v);
		uint32_t	get_vox(s_vec3i v) const;
		s_vec3i		get_size(){return (m_max);}
		int			remove_node_by_slog(s_vec3i node_pos, uint32_t slog);
		const Node_v
					*get_interresting_node(s_vec3i v, uint32_t &value) const;
		void		pruning();
		/*
		void		mesh(Mesh_interface &mesh) const;
		void		mesh(Mesh_interface &mesh, const s_vbox &box) const;
		*/

	private:
		Node_v		&m_root_static;
		s_vec3i		m_min;
		s_vec3i		m_max;
};

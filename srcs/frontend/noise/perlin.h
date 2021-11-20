/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   perlin.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trobicho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/16 17:58:09 by trobicho          #+#    #+#             */
/*   Updated: 2021/11/20 12:50:02 by trobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Noise.h"

class	Perlin_noiser
{
	public:
		Perlin_noiser(){};

		void	shuffle(){Noise noise; noise.shuffle();}

		static double	rand_noise(int t);
		static double	cerp(double a, double b, double t);
		double	cubic_interpolate(double p0, double p1
					, double b_p0, double a_p1, double t);
		double	noise2d(int x, int y);
		double	noise3d(int x, int y, int z);
		double	smooth_noise(double x);
		double	smooth_noise2d(double x, double y);
		double	smooth_noise3d(double x, double y, double z);
		double	smooth_noise2d_cubic(double x, double y);
		double	perlin(int nb_octave, double freq
					, double persistance, double x);
		double	perlin2d(int nb_octave, double freq
					, double persistence, double x, double y);
		double	perlin3d(int nb_octave, double freq
					, double persistence, double x, double y, double z);
		double	smooth_noise_firstdim(int integer_x
					, int integer_y, double fractional_x);

};

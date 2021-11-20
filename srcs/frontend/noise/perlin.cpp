/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   perlin.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trobicho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/16 18:03:36 by trobicho          #+#    #+#             */
/*   Updated: 2019/12/02 15:43:41 by trobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "perlin.h"
#include "Noise.h"
#include <cmath>

double	Perlin_noiser::rand_noise(int t)
{
	t = (t << 13) ^ t;
	t = (t * (t * t * 15731 + 789221) + 1376312589);
	return (1.0 - (t & 0x7fffffff) / 1073741824.0);
}

double	Perlin_noiser::cerp(double a, double b, double t)
{
	double	c = (1. - cos(t * M_PI)) * 0.5;
	return ((1. - c) * a + c * b);
}

double	Perlin_noiser::cubic_interpolate(double p0, double p1
			, double b_p0, double a_p1, double t)
{
	double	c3 = -0.5 * b_p0 + 1.5 * p0 - 1.5 * p1 + 0.5 * a_p1;
	double	c2 = b_p0 - 2.5 * p0 + 2. * p1 - 0.5 * a_p1; 
	double	c1 = -0.5 * b_p0 + 0.5 * p1;
	double	c0 = p0;

	return ((c3 * t * t * t) + (c2 * t * t) + (c1 * t) + c0);
}

double	Perlin_noiser::noise2d(int x, int y)
{
	int	tmp = rand_noise(x) * 850000;

	return (rand_noise(tmp + y) + 1.0) / 2.0;
}

double	Perlin_noiser::noise3d(int x, int y, int z)
{
	int	tmp_x = rand_noise(x) * 850000;
	int	tmp_y = rand_noise(tmp_x + y) * 850000;

	return (rand_noise(tmp_y + z) + 1.0) / 2.0;
}

double	Perlin_noiser::smooth_noise(double x)
{
	int		int_x = ((x >= 0) ? (int)x : (int)x - 1);
	double	fract_x = x - int_x;
	double	a = rand_noise(int_x);
	double	b = rand_noise(int_x + 1);

	return (Noise::lerp(a, b, fract_x));
}

double	Perlin_noiser::smooth_noise2d(double x, double y)
{
	int		int_x = ((x >= 0) ? (int)x : (int)x - 1);
	int		int_y = ((y >= 0) ? (int)y : (int)y - 1);
	double	fract_x = x - int_x;
	double	fract_y = y - int_y;
	double	a, b, c, d;
	double	f, g;

	a = noise2d(int_x, int_y);
	b = noise2d(int_x + 1, int_y);
	c = noise2d(int_x, int_y + 1);
	d = noise2d(int_x + 1, int_y + 1);

	f = Noise::lerp(a, b, fract_x);
	g = Noise::lerp(c, d, fract_x);

	return (Noise::lerp(f, g, fract_y));
}

double	Perlin_noiser::smooth_noise3d(double x, double y, double z)
{
	int		int_x = ((x >= 0) ? (int)x : (int)x - 1);
	int		int_y = ((y >= 0) ? (int)y : (int)y - 1);
	int		int_z = ((z >= 0) ? (int)z : (int)z - 1);
	double	fract_x = x - int_x;
	double	fract_y = y - int_y;
	double	fract_z = z - int_z;
	double	a0, b0, c0, d0;
	double	a1, b1, c1, d1;
	double	a, b, v, c, d, w;
	double	f, g;

	a0 = noise3d(int_x, int_y, int_z);
	a1 = noise3d(int_x + 1, int_y, int_z);
	
	b0 = noise3d(int_x, int_y + 1, int_z);
	b1 = noise3d(int_x + 1, int_y + 1, int_z);
	
	c0 = noise3d(int_x, int_y, int_z + 1);
	c1 = noise3d(int_x + 1, int_y, int_z + 1);

	d0 = noise3d(int_x, int_y + 1, int_z + 1);
	d1 = noise3d(int_x + 1, int_y + 1, int_z + 1);

	a = Noise::lerp(a0, a1, fract_x);
	b = Noise::lerp(b0, b1, fract_x);
	v = Noise::lerp(a, b, fract_y);
	c = Noise::lerp(c0, c1, fract_x);
	d = Noise::lerp(d0, d1, fract_x);
	w = Noise::lerp(c, d, fract_y);

	return Noise::lerp(v, w, fract_z);
}

double Perlin_noiser::smooth_noise_firstdim(int integer_x
			, int integer_y, double fract_x)
{
    double v0 = noise2d(integer_x - 1, integer_y);
    double v1 = noise2d(integer_x,     integer_y);
    double v2 = noise2d(integer_x + 1, integer_y);
    double v3 = noise2d(integer_x + 2, integer_y);

    return cubic_interpolate(v0, v1, v2, v3, fract_x);
}

double	Perlin_noiser::smooth_noise2d_cubic(double x, double y)
{
    int integer_x = (int)x;
    double fract_x = x - integer_x;
    int integer_y = (int)y;
    double fract_y = y - integer_y;

    double t0 = smooth_noise_firstdim(integer_x,
                    integer_y - 1, fract_x);
    double t1 = smooth_noise_firstdim(integer_x,
                    integer_y,     fract_x);
    double t2 = smooth_noise_firstdim(integer_x,
                    integer_y + 1, fract_x);
    double t3 = smooth_noise_firstdim(integer_x,
                    integer_y + 2, fract_x);

    return (cubic_interpolate(t0, t1, t2, t3, fract_y));
}

double	Perlin_noiser::perlin(int nb_octave, double freq
			, double persistence, double x)
{
	double	r = 0.;
	double	f = freq;
	double	amplitude = 1.;

	for (int i = 0; i < nb_octave; i++)
	{
		int t = i * 4096;
		r += smooth_noise(x * f + t) * amplitude;
		amplitude *= persistence;
		f *= 2.;
	}
	double	geo_lim = (1 - persistence) / (1 - amplitude);
	return (r * geo_lim);
}

double	Perlin_noiser::perlin2d(int nb_octave, double freq
			, double persistence, double x, double y)
{
	double	r = 0.;
	double	f = freq;
	double	amplitude = 1.;
	static Noise	noise;

	for (int i = 0; i < nb_octave; i++)
	{
		int t = i * 4096;
		//r += noise.smooth_noise2d(x * f + t, y * f + t) * amplitude;
		r += ((noise.smooth_noise2d(x * f + t, y * f + t) + 1.0f) / 2.0f) * amplitude;
		amplitude *= persistence;
		f *= 2.;
	}
	double	geo_lim = (1 - persistence) / (1 - amplitude);
	return (r * geo_lim);
}

double	Perlin_noiser::perlin3d(int nb_octave, double freq
			, double persistence, double x, double y, double z)
{
	double	r = 0.;
	double	f = freq;
	double	amplitude = 1.;
	static Noise	noise;

	for (int i = 0; i < nb_octave; i++)
	{
		int t = i * 4096;
		//r += smooth_noise3d(x * f + t, y * f + t, z * f + t) * amplitude;
		r += (noise.smooth_noise3d(x * f + t, y * f + t, z * f + t) + 0.5f) * amplitude;
		amplitude *= persistence;
		f *= 2.;
	}
	double	geo_lim = (1 - persistence) / (1 - amplitude);
	return (r * geo_lim);
}

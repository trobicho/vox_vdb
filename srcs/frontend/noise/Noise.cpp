/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Noise.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trobicho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/28 13:39:08 by trobicho          #+#    #+#             */
/*   Updated: 2020/06/24 08:56:23 by trobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Noise.h"
#include <cmath>

static unsigned char
				perm[512] = {
    151,160,137,91,90,15,131,13,201,95,96,53,194,233,7,225,140,
    36,103,30,69,142,8,99,37,240,21,10,23,190,6,148,247,120,234,
    75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,88,237,
    149,56,87,174,20,125,136,171,168,68,175,74,165,71,134,139,
    48,27,166,77,146,158,231,83,111,229,122,60,211,133,230,220,
    105,92,41,55,46,245,40,244,102,143,54,65,25,63,161,1,216,80,
    73,209,76,132,187,208,89,18,169,200,196,135,130,116,188,159,
    86,164,100,109,198,173,186,3,64,52,217,226,250,124,123,5,
    202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,
    182,189,28,42,223,183,170,213,119,248,152,2,44,154,163,70,
    221,153,101,155,167,43,172,9,129,22,39,253,19,98,108,110,79,
    113,224,232,178,185,112,104,218,246,97,228,251,34,242,193,
    238,210,144,12,191,179,162,241,81,51,145,235,249,14,239,107,
    49,192,214,31,181,199,106,157,184,84,204,176,115,121,50,45,
    127,4,150,254,138,236,205,93,222,114,67,29,24,72,243,141,
    128,195,78,66,215,61,156,180,
    151,160,137,91,90,15,131,13,201,95,96,53,194,233,7,225,140,
    36,103,30,69,142,8,99,37,240,21,10,23,190,6,148,247,120,234,
    75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,88,237,
    149,56,87,174,20,125,136,171,168,68,175,74,165,71,134,139,
    48,27,166,77,146,158,231,83,111,229,122,60,211,133,230,220,
    105,92,41,55,46,245,40,244,102,143,54,65,25,63,161,1,216,80,
    73,209,76,132,187,208,89,18,169,200,196,135,130,116,188,159,
    86,164,100,109,198,173,186,3,64,52,217,226,250,124,123,5,
    202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,
    182,189,28,42,223,183,170,213,119,248,152,2,44,154,163,70,
    221,153,101,155,167,43,172,9,129,22,39,253,19,98,108,110,79,
    113,224,232,178,185,112,104,218,246,97,228,251,34,242,193,
    238,210,144,12,191,179,162,241,81,51,145,235,249,14,239,107,
    49,192,214,31,181,199,106,157,184,84,204,176,115,121,50,45,
    127,4,150,254,138,236,205,93,222,114,67,29,24,72,243,141,
    128,195,78,66,215,61,156,180,
};

static int		_grad3[16][3] = {
    {1,1,0}, {-1,1,0}, {1,-1,0}, {-1,-1,0},
    {1,0,1}, {-1,0,1}, {1,0,-1}, {-1,0,-1},
    {0,1,1}, {0,-1,1}, {0,1,-1}, {0,-1,-1},
    {1,1,0}, {-1,1,0}, {0,-1,1}, {0,-1,-1}
};

static int		_grad2[4][2] = {
    {1,1}, {-1,1}, {1,-1}, {-1,-1},
};

static double	fast_dot(const int *v, const double x,
        const double y)
{
    return v[0] * x + v[1] * y;
}

static double	fast_dot(const int *v, const double x,
        const double y, const double z)
{
    return v[0] * x + v[1] * y + v[2] * z;
}

static int		*get_grad(int x, int y, int z)
{
    int rand_value = perm[z + perm[y + perm[x]]];
    return _grad3[rand_value & 15];
}

static int		*get_grad(int x, int y)
{
    int rand_value = perm[y + perm[x]];
    return _grad2[rand_value & 3];
}

static double	quintic_poly(const double t)
{
    const double t3 = t * t * t;
    return t3 * (t * (t * 6. - 15.) + 10.);
}

static void		int_and_frac(double value, int &integer_part,
            double &fractional_part)
{
    integer_part = (int)value;
    if(value < 0)
        integer_part -= 1;
    fractional_part = value - integer_part;
}

#include <algorithm>
#include <ctime>

void		Noise::shuffle()
{
	std::srand ( unsigned ( std::time(0) ) );
	std::random_shuffle(&perm[0], &perm[512]);
}

double		Noise::lerp(double a, double b, double t)
{
	return ((1. - t) * a + t * b);
}


double		Noise::smooth_noise2d(double x_pos, double y_pos)
{
    int		x_int, y_int;
    double	x_frac, y_frac;

    int_and_frac(x_pos, x_int, x_frac);
    int_and_frac(y_pos, y_int, y_frac);

    x_int &= 255;
    y_int &= 255;

    const double g00 = fast_dot(get_grad(x_int, y_int)
							, x_frac, y_frac);
    const double g01 = fast_dot(get_grad(x_int, y_int + 1)
							, x_frac, y_frac - 1.);
    const double g10 = fast_dot(get_grad(x_int + 1, y_int)
							, x_frac - 1., y_frac);
    const double g11 = fast_dot(get_grad(x_int + 1, y_int + 1)
							, x_frac - 1., y_frac - 1.);

    const double u = quintic_poly(x_frac);
    const double v = quintic_poly(y_frac);

    const double x00 = lerp(g00 , g10, u);
    const double x10 = lerp(g01 , g11, u);


    const double xy = lerp(x00 , x10, v);

    return (xy);
}

double		Noise::smooth_noise3d(double x_pos, double y_pos, double z_pos)
{
    int		x_int, y_int, z_int;
    double	x_frac, y_frac, z_frac;

    int_and_frac(x_pos, x_int, x_frac);
    int_and_frac(y_pos, y_int, y_frac);
    int_and_frac(z_pos, z_int, z_frac);

    x_int &= 255;
    y_int &= 255;
    z_int &= 255;

    const double g000 = fast_dot(get_grad(x_int, y_int, z_int)
							, x_frac, y_frac, z_frac);
    const double g001 = fast_dot(get_grad(x_int, y_int, z_int + 1)
							, x_frac, y_frac, z_frac - 1.);
    const double g010 = fast_dot(get_grad(x_int, y_int + 1, z_int)
							, x_frac, y_frac - 1., z_frac);
    const double g011 = fast_dot(get_grad(x_int, y_int + 1, z_int + 1)
							, x_frac, y_frac - 1., z_frac - 1.);
    const double g100 = fast_dot(get_grad(x_int + 1, y_int, z_int)
							, x_frac - 1., y_frac, z_frac);
    const double g101 = fast_dot(get_grad(x_int + 1, y_int, z_int + 1)
							, x_frac - 1., y_frac, z_frac - 1.);
    const double g110 = fast_dot(get_grad(x_int + 1, y_int + 1, z_int)
							, x_frac - 1., y_frac - 1., z_frac);
    const double g111 = fast_dot(get_grad(x_int + 1, y_int + 1, z_int + 1)
							, x_frac - 1., y_frac - 1., z_frac - 1.);

    const double u = quintic_poly(x_frac);
    const double v = quintic_poly(y_frac);
    const double w = quintic_poly(z_frac);

    const double x00 = lerp(g000 , g100, u);
    const double x10 = lerp(g010 , g110, u);
    const double x01 = lerp(g001 , g101, u);
    const double x11 = lerp(g011 , g111, u);

    const double xy0 = lerp(x00 , x10, v);
    const double xy1 = lerp(x01 , x11, v);

    const double xyz = lerp(xy0 , xy1, w);

    return (xyz);
}

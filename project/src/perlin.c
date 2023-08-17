/* Simple Perlin Noise Library */
#include "data_types.h"

f32 interpolate(f32 a0, f32 a1, f32 w)
{
    // Very rough interpolation
    // Since this is a tile based world I don't need smoothness
    // But you can apply smoothstep for further smoothness
    // SmoothStep:
    // return (a1 - a0) * (3.0 - w * 2.0) * w * w + a0;
    return (a1 - a0) * w + a0;
}

i32 perlin_rand(u64* seed)
{
    // A bit hodge podge but it works
	return ((*seed = *seed * 1103515245 + 12345) % ((u64)2147483647 + 1));
}

// Create pseudorandom direction vector
f32* random_gradient(u64 seed, i32 ix, i32 iy)
{
    // No precomputed gradients mean this works for any number of grid coordinates
    const u32 w = 8 * sizeof(u32);
    const u32 s = w / 2; // rotation width
    u32 a = ix, b = iy;
    a *= perlin_rand(&seed); b ^= a << s | a >> w-s;
    b *= perlin_rand(&seed); a ^= b << s | b >> w-s;
    a *= perlin_rand(&seed);
    f32 random = a * (3.14159265 / ~(~0u >> 1)); // in [0, 2*Pi]
    vec2 v = {cos(random),sin(random)};
    return v;
}

// Computes the dot product of the offset and gradient vectors.
f32 dot_grid_gradient(u64 seed, i32 ix, i32 iy, f32 x, f32 y)
{
    // Get gradient from integer coordinates
    f32* gradient = random_gradient(seed, ix, iy);

    // Compute the offset vector
    f32 dx = x - (f32)ix;
    f32 dy = y - (f32)iy;

    // Actually compute the dot-product of the offset vector and the gradient vector
    return (dx*gradient[0] + dy*gradient[1]);
}

// Compute Perlin noise at coordinates x, y
f32 perlin(u64 seed, f32 x, f32 y)
{
    // Determine grid cell coordinates
    i32 x0 = (i32)floor(x);
    i32 x1 = x0 + 1;
    i32 y0 = (i32)floor(y);
    i32 y1 = y0 + 1;

    // Determine i32erpolation weights (How far away it is from grid boundaries)
    f32 sx = x - (f32)x0;
    f32 sy = y - (f32)y0;

    // Interpolate between grid poi32 gradients (Average all the vectors i32o a single number)
    f32 n0, n1, ix0, ix1, value;

    n0 = dot_grid_gradient(seed, x0, y0, x, y);
    n1 = dot_grid_gradient(seed, x1, y0, x, y);
    ix0 = interpolate(n0, n1, sx);

    n0 = dot_grid_gradient(seed, x0, y1, x, y);
    n1 = dot_grid_gradient(seed, x1, y1, x, y);
    ix1 = interpolate(n0, n1, sx);

    value = interpolate(ix0, ix1, sy);
    return value; // Will return in range -1 to 1. To make it in range 0 to 1, multiply by 0.5 and add 0.5
}

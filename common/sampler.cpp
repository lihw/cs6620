/**
 * \file sampler.cpp
 * \author lihw (lihw81@gmail.com)
 * \changelog
 * - 2019/09/01 initial check in
 *
 * The common definitions for the ray tracer.
 */

#include "sampler.hpp"


CS6620_NAMESPACE_BEGIN
    
Sampler::Sampler(u32 n)
{
    this->_samples.resize(n);
}

Sampler::~Sampler()
{
}
    
NaiveSampler::NaiveSampler(u32 n)
    : Sampler(n)
{
    assert((n & (n - 1)) == 0);

    u32 m = 1;
    for (m = 1; m * m < n; m *= 2)
        ;

    f32 inv = 1.0f / (f32)m;

    u32 s = 0;
    for (u32 i = 0; i < m; i++)
    for (u32 j = 0; j < m; j++)
    {
        f32 x = (j + 0.5f) * inv;
        f32 y = (i + 0.5f) * inv;

        this->_samples[s].x = x;
        this->_samples[s].y = y;

        s++;
    }
}

NaiveSampler::~NaiveSampler()
{
}
    
CS6620_NAMESPACE_END
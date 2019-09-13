/**
 * \file sampler.hpp
 * \author lihw (lihw81@gmail.com)
 * \changelog
 * - 2019/09/01 initial check in
 *
 * The common definitions for the ray tracer.
 */

#ifndef SAMPLER_HPP
#define SAMPLER_HPP

#include "common.h"

#include <vector>

CS6620_NAMESPACE_BEGIN
CS6620_NAMESPACE_BEGIN


class Sampler
{
public:
    /**
     */
    explicit Sampler(u32 n);
    /**
     */
    virtual ~Sampler();

    vec2 * samples() const { return &this->_samples[0]; }; 

protected:
    std::vector<vec2> _samples;
};

class NaiveSampler : public Sampler
{
public:
    /**
     * @param n the number of samples per pixel.
     */
    explicit NaiveSampler(u32 n);

    virtual ~NaiveSampler();
};

CS6620_NAMESPACE_END

#endif // !SAMPLER_HPP

#pragma once
#include <stdexcept>
#include <cmath>
#include "inputiterator.hpp"

class SimpleRNG
{
public:
    using result_type = double;

    SimpleRNG(double m, double a, double c, double x0 = 1.0)
        : m(m), a(a), c(c), _X_0(x0), _X_n(x0)
    {
        if (m <= 1)
            throw std::invalid_argument("m must be > 1");
        if (!(0 < a && a < 1))
            throw std::invalid_argument("a must be in (0,1)");
        if (!(0 < c && c < m))
            throw std::invalid_argument("c must be in (0,m)");
    }

    result_type operator()() noexcept
    {
        _X_n = std::fmod(a * _X_n + c, m);
        return _X_n;
    }

    void reset() noexcept { _X_n = _X_0; }

    void reset(double seed)
    {
        _X_0 = std::fmod(seed, m);
        _X_n = _X_0;
    }

    InputIterator<SimpleRNG> begin()
    {
        return InputIterator<SimpleRNG>(*this, _X_n);
    }

    InputIterator<SimpleRNG> end(double eps = 0.05)
    {
        return InputIterator<SimpleRNG>(eps);
    }

private:
    double m, a, c;
    double _X_0;
    double _X_n;
};
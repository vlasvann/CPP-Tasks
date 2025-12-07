#pragma once
#include <concepts>
#include <cstdint>
#include <limits>
#include "inputiterator.hpp"

template <
    std::unsigned_integral type = std::uint32_t,
    type A = static_cast<type>(48271),
    type C = static_cast<type>(0),
    type M = static_cast<type>(2147483647)>
class IntegerLCG
{
public:
    using result_type = type;
    static constexpr type multiplier = A;
    static constexpr type increment = C;
    static constexpr type modulus = M;

    static constexpr type q = modulus / multiplier;
    static constexpr type r = modulus % multiplier;

    static constexpr type min() noexcept
    {
        return increment == 0 ? 1 : 0;
    }
    static constexpr type max() noexcept { return modulus - 1; }

    constexpr explicit IntegerLCG(type seed = 1) noexcept
        : _X_n(seed % modulus)
    {
        if constexpr (increment == 0)
        {
            if (_X_n == 0)
                _X_n = 1;
        }
    }

    constexpr void reset() noexcept
    {
        _X_n = 1;
    }

    constexpr void reset(type seed) noexcept
    {
        _X_n = seed % modulus;
        if constexpr (increment == 0)
        {
            if (_X_n == 0)
                _X_n = 1;
        }
    }

    constexpr type operator()() noexcept
    {
        _X_n = calc_step(_X_n);
        return _X_n;
    }

    InputIterator<IntegerLCG> begin()
    {
        return InputIterator<IntegerLCG>(*this, _X_n);
    }

    InputIterator<IntegerLCG> end(double eps = 0.0)
    {
        return InputIterator<IntegerLCG>(eps);
    }

private:
    //multiplier, modulus > 0, modulus % multiplier < modulus / multiplier
    static constexpr type calc_step(result_type x) noexcept
    {
        if constexpr (modulus != 1)
        {
            result_type t1 = multiplier * (x % q);
            result_type t2 = r * (x / q);

            if (t1 >= t2)
                x = t1 - t2;
            else
                x = modulus - (t2 - t1);
        }
        else
        {
            x %= modulus;
        }

        if constexpr (increment != 0)
        {
            result_type d = modulus - x;
            if (d > increment)
                x += increment;
            else
                x = increment - d;
        }

        return x;
    }

private:
    result_type _X_n;
};
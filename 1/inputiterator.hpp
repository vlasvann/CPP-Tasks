#pragma once
#include <iterator>

template <typename RNG>
class InputIterator
{
public:
    using value_type = typename RNG::result_type;
    using pointer = const value_type *;
    using reference = const value_type;
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::input_iterator_tag;

private:
    RNG *_rng{nullptr};
    value_type _value{};
    value_type _start_value{};
    double _eps{0.0};
    bool _is_end_iterator{false};
    bool _first_step{true};

public:
    InputIterator(RNG &rng, value_type start_value)
        : _rng(&rng), _value(start_value), _start_value(start_value) {}

    InputIterator(double eps)
        : _eps(eps), _is_end_iterator(true) {}

    reference operator*() const { return _value; }
    pointer operator->() const { return &_value; }

    InputIterator &operator++()
    {
        if (_is_end_iterator)
            return *this;
        _value = (*_rng)();
        _first_step = false;
        return *this;
    }

    InputIterator operator++(int)
    {
        InputIterator tmp = *this;
        ++(*this);
        return tmp;
    }

    friend bool operator==(const InputIterator &a, const InputIterator &b)
    {
        if (a._is_end_iterator && b._is_end_iterator)
            return true;

        if (!a._is_end_iterator && !b._is_end_iterator)
            return a._value == b._value;

        if (!a._is_end_iterator && b._is_end_iterator)
        {
            if (a._first_step)
                return false;
            return std::fabs(a._value - a._start_value) < b._eps;
        }

        if (a._is_end_iterator && !b._is_end_iterator)
        {
            if (a._first_step)
                return false;
            return std::fabs(b._value - b._start_value) < a._eps;
        }
        return false;
    }

    friend bool operator!=(const InputIterator &a, const InputIterator &b)
    {
        return !(a == b);
    }
};
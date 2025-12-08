#include <type_traits>
#include <iostream>
#include <array>
#include <algorithm>
#include <concepts>
#include <cstddef>

template <typename Container>
concept SliceableByIndex =
    requires(Container c, std::size_t i) {
        { c.size() } -> std::convertible_to<std::size_t>;
        { c.at(i) };
        { c.resize(i) };
    };

template <typename Container>
concept EraseIfCompatible =
    requires(Container c) {
        std::erase_if(c, [](const typename Container::value_type &)
                      { return false; });
    };

template <typename Container, typename Func>
concept Transformable =
    requires(Container c, Func f) {
        { Container{c} };
        { f(*c.begin()) };
        requires(!std::is_const_v<typename Container::value_type>);
        requires std::assignable_from<typename Container::value_type &, decltype(f(*c.begin()))>;
    };

template <std::size_t N>
class Mask
{
public:
    std::array<bool, N> __data_;

    template <typename... Ts>
    constexpr Mask(Ts... values) : __data_{static_cast<bool>(values)...}
    {
        static_assert((sizeof...(values) == N), "Incorrect values!");
        static_assert((std::is_integral_v<Ts> && ...), "Must be integer");

        (check_value(values), ...);
    }

    constexpr std::size_t size() const { return N; }

    constexpr bool at(std::size_t index) const
    {
        return index < N
                   ? __data_[index]
                   : (throw std::out_of_range("Mask index out of range"), __data_[0]);
    }

    // constexpr bool at(size_t index) const
    // {
    //     if (index >= N)
    //         throw std::out_of_range("Mask index out of range");
    //     return __data_[index];
    // }

    template <std::size_t I>
    constexpr bool at() const
        requires(I < N)
    {
        return __data_[I];
    }

    template <SliceableByIndex Container>
    void slice(Container &container) const
    {
        std::size_t write = 0;
        std::size_t size = container.size();

        for (std::size_t i = 0; i < size; ++i)
        {
            if (__data_[i % N])
            {
                if (write != i)
                    container.at(write) = std::move(container.at(i));
                ++write;
            }
        }
        container.resize(write);
    }

    template <EraseIfCompatible Container>
        requires(!SliceableByIndex<Container>)
    void slice(Container &container) const
    {
        std::size_t index_mask = 0;
        std::erase_if(container,
                      [this, &index_mask]([[maybe_unused]] const auto &)
                      {
                          return !__data_[index_mask++ % N];
                      });
    }

    template <typename Container, typename Func>
        requires Transformable<Container, Func>
    Container transform(const Container &c, Func f) const
    {
        Container res = c;
        std::size_t index_mask = 0;

        for (auto &elem : res)
        {
            if (__data_[index_mask++ % N])
                elem = f(elem);
        }
        return res;
    }

    template <typename Container, typename Func>
        requires(Transformable<Container, Func> &&
                 (SliceableByIndex<Container> || EraseIfCompatible<Container>))
    Container slice_and_transform(const Container &c, Func f) const
    {
        Container res = transform(c, f);
        slice(res);
        return res;
    }

private:
    constexpr static void check_value(int value)
    {
        return (value == 0 || value == 1)
                   ? void()
                   : (throw std::invalid_argument("Mask values must be 0 or 1"), void());
    }
};

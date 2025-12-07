#include <iostream>
#include <vector>
#include <list>
#include <forward_list>
#include <deque>
#include <string>
#include <string_view>
#include "mask.hpp"

template <Mask<3> M>
struct Foo
{
    void print() const
    {
        for (std::size_t i = 0; i < M.size(); ++i)
            std::cout << M.at(i) << ' ';
        std::cout << '\n';
    }
};

template <typename Container>
void print_container(const Container &c)
{
    using T = typename Container::value_type;
    if constexpr (std::is_same_v<T, wchar_t>)
    {
        std::for_each(c.begin(), c.end(), [](wchar_t ch)
                      { std::wcout << ch << L" "; });
    }
    else
    {
        std::for_each(c.begin(), c.end(), [](const auto &x)
                      { std::cout << x << " "; });
    }
}

int main()
{
    Mask<4> mask = {1, 0, 1, 1};

    auto mul10 = [](int x)
    { return x * 10; };
    auto char_shift = [](char c)
    { return char(c + 1); };
    auto w_shift = [](wchar_t c)
    { return wchar_t(c + 1); };

    std::cout << "\nslice_and_transform:\n";

    {
        std::cout << "\nstd::vector<int>\n";
        std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8};

        std::cout << "Before: ";
        print_container(v);
        std::cout << "\n";

        auto res = mask.slice_and_transform(v, mul10);

        std::cout << "After:  ";
        print_container(res);
        std::cout << "\n";
    }

    {
        std::cout << "\nstd::deque<int>\n";
        std::deque<int> d = {1, 2, 3, 4, 5, 6, 7, 8};

        std::cout << "Before: ";
        print_container(d);
        std::cout << "\n";

        auto res = mask.slice_and_transform(d, mul10);

        std::cout << "After:  ";
        print_container(res);
        std::cout << "\n";
    }

    {
        std::cout << "\nstd::string\n";
        std::string s = "ABCDEFGH";

        std::cout << "Before: ";
        print_container(s);
        std::cout << "\n";

        auto res = mask.slice_and_transform(s, char_shift);

        std::cout << "After:  ";
        print_container(res);
        std::cout << "\n";
    }

    {
        std::cout << "\nstd::wstring\n";
        std::wstring ws = L"ABCDEFGH";

        std::wcout << L"Before: ";
        print_container(ws);
        std::wcout << L"\n";

        auto res = mask.slice_and_transform(ws, w_shift);

        std::wcout << L"After:  ";
        print_container(res);
        std::wcout << L"\n";
    }

    {
        std::cout << "\nstd::list<int>\n";
        std::list<int> lst = {1, 2, 3, 4, 5, 6, 7, 8};

        std::cout << "Before: ";
        print_container(lst);
        std::cout << "\n";

        auto res = mask.slice_and_transform(lst, mul10);

        std::cout << "After:  ";
        print_container(res);
        std::cout << "\n";
    }

    {
        std::cout << "\nstd::forward_list<int>\n";
        std::forward_list<int> fl = {1, 2, 3, 4, 5, 6, 7, 8};

        std::cout << "Before: ";
        print_container(fl);
        std::cout << "\n";

        auto res = mask.slice_and_transform(fl, mul10);

        std::cout << "After:  ";
        print_container(res);
        std::cout << "\n";
    }

    std::cout << "\nNTTP:\n";

    constexpr Mask<3> m = {1, 0, 1};
    Foo<m> f;
    f.print();

    return 0;
}
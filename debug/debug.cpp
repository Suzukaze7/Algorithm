#pragma once
#include<iostream>
#include<sstream>
#include<type_traits>
#include<utility>
#include<queue>

#define LEN 1
#define debug(args...) suzukaze::_debug(__LINE__, #args, ##args)

namespace suzukaze
{
    template<typename> struct is_pair: std::false_type { };
    template<typename X, typename Y> struct is_pair<std::pair<X, Y>>: std::true_type { };

    template<typename T> struct is_string: std::__or_<std::is_same<T, char *>, std::is_same<T, std::string>> { };
    template<size_t U> struct is_string<char[U]>: std::true_type { };

    template<typename, typename = void> struct is_iterable: std::false_type { };
    template<typename T> struct is_iterable<T, std::void_t<decltype(std::begin(std::declval<T &>())), decltype(std::end(std::declval<T &>()))>>: std::true_type { };

    template<typename, typename = void> struct has_top: std::false_type { };
    template<typename T> struct has_top<T, std::void_t<decltype(std::declval<T>().top())>>: std::true_type { };

    class _debug
    {
        const std::string blank = std::string(LEN, ' '), sep = "," + blank, equal = blank + '=' + blank;

        std::ostringstream out;
        std::string names;
        int idx, cover;

    public:
        template<typename T, typename ...U>
        _debug(const int line, const char *names, const T &arg, const U &...args): names(names)
        {
            out << ".." << line << "..\t";
            if constexpr (std::__or_v<std::is_arithmetic<T>, is_pair<T>, is_string<T>>)
                _print(arg, false), (..., _print(args, true));
            else
                _print(arg, false, args...);
            std::clog << out.str() << "\n";
        }

        _debug(const int line, const char *names) { std::clog << "-------------------------------\n"; }

    private:
        void _print_sep(int cnt) { if (cnt) out << sep; }

        template<typename T, typename ...U>
        void _print(const T &arg, const bool flag, const U &...args)
        {
            _print_sep(flag);
            while (idx < names.size() && (cover || names[idx] != ','))
            {
                if (names[idx] == '(')
                    cover = true;
                else if (names[idx] == ')')
                    cover = false;
                out << names[idx++];
            }
            out << equal;
            idx += sep.size();

            __print(arg, args...);
        }

        template<typename T>
        std::enable_if_t<std::is_arithmetic_v<T>> __print(const T arg) { out << arg; }

        template<typename T>
        std::enable_if_t<is_string<T>::value> __print(const T &arg) { out << '"' << arg << '"'; }

        template<typename T>
        std::enable_if_t<is_pair<T>::value> __print(const T &arg)
        {
            out << '(' << arg.first << sep << arg.second << ')';
        }

        template<typename T, typename ...U>
        std::enable_if_t<is_iterable<T>::value && !is_string<T>::value> __print(const T &arg, const int size = INT32_MAX, const U &...args)
        {
            int cnt = 0;
            out << '[';
            for (auto it = std::begin(arg); cnt < size && it != std::end(arg); it++, cnt++)
                _print_sep(cnt), __print(*it, args...);
            out << ']';
        }

        template<typename T, typename ...U>
        std::enable_if_t<has_top<T>::value> __print(T arg, const int size = INT32_MAX, const U &...args)
        {
            int cnt = 0;
            out << '<';
            while (cnt < size && arg.size())
                _print_sep(cnt++), __print(arg.top()), arg.pop();
            out << '>';
        }

        template<typename T, typename ...U>
        void __print(std::queue<T> arg, const int size = INT32_MAX, const U &...args)
        {
            int cnt = 0;
            out << '<';
            while (cnt < size && arg.size())
                _print_sep(cnt++), __print(arg.front()), arg.pop();
            out << '>';
        }
    };
}
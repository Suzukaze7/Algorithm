#pragma once
#include<iostream>
#include<sstream>
#include<type_traits>
#include<utility>
#include<queue>

#define LEN 1
#define debug(...) suzukaze::_debug(__LINE__, #__VA_ARGS__ __VA_OPT__(,) __VA_ARGS__)

namespace suzukaze
{
    template<typename T> struct is_string: std::__or_<std::is_same<T, char *>, std::is_same<T, std::string>> { };
    template<size_t U> struct is_string<char[U]>: std::true_type { };

    template<typename, typename = void> struct is_iterable: std::false_type { };
    template<typename T> struct is_iterable<T, std::void_t<decltype(std::begin(std::declval<T &>())), decltype(std::end(std::declval<T &>()))>>: std::true_type { };

    template<typename, typename = void> struct has_top: std::false_type { };
    template<typename T> struct has_top<T, std::void_t<decltype(std::declval<T>().top())>>: std::true_type { };

    template<typename> struct is_queue: std::false_type { };
    template<typename T> struct is_queue<std::queue<T>>: std::true_type { };

    class _debug
    {
        const std::string blank = std::string(LEN, ' '), sep = "," + blank, equal = blank + '=' + blank;

        std::ostringstream out;
        std::string names;
        int idx = 0, cover = 0;

    public:
        template<typename T, typename ...U>
        _debug(const int line, const char *names, const T &arg, const U &...args): names(names)
        {
            out << ".." << line << "..\t";
            if constexpr (std::is_arithmetic_v<T> || std::__is_pair<T> || is_string<T>::value)
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
                cover += names[idx] == '(' || names[idx] == '<';
                cover -= names[idx] == ')' || names[idx] == '>';
                out << names[idx++];
            }
            out << equal;
            idx += sep.size();

            __print(arg, args...);
        }

        template<typename T, typename ...U>
        void __print(const T &arg, const int size = INT_MAX, const U & ...args)
        {
            int cnt = 0;
            if constexpr (std::is_arithmetic_v<T>)
                out << arg;
            if constexpr (is_string<T>::value)
                out << '"' << arg << '"';
            if constexpr (std::__is_pair<T>)
                out << '(' << arg.first << sep << arg.second << ')';
            if constexpr (is_iterable<T>::value && !is_string<T>::value)
            {
                out << '[';
                for (auto it = std::begin(arg); cnt < size && it != std::end(arg); it++, cnt++)
                    _print_sep(cnt), __print(*it, args...);
                out << ']';
            }
            if constexpr (has_top<T>::value)
            {
                auto t = arg;
                out << '<';
                while (cnt < size && t.size())
                    _print_sep(cnt++), __print(t.top()), t.pop();
                out << '>';
            }
            if constexpr (is_queue<T>::value)
            {
                auto t = arg;
                out << '<';
                while (cnt < size && t.size())
                    _print_sep(cnt++), __print(t.front()), t.pop();
                out << '>';
            }
        }
    };
}
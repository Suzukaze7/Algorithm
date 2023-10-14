#pragma once
#include<iostream>
#include<sstream>
#include<concepts>

#define debug(...) suzukaze::_debug(__LINE__, #__VA_ARGS__ __VA_OPT__(,) __VA_ARGS__)

namespace suzukaze
{
    template<typename T> concept string = std::convertible_to<T, std::string>;
    template<typename T> concept iterable = requires(const T & t) { std::begin(t); std::end(t); };
    template<typename T> concept has_top = requires(T t) { t.top(); };

    class _debug
    {
        const std::string blank = std::string(1, ' '), sep = "," + blank, equal = blank + '=' + blank;

        std::ostringstream sout;
        std::string names;
        int idx = 0, cover = 0;

    public:
        template<typename T, typename ...U>
        _debug(const int line, const std::string &names, const T &arg, const U &...args) : names(names)
        {
            sout << ".." << line << "..\t";
            if constexpr (!iterable<T>)
                _print(arg, false), (..., _print(args, true));
            else
                _print(arg, false, args...);
            std::clog << sout.str() << "\n";
        }

        _debug(const int line, const std::string &names) { std::clog << "-------------------------------\n"; }

    private:
        void _print_sep(int idx) { if (idx) sout << sep; }

        template<typename T, typename ...U>
        void _print(const T &arg, const bool flag, const U &...args)
        {
            _print_sep(flag);
            while (idx < names.size() && (cover || names[idx] != ','))
            {
                cover += names[idx] == '(' || names[idx] == '<';
                cover -= names[idx] == ')' || names[idx] == '>';
                sout << names[idx++];
            }
            sout << equal;
            idx += sep.size();

            __print(arg, args...);
        }

        template<typename T, typename ...U>
        void __print(const T &arg, const int size = INT_MAX, const U & ...args)
        {
            int idx = 0;
            if constexpr (std::is_arithmetic_v<T>)
                sout << arg;
            else if constexpr (string<T>)
                sout << '"' << arg << '"';
            else if constexpr (std::__is_pair<T>)
                sout << '(', __print(arg.first, size, args...), sout << sep, __print(arg.second, size, args...), sout << ')';
            else if constexpr (iterable<T>)
            {
                sout << '[';
                for (auto it = std::begin(arg); idx < size && it != std::end(arg); it++, idx++)
                    _print_sep(idx), __print(*it, args...);
                sout << ']';
            }
            else
            {
                auto t = arg;
                sout << '<';
                while (idx < size && t.size())
                    if constexpr (has_top<T>)
                        _print_sep(idx++), __print(t.top()), t.pop();
                    else
                        _print_sep(idx++), __print(t.front()), t.pop();
                sout << '>';
            }
        }
    };
}
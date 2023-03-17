#include<iostream>
#include<type_traits>

using namespace std;

#define debug(_args...) clog << ".." << __LINE__ << "..\t"#_args" = ", _debug(_args)

template<typename T> struct is_pair: false_type { };
template<typename X, typename Y> struct is_pair<pair<X, Y>>: true_type { };

template<typename T> struct is_string: false_type { };
template<size_t U> struct is_string<char[U]>: true_type { };
template<> struct is_string<string>: true_type { };
template<> struct is_string<char *>: true_type { };

template<typename T> void _read(const T &t) { clog << t; }

template<typename T, typename U = size_t, typename ...O>
void _print(const T &arg, const bool flag = true, const U &len = -1, const O &...oths)
{
    if (flag)
        _read(", ");

    if constexpr (is_arithmetic<T>::value)
        _read(arg);
    else
    {
        if constexpr (is_pair<T>::value)
        {
            _read("(");
            _print(arg.first, false, len, oths...);
            _print(arg.second, len, oths...);
            _read(")");
        }
        else
        {
            if constexpr (is_string<T>::value)
                _read("\"" + string(arg) + "\"");
            else
            {
                _read("[");
                int i = 0;
                for (auto &x : arg)
                    if (++i <= len)
                        _print(x, i != 1, oths...);
                    else
                        break;
                _read("]");
            }
        }
    }
}

template<typename T, typename ...U>
void _debug(const T &_arg, const U &...oths)
{
    if constexpr (is_arithmetic<T>::value || is_pair<T>::value || is_string<T>::value)
        _print(_arg, false), (..., _print(oths));
    else
        _print(_arg, false, oths...);
    clog << endl;
}

void _debug() { clog << "--------------\n"; }
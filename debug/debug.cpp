#include<iostream>
#include<type_traits>

using namespace std;

#define debug(args...) clog << ".." << __LINE__ << "..\t"#args" = ", _debug(args)

template<typename T> struct is_pair: false_type { };
template<typename X, typename Y> struct is_pair<pair<X, Y>>: true_type { };

template<typename T> struct is_string: false_type { };
template<size_t U> struct is_string<char[U]>: true_type { };
template<> struct is_string<string>: true_type { };
template<> struct is_string<char *>: true_type { };

string _res;
template<typename T> void _read(const T &t) { _res += t; }
void _set() { _res.pop_back(); _res.pop_back(); }

template<typename T, typename U = size_t, typename ...O>
void _print(const T &arg, const U &len = -1, const O &...oths)
{
    if constexpr (is_arithmetic<T>::value)
        if constexpr (__is_char<T>::__value)
            _read(arg);
        else
            _read(to_string(arg));
    else
    {
        if constexpr (is_pair<T>::value)
        {
            _read("[ ");
            _print(arg.first, len, oths...);
            _print(arg.second, len, oths...);
            _set(); _read(" ]");
        }
        else
        {
            if constexpr (is_string<T>::value)
                _read(string("\"") + arg + "\"");
            else
            {
                _read("{ ");
                int i = 0;
                for (auto &x : arg)
                    if (++i <= len)
                        _print(x, oths...);
                    else
                        break;
                _set(); _read(" }");
            }
        }
    }
    _read(", ");
}

template<typename T, typename ...U>
void _debug(const T &_arg, const U &...oths)
{
    _res.clear();
    if constexpr (is_arithmetic<T>::value || is_pair<T>::value || is_string<T>::value)
        _print(_arg), (..., _print(oths));
    else
        _print(_arg, oths...);
    _set();
    clog << _res << endl;
}
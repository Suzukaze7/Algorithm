#include<iostream>
#include<type_traits>

using namespace std;

#define debug(args...) clog << ".." << __LINE__ << "..\t", _idx = 0, _names = #args, _debug(args)

template<typename T> struct is_pair: false_type { };
template<typename X, typename Y> struct is_pair<pair<X, Y>>: true_type { };

template<typename T> struct is_string: false_type { };
template<size_t U> struct is_string<char[U]>: true_type { };
template<> struct is_string<string>: true_type { };
template<> struct is_string<char *>: true_type { };

template<typename T> void _read(const T &t) { clog << t; }

template<typename T, typename U = size_t, typename ...O>
void _print(const T &arg, const bool flag, const U &len = -1, const O &...oths)
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
            _print(arg.second, true, len, oths...);
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

int _idx, _flag;
string _names;
template<typename T, typename ...U>
void __print(const T &arg, const bool flag, U ...args)
{
    if (flag)
        _read(", ");

    while (_flag || _names[_idx] != ',' && _idx < _names.size())
    {
        if (_names[_idx] == '(' || _names[_idx] == ')')
            _flag = _names[_idx] == '(';
        _read(_names[_idx++]);
    }
    _read(" = ");
    _idx += 2;

    _print(arg, false, args...);
}

template<typename T, typename ...U>
void _debug(const T &arg, const U &...oths)
{
    if constexpr (is_arithmetic<T>::value || is_pair<T>::value || is_string<T>::value)
        __print(arg, false), (..., __print(oths, true));
    else
        __print(arg, false, oths...);
    clog << endl;
}

void _debug() { clog << "--------------\n"; }
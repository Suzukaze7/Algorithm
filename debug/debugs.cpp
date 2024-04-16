#include<iostream>
#include<type_traits>

using namespace std;

#define debug(...) clog << ".." << __LINE__ << "..\t" << #__VA_ARGS__ << " = ", _debug(__VA_ARGS__)

template<typename T>
auto operator<<(ostream &out, const T &arg)
-> enable_if_t<!is_convertible_v<T, string>, decltype(begin(arg), out)> {
    clog << '[';
    for (const auto &x : arg)
        clog << x << ", ";
    return clog << ']';
}

template<typename ...T>
void _debug(const T &...args) {
#if __cplusplus >= 201703L
    ((clog << args << ", "), ...);
#else
    int _[] = { (clog << args << ", ", 0)... };
#endif
    clog << endl;
}
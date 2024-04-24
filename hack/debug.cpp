#include<iostream>

using namespace std;

#define debug(...) clog << __LINE__ << '\t' << #__VA_ARGS__ << " = ",\
    [](const auto &...args) { ((clog << args << ", "), ...), clog << endl; }(__VA_ARGS__)


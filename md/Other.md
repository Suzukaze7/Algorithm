- 128mb 内存可开的 int 数组长度为 3e7

## 快读/int128

```cpp
template<class Int>
void read(Int &x)
{
    x = 0;
    int f = 1;
    char c = getchar();
    while (c < '0' || c > '9')
    {
        if (c == '-')
            f = -1;
        c = getchar();
    }
    while (c >= '0' && c <= '9')
    {
        x = (x << 3) + (x << 1) + (c ^ 48);
        c = getchar();
    }
    x *= f;
}

template<class Int>
void print(Int x)
{
    if (x < 0)
        putchar('-'), x = -x;
    if (x > 9)
        print(x / 10);
    putchar(x % 10 + '0');
}
```

## 一些内建函数

- `__builtin_popcount(unsigned x)`: x 的二进制下 1 的个数
- `__builtin_ctz(unsigned x)`: x 的二进制下低位连续 0 的个数，x == 0 未定义
- `__builtin_clz(unsigned x)`: x 的二进制下高位连续 0 的个数，x == 0 未定义

这些函数都有三个版本，在函数末尾加 `l`、`ll` 代表 ul、ull
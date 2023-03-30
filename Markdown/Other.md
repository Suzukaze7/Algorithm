- [快读/int\_128](#快读int_128)
- [矩阵快速幂](#矩阵快速幂)

### 快读/int_128
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

### 矩阵快速幂
- 解决一些数据非常大的线性dp，比如通过**状态机dp**求不能出现某些串的字符串方案数
  - 模板题都可以优化
  - 当要求构造的字符串需要由一些串构成，且这些串长度仅为1, 2时，可以优化
```cpp
namespace
{
    using ll = long long;
    using Col = vector<int>;
    using Mat = vector<Col>;

    void turn_E(Mat &a)
    {
        for (int i = 0; i < a.size(); ++i)
            for (int j = 0; j < a[i].size(); ++j)
                a[i][j] = (i == j);
    }
    Mat operator*(const Mat &a, const Mat &b)
    {
        Mat res = Mat(a.size(), Col(b.front().size()));
        for (int i = 0; i < a.size(); ++i)
            for (int j = 0, k; j < b.front().size(); ++j)
                for (k = res[i][j] = 0; k < a.front().size(); ++k)
                    res[i][j] = (res[i][j] + (ll)a[i][k] * b[k][j]) % mod;
        return res;
    }
    Mat operator^(Mat a, ll n)
    {
        Mat res = a;
        turn_E(res);
        while (n)
        {
            if (n & 1) res = res * a;
            a = a * a;
            n >>= 1;
        }
        return res;
    }
}
```
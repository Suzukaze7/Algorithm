- [字符串](#字符串)
  - [KMP](#kmp)
  - [Trie树](#trie树)
  - [字符串哈希](#字符串哈希)
  - [AC自动机](#ac自动机)
  - [Border/Fail树的一些性质](#borderfail树的一些性质)
    - [求串的出现次数](#求串的出现次数)
    - [树中节点到根节点的路径上全部加1](#树中节点到根节点的路径上全部加1)
  - [回文串](#回文串)
    - [性质](#性质)
  - [Manacher](#manacher)
    - [前期处理](#前期处理)
    - [用法](#用法)
    - [实现](#实现)
  - [回文自动机](#回文自动机)
    - [最小回文划分](#最小回文划分)
  - [后缀数组](#后缀数组)
  - [后缀自动机](#后缀自动机)

# 字符串

## KMP

```cpp
struct KMP {
    int size;
    char s[N];
    int ne[N];

    void op() {
        size = strlen(s + 1);
        for (int i = 2, j = 0; i <= size; i++) {
            while (j && s[i] != s[j + 1])
                j = ne[j];
            if (s[i] == s[j + 1])
                j++;
            ne[i] = j;
        }
    }
};
```

## Trie树

```cpp
struct Trie {
    static constexpr int M = 31;
    int tr[N * M][2], idx;
    int d[N * M], to[N], from[N * M];

    void init() {
        for (int i = 1; i <= idx; i++)
            tr[i][0] = tr[i][1] = from[i] = d[i] = 0;
        idx = 0;
    }

    void insert(int k, int x) {
        int p = 0;
        for (int i = M - 1; ~i; i--) {
            int u = x >> i & 1;
            if (!tr[p][u])
                tr[p][u] = ++idx;
            p = tr[p][u];
        }

        d[p] = x;
        to[k] = p;
        from[p] = k;
    }

    int query(int k) {
        int x = a[k], p = 0;
        for (int i = M - 1; ~i; i--) {
            int u = x >> i & 1;
            if (tr[p][u ^ 1])
                p = tr[p][u ^ 1];
            else
                p = tr[p][u];
        }

        return x ^ d[p];
    }
};
```

## 字符串哈希

```cpp
struct Hash {
    using ll = long long;
    static constexpr int P = 13331, MOD1 = 1e9 + 7, MOD2 = 1e9 + 9;
    static inline int pow1[N], pow2[N];
    static inline int _ = [] {
        pow1[0] = pow2[0] = 1;
        for (int i = 1; i < N; i++)
            pow1[i] = (ll)pow1[i - 1] * P % MOD1,
            pow2[i] = (ll)pow2[i - 1] * P % MOD2;
        return 0;
    }();

    char s[N];
    int hs1[N], hs2[N];

    void op() {
        for (int i = 1; s[i]; i++)
            hs1[i] = ((ll)hs1[i - 1] * P + s[i - 1]) % MOD1,
            hs2[i] = ((ll)hs2[i - 1] * P + s[i - 1]) % MOD2;
    }

    pair<int, int> get(int l, int r) {
        int d = r - l + 1;
        return make_pair(((hs1[r] - (ll)hs1[l - 1] * pow1[d]) % MOD1 + MOD1) % MOD1,
            ((hs2[r] - (ll)hs2[l - 1] * pow2[d]) % MOD2 + MOD2) % MOD2);
    }
};

// 判断回文串
struct Pal {
    struct Hash {
        using ll = long long;
        static constexpr int P = 13331, MOD1 = 1e9 + 7, MOD2 = 1e9 + 9;
        static inline int pow1[N], pow2[N];
        static inline int _ = [] {
            pow1[0] = pow2[0] = 1;
            for (int i = 1; i < N; i++)
                pow1[i] = (ll)pow1[i - 1] * P % MOD1,
                pow2[i] = (ll)pow2[i - 1] * P % MOD2;
            return 0;
        }();

        char s[N];
        int hs1[N], hs2[N];

        void op() {
            for (int i = 1; s[i]; i++)
                hs1[i] = ((ll)hs1[i - 1] * P + s[i]) % MOD1,
                hs2[i] = ((ll)hs2[i - 1] * P + s[i]) % MOD2;
        }

        pair<int, int> get(int l, int r) {
            int d = r - l + 1;
            return make_pair(((hs1[r] - (ll)hs1[l - 1] * pow1[d]) % MOD1 + MOD1) % MOD1,
                ((hs2[r] - (ll)hs2[l - 1] * pow2[d]) % MOD2 + MOD2) % MOD2);
        }
    }hs1, hs2;

    int size;

    void op() {
        size = strlen(hs1.s + 1);
        strcpy(hs2.s + 1, hs1.s + 1);
        reverse(hs2.s + 1, hs2.s + 1 + size);
        hs1.op(), hs2.op();
    }

    bool is_pal(int l, int r) {
        return hs1.get(l, r) == hs2.get(size - r + 1, size - l + 1);
    }
};
```

## AC自动机

```cpp
struct ACAM {
    static constexpr int M = 26;
    char s[N];
    int tr[N][M], idx;
    int ne[N], cnt[N], to[N], from[N];
    int q[N], hh, tt;
    vector<int> fail[N];

    void insert(int k) {
        int p = 0;
        for (int i = 1; s[i]; i++) {
            int u = s[i] - 'a';

            if (!tr[p][u])
                tr[p][u] = ++idx;
            p = tr[p][u];
        }

        cnt[p]++;
        to[k] = p;
        from[p] = k;
    }

    void build() {
        for (int i = 0; i < M; i++)
            if (tr[0][i]) {
                q[tt++] = tr[0][i];
                fail[0].push_back(tr[0][i]);
            }

        while (hh != tt) {
            int t = q[hh++];
            for (int c = 0; c < M; c++) {
                int &p = tr[t][c];
                if (!p)
                    p = tr[ne[t]][c];
                else {
                    ne[p] = tr[ne[t]][c];

                    q[tt++] = p;
                    fail[ne[p]].push_back(p);
                }
            }
        }
    }

    void init() {
        for (int i = 0; i <= idx; i++) {
            ne[i] = cnt[i] = from[i] = 0;
            fill(tr[i], tr[i] + M, 0);
            fail[i].clear();
        }
        idx = hh = tt = 0;
    }
};
```

## Border/Fail树的一些性质

### 求串的出现次数

1. 首先，一个串的所有子串，等价于这个串**所有前缀**的**所有后缀**
2. 那么，一个串`s`在另一个串`t`中出现的次数，等于`t`所有前缀中`s`作为后缀出现的次数
3. 而`Border/fail树（以下简称为树）`中节点的祖先节点是这个节点的所有`Border`，也就是该前缀的所有后缀中出现的，树中的前缀，可以利用该性质求出各种`s`在`t`中的数量关系
4. 补充：树中任一节点的父节点是该节点的最大`Border`

### 树中节点到根节点的路径上全部加1

- 等价于路径上所有点的子树大小加1，则可以用`dfs`序+树状数组**离线**维护

## 回文串

### 性质

- 回文半径的二分性：回文半径$-1$，等价于同时删掉回文串的首尾字母，依然是回文串
- 回文串和`Border`：对于回文串`S`，回文前（后）缀等价于`Border`

## Manacher

### 前期处理

- 将`S`中任意两个字符间与开头结尾插入`#`，因此所有回文串都变成奇数长度，且首尾 一定是`#`
- 可以发现： $|S^\#|=2|S|+1$ ，以及 $|S|=\frac{|S^\#|-1}{2}=\lfloor\frac{|S^\#|}{2}\rfloor$。对于回文半径 $|S|=\lfloor\frac{|S^\#|}{2}\rfloor$ 

### 用法

- 求每个回文中心的回文半径
- 求本质不同回文串：在`Manacher`中，新的回文串一定出现在使得**最右串右移**的时候，因此本质不同回文串至多$n$个，把所有更新最右回文串去重即得到本质不同回文串

### 实现

- `r`：回文串最右端点， `p`：最右端点对应的回文中心
- `get(k, x)`：求中心 `k` 的最长回文半径，初始长 `x`
- `i > r` 则 `r - i + 1 == 0` 符合初始为 0

```cpp
struct Manacher {
    int size;
    char s[N << 1], t[N];
    int p, r, len[N << 1];

    void get(int k, int x) {
        for (; 1 <= k - x && k + x <= size && s[k - x] == s[k + x]; x++);

        len[k] = x;
        if (k + x - 1 > r)
            r = k + x - 1, p = k;
    }

    void op() {
        size = p = r = 0;
        s[++size] = '#';
        for (int i = 1; t[i]; i++)
            s[++size] = t[i], s[++size] = '#';

        for (int i = 1; i <= size; i++)
            get(i, i > r ? 0 : min(len[p * 2 - i], r - i + 1));
    }
};
```

## 回文自动机

```cpp
struct PAM {
    char s[N];
    int size, last, cur, idx;
    int tr[N][26], len[N], ne[N];

    PAM() { idx = 2, len[1] = -1, ne[0] = 1; }

    int get_next(int x) {
        for (; s[cur] != s[cur - len[x] - 1]; x = ne[x]);
        return x;
    }

    void insert() {
        int now = get_next(last);
        int u = s[cur] - 'a';
        if (!tr[now][u]) {
            ne[idx] = tr[get_next(ne[now])][u];
            len[idx] = len[now] + 2;
            tr[now][u] = idx++;
        }

        last = tr[now][u];
    }

    void op() {
        size = strlen(s + 1);
        last = cur = 1;
        for (; cur <= size; cur++)
            insert();
    }
};
```

### 最小回文划分
```cpp
for (int i = last; i > 1; i = slink[i])
{
    sf[i] = f[tot - (len[slink[i]] + dif[i])] % mod;
    if (dif[i] == dif[ne[i]])
        sf[i] = (sf[i] + sf[ne[i]]) % mod;
}
```

## 后缀数组

- “区间可加性”：
    有一组排序过的字符串$A=[A_1,A_2,\cdots,A_n]$，对于任意的$k\in[i,j]$
    $$
    LCP(A_i A_j)=min(LCP(A_i,A_{i+1}),\cdots,LCP(A_{j-1},A_j))
    $$
- `Height`数组：$Height[i]\geq Height[i-1]-1$
- 多字符串拼接一定要插入一个不在字符集的字符，如果是多于两个字符串，还需要存每个位置原本的后缀长度

```cpp
struct SA
{
    int size;
    char s[N];
    int sa[N], rk[N], oldrk[N << 1], cnt[N], id[N], height[N];

    void get(int k, int x)
    {
        for (; sa[rk[k] - 1] + x <= size && k + x <= size; x++)
            if (s[sa[rk[k] - 1] + x] != s[k + x])
                break;

        height[rk[k]] = x;
    }

    void op()
    {
        size = strlen(s + 1);
        int m = 127;

        for (int i = 1; i <= size; i++)
            cnt[oldrk[i] = s[i]]++;
        for (int i = 1; i <= m; i++)
            cnt[i] += cnt[i - 1];
        for (int i = size; i; i--)
            sa[cnt[oldrk[i]]--] = i;

        for (int i = 1, p = 0; i <= size; i++, m = p)
            if (oldrk[sa[i]] == oldrk[sa[i - 1]])
                rk[sa[i]] = p;
            else
                rk[sa[i]] = ++p;

        for (int w = 1, i, p; w < size; w <<= 1, m = p)
        {
                // 第一次按关键字排序并不要求顺序
            for (i = size, p = 1; i > size - w; i--)
                id[p++] = i;
            for (i = 1; i <= size; i++)
                if (sa[i] > w)
                    id[p++] = sa[i] - w;

            memset(cnt + 1, 0, sizeof(int) * m);
            for (i = 1; i <= size; i++)
                cnt[rk[i]]++;
            for (i = 1; i <= m; i++)
                cnt[i] += cnt[i - 1];
            for (i = size; i; i--)
                sa[cnt[rk[id[i]]]--] = id[i];

            memcpy(oldrk + 1, rk + 1, sizeof(int) * size);
            for (i = 1, p = 0; i <= size; i++)
                if (oldrk[sa[i]] == oldrk[sa[i - 1]] &&
                    oldrk[sa[i] + w] == oldrk[sa[i - 1] + w])
                    rk[sa[i]] = p;
                else
                    rk[sa[i]] = ++p;

            if (p == size)
                break;
        }

        for (int i = 1; i <= size; i++)
            get(i, max(0, height[rk[i - 1]] - 1));
    }
};
```

## 后缀自动机
```cpp
struct SAM
{
    int size;
    char s[N];
    int last, idx;
    int tr[N][26], ne[N], len[N], cnt[N];
    vector<int> link[N];

    SAM()
    {
        ne[0] = -1;
        idx = 1;
    }

    void insert(int k)
    {
        int p = last, u = s[k] - 'a';
        len[idx] = len[last] + 1;
        last = idx++;
        cnt[last]++;

        while (~p && !tr[p][u])
        {
            tr[p][u] = last;
            p = ne[p];
        }

        if (p == -1)
            return;

        int q = tr[p][u];
        if (len[p] + 1 == len[q])
            ne[last] = q;
        else
        {
            memcpy(tr[idx], tr[q], sizeof tr[q]);
            len[idx] = len[p] + 1;
            ne[idx] = ne[q];
            ne[q] = ne[last] = idx;

            while (~p && tr[p][u] == q)
            {
                tr[p][u] = idx;
                p = ne[p];
            }

            idx++;
        }
    }

    void dfs(int u)
    {
        for (auto son : link[u])
        {
            dfs(son);
            cnt[u] += cnt[son];
        }
    }

    void op()
    {
        size = strlen(s + 1);
        for (int i = 1; i <= size; i++)
            insert(i);

        for (int i = 1; i < idx; i++)
            link[ne[i]].push_back(i);

        dfs(0);
    }
};
```
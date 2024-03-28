- [数据结构](#数据结构)
  - [LCA](#lca)
  - [树状数组](#树状数组)
    - [普通](#普通)
    - [区间加](#区间加)
  - [ST表](#st表)
  - [Splay](#splay)
    - [启发式合并](#启发式合并)
  - [莫队](#莫队)
  - [树链剖分](#树链剖分)
  - [LCT](#lct)
    - [将边权转化为点权](#将边权转化为点权)
  - [DLX](#dlx)
  - [左偏树](#左偏树)
  - [点分治](#点分治)
  - [点分树](#点分树)
  - [CDQ分治](#cdq分治)
  - [Dsu on tree](#dsu-on-tree)
  - [用两个优先队列实现可删除堆](#用两个优先队列实现可删除堆)

# 数据结构

## LCA

```cpp
struct LCA {
    vector<int> h[N];
    int fa[N][19];

    void dfs(int u) {
        for (auto sn : h[u]) {
            fa[sn][0] = u;
            for (int i = 1; i < 19; i++)
                fa[sn][i] = fa[fa[sn][i - 1]][i - 1];
            dfs(sn);
        }
    }
};
```

## 树状数组

### 普通

```cpp
struct BIT {
    int tr[N];

    void add(int k, int x) {
        for (int i = k; i < N; i += i & -i)
            tr[i] += x;
    }

    int query(int k) {
        int res = 0;
        for (int i = k; i; i -= i & -i)
            res += tr[i];
        return res;
    }

    int query_rev(int k) { return query(N - 1) - query(k - 1); }

    int query_seg(int l, int r) { return query(r) - query(l - 1); }
};
```

### 区间加

```cpp
struct SegBIT {
    struct BIT {
        vector<int> tr;

        void init(int n) { tr.assign(n + 10, 0); }

        void add(int k, int x) {
            for (int i = k; i < tr.size(); i += i & -i)
                tr[i] += x;
        }

        int query(int k) {
            int res = 0;
            for (int i = k; i; i -= i & -i)
                res += tr[i];
            return res;
        }
    }bit1, bit2;

    void init(int n) { bit1.init(n), bit2.init(n); }

    void add(int l, int r, int x) {
        bit1.add(l, x), bit1.add(r + 1, -x);
        bit2.add(l, x * l), bit2.add(r + 1, -x * (r + 1));
    }

    int query(int l, int r) {
        return bit1.query(r) * (r + 1) - bit2.query(r) -
            (bit1.query(l - 1) * l - bit2.query(l - 1));
    }
};
```

## ST表

```cpp
struct ST {
    static constexpr int M = 25;
    static inline int log2[N];
    static inline int init = [] {
        for (int i = 2; i < N; i++)
            log2[i] = log2[i / 2] + 1;
        return 0;
    }();

    int f[N][M];

    void op() {
        for (int i = 1; i <= n; i++)
            f[i][0] = a[i];

        for (int j = 1; j < M; j++)
            for (int i = 1; i + (1 << j) - 1 <= n; i++)
                f[i][j] = max(f[i][j - 1], f[i + (1 << j - 1)][j - 1]);
    }

    int query(int l, int r) {
        int t = log2[r - l + 1];
        return max(f[l][t], f[r - (1 << t) + 1][t]);
    }
};
```

## Splay
- `splay`和线段树最大的不同就是根节点也有一个数
```cpp
struct Splay
{
    struct Node
    {
        int s[2], p;
        int v, size;
        int lazy;

        void init(int _p, int _v)
        {
            p = _p;
            v = _v;
            size = 1;
        }
    }tr[N << 1];
    int root, idx;

    void add_rev(int u)
    {
        if (!u)
            return;
        swap(tr[u].s[0], tr[u].s[1]);
        tr[u].lazy ^= 1;
    }

    void push_up(int u)
    {
        tr[u].size = tr[tr[u].s[0]].size + tr[tr[u].s[1]].size + 1;
    }

    void push_down(int u)
    {
        if (tr[u].lazy)
        {
            add_rev(tr[u].s[0]);
            add_rev(tr[u].s[1]);
            tr[u].lazy ^= 1;
        }
        // 如果是求和一类的需要判断子节点是否为0
    }

    void rotate(int x)
    {
        int y = tr[x].p, z = tr[y].p;
        int k = (tr[y].s[1] == x);

        tr[z].s[tr[z].s[1] == y] = x, tr[x].p = z;
        tr[y].s[k] = tr[x].s[k ^ 1], tr[tr[x].s[k ^ 1]].p = y;
        tr[x].s[k ^ 1] = y, tr[y].p = x;

        push_up(y), push_up(x);
    }

    void splay(int x, int k)
    {
        while (tr[x].p != k)
        {
            int y = tr[x].p, z = tr[y].p;
            if (z != k)
                if ((tr[y].s[1] == x) ^ (tr[z].s[1] == y))
                    rotate(x);
                else
                    rotate(y);
            rotate(x);
        }

        if (!k)
            root = x;
    }

    // 以下所有操作都要加上push_down
    int get(int k)
    {
        int u = root;
        while (u)
        {
            if (tr[tr[u].s[0]].size >= k)
                u = tr[u].s[0];
            else if (tr[tr[u].s[0]].size + 1 == k)
                break;
            else
            {
                k -= tr[tr[u].s[0]].size + 1;
                u = tr[u].s[1];
            }
        }

        return u ? u : -1;
    }

    int get_k(int x)  // 需要保证x存在
    {
        int u = root, cnt = 0;
        while (true)
        {
            if (tr[u].v < x)
            {
                cnt += tr[tr[u].s[0]].size + 1;
                u = tr[u].s[1];
            }
            else if (tr[u].v == x)
                return cnt + tr[tr[u].s[0]].size + 1;
            else
                u = tr[u].s[0];
        }
    }

    void insert1(int x)  // 按值大小插入
    {
        int p = 0, u = root;
        while (u)
            p = u, u = tr[u].s[tr[u].v < x];

        tr[p].s[tr[p].v < x] = u = ++idx;
        tr[u].init(p, x);
        splay(u, 0);
    }

    void insert2(int x)  // 插入到最后
    {
        int p = 0, u = root;
        while (u)
            p = u, u = tr[u].s[1];

        tr[p].s[1] = u = ++idx;
        tr[u].init(p, x);
        splay(u, 0);
    }

    void insert3(int k, int x)  // 插入到第k个数后
    {                           // 一般需要有哨兵
        int l = get(k + 1), r = get(k + 2);
        splay(l, 0), splay(r, l);

        tr[r].s[0] = k = ++idx;
        tr[k].init(r, x);
        push_up(r), push_up(l);
    }

    void erase(int x)
    {
        int k = get_k(x);
        int l = get(k - 1), r = get(k + 1);

        splay(l, 0), splay(r, l);
        tr[r].s[0] = 0;
        push_up(r), push_up(l);
    }

    int get_pre(int u, int x)
    {
        if (!u)
            return -INF;

        if (tr[u].v > x)
            return get_pre(tr[u].s[0], x);
        else
            return max(tr[u].v, get_pre(tr[u].s[1], x));
    }

    int get_aft(int u, int x)
    {
        if (!u)
            return INF;

        if (tr[u].v < x)
            return get_aft(tr[u].s[1], x);
        else
            return min(tr[u].v, get_aft(tr[u].s[0], x));
    }

    void print(int u)
    {
        if (!u)
            return;

        print(tr[u].s[0]);
        cout << tr[u].v << " ";
        print(tr[u].s[1]);
    }
};
```
### 启发式合并
- 每次合并两个集合时，将**较小的**集合合并到**较大的**集合，时间复杂度为$O(n\log n)$

## 莫队
```cpp
namespace
{
    int part, sum;
    int cnt[N], res[N];

    int get(int x)
    {
        return x / part;
    }

    struct Query
    {
        int l, r, idx;

        bool operator<(const Query &y) const
        {
            if (get(l) != get(y.l))
                return get(l) < get(y.l);
            return r < y.r;
        }
    }q[N];

    void add(int k)
    {
        int x = a[k];
    }

    void del(int k)
    {
        int x = a[k];
    }

    void solve()
    {
        part = sqrt(n);
        sort(q + 1, q + 1 + m);

        int l = 1, r = 0;
        for (int i = 1; i <= m; i++)
        {
            auto &t = q[i];

            while (l < t.l)
                del(l++);
            while (t.l < l)
                add(--l);
            while (r < t.r)
                add(++r);
            while (t.r < r)
                del(r--);

            res[t.idx] = sum;
        }

        for (int i = 1; i <= m; i++)
            cout << res[i] << "\n";
    }
}
```

## 树链剖分

```cpp
// 树链剖分部分
struct QTree {
    vector<int> h[N];
    int dfn[N], first[N], last[N], idx;
    int size[N], depth[N], fa[N], top[N], son[N];

    void dfs1(int u, int p) {
        size[u] = 1, depth[u] = depth[p] + 1, fa[u] = p;
        for (auto sn : h[u]) {
            if (sn == p)
                continue;

            dfs1(sn, u);
            size[u] += size[sn];

            if (size[sn] > size[son[u]])
                son[u] = sn;
        }
    }

    void dfs2(int u, int t) {
        top[u] = t, dfn[++idx] = u, first[u] = idx;

        if (son[u]) {
            dfs2(son[u], t);
            for (auto sn : h[u]) {
                if (sn == fa[u] || sn == son[u])
                    continue;

                dfs2(sn, sn);
            }
        }

        last[u] = idx;
    }

    void init() {
        for (int i = 1; i <= idx; i++)
            h[i].clear(), son[i] = 0;
        idx = 0;
    }

    void op() {
        dfs1(1, 0);
        dfs2(1, 1);
    }
};

// 线段树查询部分
void modify_path(int u, int v, int x)
{
    auto &q = qtree;
    while (q.top[u] != q.top[v])
    {
        if (q.depth[q.top[u]] < q.depth[q.top[v]])
            swap(u, v);

        modify(1, q.first[q.top[u]], q.first[u], x);
        u = q.fa[q.top[u]];
    }

    if (q.depth[u] > q.depth[v])
        swap(u, v);
    modify(1, q.first[u], q.first[v], x);
}

Val query_path(int u, int v)
{
    auto &q = qtree;
    Val res = {};
    while (q.top[u] != q.top[v])
    {
        if (q.depth[q.top[u]] < q.depth[q.top[v]])
            swap(u, v);

        res = merge(res, query(1, q.first[q.top[u]], q.first[u]));
        u = q.fa[q.top[u]];
    }

    if (q.depth[u] > q.depth[v])
        swap(u, v);
    res = merge(res, query(1, q.s[q.first[u]], q.s[q.first[v]]));

    return res;
}
```

## LCT
```cpp
struct LCT
{
    struct Node
    {
        int s[2], v, p;
        int sum, lazy_rev;
    }tr[N];

    void push_up(int u)
    {
        auto &p = tr[u], &l = tr[p.s[0]], &r = tr[p.s[1]];
        p.sum = p.v ^ l.sum ^ r.sum;
    }

    void push_down(int u)
    {       // 如果是求和一类的需要判断子节点是否为0
        auto &p = tr[u];
        if (p.lazy_rev)
        {
            add_rev(p.s[0]);
            add_rev(p.s[1]);
            p.lazy_rev = 0;
        }
    }

    void add_rev(int u)
    {
        swap(tr[u].s[0], tr[u].s[1]);
        tr[u].lazy_rev ^= 1;
    }

    bool is_root(int u)
    {
        int t = tr[u].p;
        return tr[t].s[0] != u && tr[t].s[1] != u;
    }

    void rotate(int x)
    {
        int y = tr[x].p, z = tr[y].p;
        int k = tr[y].s[1] == x;

        if (!is_root(y))
            tr[z].s[tr[z].s[1] == y] = x;
        tr[x].p = z;
        tr[y].s[k] = tr[x].s[k ^ 1], tr[tr[x].s[k ^ 1]].p = y;
        tr[x].s[k ^ 1] = y, tr[y].p = x;

        push_up(y), push_up(x);
    }

    void splay(int x)
    {
        int u = x;
        while (!is_root(u))
        {
            stk[top++] = u;
            u = tr[u].p;
        }
        stk[top++] = u;
        while (top)
            push_down(stk[--top]);

        while (!is_root(x))
        {
            int y = tr[x].p, z = tr[y].p;
            if (!is_root(y))
                if ((tr[y].s[1] == x) ^ (tr[z].s[1] == y))
                    rotate(x);
                else
                    rotate(y);
            rotate(x);
        }
    }

    void access(int y)
    {
        int t = y;
        for (int x = 0; y; x = y, y = tr[y].p)
        {
            splay(y);
            tr[y].s[1] = x;
            push_up(y);
        }

        splay(t);
    }

    void make_root(int u)
    {
        access(u);
        add_rev(u);
    }

    int find_root(int u)
    {
        access(u);
        while (tr[u].s[0])
            u = tr[u].s[0];

        splay(u);
        return u;
    }

    void split(int x, int y)
    {
        make_root(x);
        access(y);
    }

    void link(int x, int y)
    {
        make_root(x);
        if (find_root(y) != x)
            tr[x].p = y;
    }

    void cut(int x, int y)
    {
        make_root(x);
        if (find_root(y) == x && tr[x].s[1] == y && !tr[y].s[0])
        {
            tr[x].s[1] = 0, tr[y].p = 0;
            push_up(x);
        }
    }
};
```

### 将边权转化为点权

- 将边的两个顶点连接一个新的点，并且该点权值为原来边的值

## DLX

- 精确覆盖

```cpp
struct DLX {
    struct Node {
        int d, u, l, r;
        int row, col;
        int cnt;
    }nd[N * 11];
    int idx;
    int res[N], top;

    void init() {
        for (int i = 0; i <= m; i++)
            nd[i].l = i - 1, nd[i].r = i + 1, nd[i].u = nd[i].d = i;
        nd[0].l = m, nd[m].r = 0;
        idx = m + 1;
    }

    void add(int &hh, int &tt, int x, int y) {
        auto &ndi = nd[idx], &ndy = nd[y];

        ndi.row = x, ndi.col = y, ndy.cnt++;
        ndi.u = y, ndi.d = ndy.d, ndy.d = nd[ndi.d].u = idx;
        ndi.l = hh, ndi.r = tt, nd[hh].r = nd[tt].l = idx;
        hh = idx++;
    }

    void remove(int p) {
        nd[nd[p].r].l = nd[p].l, nd[nd[p].l].r = nd[p].r;
        for (int i = nd[p].d; i != p; i = nd[i].d)
            for (int j = nd[i].r; j != i; j = nd[j].r) {
                nd[nd[j].u].d = nd[j].d, nd[nd[j].d].u = nd[j].u;
                nd[nd[j].col].cnt--;
            }
    }

    void resume(int p) {
        for (int i = nd[p].u; i != p; i = nd[i].u)
            for (int j = nd[i].l; j != i; j = nd[j].l) {
                nd[nd[j].col].cnt++;
                nd[nd[j].u].d = nd[nd[j].d].u = j;
            }
        nd[nd[p].l].r = nd[nd[p].r].l = p;
    }

    bool dfs() {
        if (!nd[0].r)
            return true;

        int p = nd[0].r;
        for (int i = p; i; i = nd[i].r)
            if (nd[i].cnt < nd[p].cnt)
                p = i;

        remove(p);
        for (int i = nd[p].d; i != p; i = nd[i].d) {
            res[++top] = nd[i].row;
            for (int j = nd[i].r; j != i; j = nd[j].r)
                remove(nd[j].col);

            if (dfs())
                return true;

            for (int j = nd[i].l; j != i; j = nd[j].l)
                resume(nd[j].col);
            top--;
        }
        resume(p);

        return false;
    }
};
```

- 重复覆盖

```cpp
struct DLX {
    struct Node {
        int u, d, l, r;
        int row, col;
        int cnt;
    }nd[N * N];
    int idx, max_depth;
    int res[N];
    bool book[N];

    void init() {
        for (int i = 0; i <= m; i++)
            nd[i].l = i - 1, nd[i].r = i + 1, nd[i].u = nd[i].d = i;
        nd[0].l = m, nd[m].r = 0;
        idx = m + 1;
    }

    void add(int &hh, int &tt, int x, int y) {
        auto &ndi = nd[idx], &ndy = nd[y];

        ndi.row = x, ndi.col = y, ndy.cnt++;
        ndi.u = y, ndi.d = ndy.d, ndy.d = nd[ndi.d].u = idx;
        ndi.l = hh, ndi.r = tt, nd[hh].r = nd[tt].l = idx;
        hh = idx++;
    }

    void remove(int p) {
        for (int i = nd[p].d; i != p; i = nd[i].d)
            nd[nd[i].r].l = nd[i].l, nd[nd[i].l].r = nd[i].r;
    }

    void resume(int p) {
        for (int i = nd[p].u; i != p; i = nd[i].u)
            nd[nd[i].r].l = nd[nd[i].l].r = i;
    }

    int val() {
        memset(book, 0, sizeof book);
        int cnt = 0;
        for (int i = nd[0].r; i; i = nd[i].r)
            if (!book[i]) {
                book[i] = true;
                for (int j = nd[i].d; j != i; j = nd[j].d)
                    for (int k = nd[j].r; k != j; k = nd[k].r)
                        book[nd[k].col] = true;
                cnt++;
            }

        return cnt;
    }

    bool dfs(int depth) {
        if (depth + val() > max_depth)
            return false;

        if (!nd[0].r)
            return true;

        int p = nd[0].r;
        for (int i = p; i; i = nd[i].r)
            if (nd[i].cnt < nd[p].cnt)
                p = i;

        for (int i = nd[p].d; i != p; i = nd[i].d) {
            res[depth + 1] = nd[i].row;
            remove(i);
            for (int j = nd[i].r; j != i; j = nd[j].r)
                remove(j);

            if (dfs(depth + 1))
                return true;

            for (int j = nd[i].l; j != i; j = nd[j].l)
                resume(j);
            resume(i);
        }

        return false;
    }
};
```

## 左偏树

```cpp
struct LeftTree
{
    struct Node
    {
        int l, r;
        int v, dis;
    }tr[N];
    int idx;

    LeftTree() { tr[0].v = 2e9; }

    bool cmp(int x, int y)
    {
        if (tr[x].v != tr[y].v)
            return tr[x].v < tr[y].v;
        return x < y;
    }

    int merge(int x, int y)
    {
        if (!x || !y)
            return x + y;

        if (cmp(y, x))
            swap(x, y);
        tr[x].r = merge(tr[x].r, y);
        if (tr[tr[x].l].dis < tr[tr[x].r].dis)
            swap(tr[x].l, tr[x].r);
        tr[x].dis = tr[tr[x].r].dis + 1;

        return x;
    }
};
```

## 点分治

- 性质：重心的所有子树的点数$\leq n/2$

```cpp
bool book[N];

int get_size(int u, int p)
{
    if (book[u])
        return 0;

    int sum = 1;
    for (auto sn : h[u])
    {
        if (sn == p)
            continue;

        sum += get_size(sn, u);
    }

    return sum;
}

int get_wc(int u, int p, int tot, int &wc)
{
    if (book[u])
        return 0;

    int sum = 1, maxv = 0;
    for (auto sn : h[u])
    {
        if (sn == p)
            continue;

        int t = get_wc(sn, u, tot, wc);
        sum += t;
        maxv = max(maxv, t);
    }

    maxv = max(maxv, tot - sum);
    if (maxv <= tot >> 1)
        wc = u;

    return sum;
}

void get(int u, int p, int depth, Node q[], int &qt)
{
    if (book[u])
        return;

    q[++qt] = { a[u], a[u] * depth % mod, depth };

    for (auto sn : h[u])
    {
        if (sn == p)
            continue;

        get(sn, u, depth + 1, q, qt);
    }
}

int calc(int u)
{
    if (book[u])
        return 0;

    get_wc(u, 0, get_size(u, 0), u);
    book[u] = true;

    int pt = 0, res = 0;
    for (auto sn : h[u])
    {
        int qt = 0;
        get(sn, u, 1, q, qt);
        sort(q + 1, q + 1 + qt);

        for (int i = 1; i <= qt; i++)
            p[++pt] = q[i];

        for (int i = 1; i <= qt; i++)
        {
            q[i].v = (q[i].v + q[i - 1].v) % mod;
            q[i].mul = (q[i].mul + q[i - 1].mul) % mod;
        }
        for (int i = qt; i; i--)
        {
            auto &t = q[i];
            res = (res - t.mul - t.v * t.dep) % mod;
        }
    }

    for (int i = 1; i <= pt; i++)
        res = (res + min(a[u], p[i].v) * p[i].dep) % mod;

    sort(p + 1, p + 1 + pt);
    for (int i = 1; i <= pt; i++)
    {
        p[i].v = (p[i].v + p[i - 1].v) % mod;
        p[i].mul = (p[i].mul + p[i - 1].mul) % mod;
    }
    for (int i = pt; i; i--)
    {
        auto &t = p[i];
        res = (res + t.mul + t.v * t.dep) % mod;
    }

    for (auto sn : h[u])
        res = (res + calc(sn)) % mod;

    return res;
}
```

## 点分树

```cpp
namespace
{
    int age[N];
    bool book[N];

    struct Parent
    {
        int idx, id;
        ll dist;
    };
    vector<Parent> parent[N];
    struct Son
    {
        int age;
        ll dist;

        bool operator<(const Son &y) const
        {
            return age < y.age;
        }
    };
    vector<Son> son[N][3];

    void add(int a, int b, int c)
    {
        e[idx] = b, ne[idx] = h[a], w[idx] = c, h[a] = idx++;
    }

    int get_size(int u, int p)
    {
        if (book[u])
            return 0;

        int sum = 1;
        for (int i = h[u]; ~i; i = ne[i])
        {
            int j = e[i];
            if (j == p)
                continue;
            sum += get_size(j, u);
        }

        return sum;
    }

    int get_wc(int u, int p, int tot, int &wc)
    {
        if (book[u])
            return 0;

        int sum = 1, ms = 0;
        for (int i = h[u]; ~i; i = ne[i])
        {
            int j = e[i];
            if (j == p)
                continue;

            int t = get_wc(j, u, tot, wc);
            ms = max(ms, t);
            sum += t;
        }

        ms = max(ms, tot - sum);
        if (ms <= tot >> 1)
            wc = u;

        return sum;
    }

    void get_dist(int u, int p, int root, int id, ll dist)
    {
        if (book[u])
            return;

        parent[u].push_back({ root, id, dist });
        son[root][id].push_back({ age[u], dist });

        for (int i = h[u]; ~i; i = ne[i])
        {
            int j = e[i];
            if (j == p)
                continue;
            get_dist(j, u, root, id, dist + w[i]);
        }
    }

    void calc(int u)
    {
        if (book[u])
            return;

        get_wc(u, -1, get_size(u, -1), u);
        book[u] = true;

        for (int i = h[u], k = 0; ~i; i = ne[i], k++)
        {
            int j = e[i];
            if (book[j])
                continue;

            get_dist(j, u, u, k, w[i]);

            auto &t = son[u][k];
            t.push_back({ -1, 0 }), t.push_back({ A + 1, 0 });
            sort(t.begin(), t.end());
            for (int i = 1; i < t.size(); i++)
                t[i].dist += t[i - 1].dist;
        }

        for (int i = h[u]; ~i; i = ne[i])
            calc(e[i]);
    }
};
```

## CDQ分治

- 需要添加 BIT

```cpp
struct CDQ {
    // bit
    struct Node {
        int a, b, c, res, cnt;

        bool operator<(const Node &y) const {
            if (a != y.a)
                return a < y.a;
            if (b != y.b)
                return b < y.b;
            return c < y.c;
        }

        bool operator==(const Node &y) const { return a == y.a && b == y.b && c == y.c; }
    }nd[N];
    int idx;

    void cdq(int l, int r) {
        if (l == r)
            return;

        int mid = l + r >> 1;
        cdq(l, mid), cdq(mid + 1, r);

        int i, j;
        for (i = l, j = mid + 1; j <= r; j++) {
            for (; i <= mid && nd[i].b <= nd[j].b; i++)
                bit.add(nd[i].c, nd[i].cnt);
            nd[j].res += bit.query(nd[j].c);
        }

        for (int k = l; k < i; k++)
            bit.add(nd[k].c, -nd[k].cnt);

        inplace_merge(nd + l, nd + mid + 1, nd + r + 1, [](const Node &x, const Node &y) {
            return x.b < y.b;
            });
    }

    void op() {
        sort(nd + 1, nd + 1 + n);

        for (int i = 1; i <= n; i++)
            if (nd[i - 1] == nd[i])
                nd[idx].cnt++;
            else
                nd[++idx] = nd[i], nd[idx].cnt = 1;

        cdq(1, idx);
    }
};
```

## Dsu on tree
```cpp
bool book[N];
void dfs(int u, bool del) {
    if (del) {
        book[u] = false;
        // 删除
    } else {
        book[u] = true;
        // 添加
    }

    for (auto sn : qt.h[u]) {
        if (sn == qt.fa[u])
            continue;

        if (del == book[sn])
            dfs(sn, del);
    }
}

void solve() {
    for (int i = n; i; i--) {
        int u = qt.dfn[i];
        dfs(u, false);

        // 查询

        if (qt.top[u] == u)
            dfs(u, true);
    }
}
```

## 用两个优先队列实现可删除堆
```cpp
struct Heap {
    priority_queue<int> heap, del;

    int top() {
        while (heap.size() && del.size() && heap.top() == del.top())
            heap.pop(), del.pop();

        return heap.size() ? heap.top() : 0;
    }

    void add(int x) { heap.push(x); }

    void remove(int x) { del.push(x); }
};
```
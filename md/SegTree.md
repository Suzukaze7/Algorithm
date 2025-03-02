- [线段树](#线段树)
  - [普通build](#普通build)
  - [扫描线](#扫描线)
  - [主席树](#主席树)
  - [动态开点](#动态开点)


# 线段树

## 普通build

```cpp
struct SegTree {
    struct Val { };
    struct Lazy { };

    struct Node {
        int l, r;
        Val val;
        Lazy lazy;
    }tr[N << 2];

    // 区间合并
    friend Val operator+(const Val &lhs, const Val &rhs) {
        return {};
    }

    // 加懒标记
    friend void operator+=(Node &lhs, const Lazy &rhs) {
    }

    void push_up(int u) {
        auto &p = tr[u], &l = tr[u << 1], &r = tr[u << 1 | 1];
        p.val = l.val + r.val;
    }

    void push_down(int u) {
        auto &p = tr[u];
        tr[u << 1] += p.lazy;
        tr[u << 1 | 1] += p.lazy;
        p.lazy = {};
    }

    void build(int u, int l, int r) {
        tr[u] = { l, r };

        if (l == r) {
            return;
        }

        int mid = l + r >> 1;
        build(u << 1, l, mid);
        build(u << 1 | 1, mid + 1, r);

        push_up(u);
    }

    void modify(int u, int l, int r, const Lazy &x) {
        if (l <= tr[u].l && tr[u].r <= r) {
            tr[u] += x;
            return;
        }

        push_down(u);

        int mid = tr[u].l + tr[u].r >> 1;
        if (l <= mid)
            modify(u << 1, l, r, x);
        if (mid < r)
            modify(u << 1 | 1, l, r, x);

        push_up(u);
    }

    Val query(int u, int l, int r) {
        if (l <= tr[u].l && tr[u].r <= r)
            return tr[u].val;

        push_down(u);

        int mid = tr[u].l + tr[u].r >> 1;
        Val res = {};
        if (l <= mid)
            res = query(u << 1, l, r);
        if (mid < r)
            res = res + query(u << 1 | 1, l, r);

        return res;
    }
};
```

## 扫描线

```cpp
struct SegTree {
    struct Node {
        int l, r;
        int cnt, sum;
    }tr[N << 4];

    void add(int u, int x) {
        auto &p = tr[u], &l = tr[u << 1], &r = tr[u << 1 | 1];
        p.cnt += x;
        if (p.cnt)
            p.sum = p.r - p.l + 1;
        else
            p.sum = l.sum + r.sum;
    }

    void push_up(int u) { add(u, 0); }

    void build(int u, int l, int r) {
        tr[u] = { l, r };

        if (l == r)
            return;

        int mid = l + r >> 1;
        build(u << 1, l, mid);
        build(u << 1 | 1, mid + 1, r);
    }

    void modify(int u, int l, int r, int x) {
        if (l <= tr[u].l && tr[u].r <= r) {
            add(u, x);
            return;
        }

        int mid = tr[u].l + tr[u].r >> 1;
        if (l <= mid)
            modify(u << 1, l, r, x);
        if (mid < r)
            modify(u << 1 | 1, l, r, x);

        push_up(u);
    }

    int query() { return tr[1].sum; }
};
```

## 主席树

```cpp
struct SegTree {
    struct Val { };
    struct Node {
        int lc, rc;
        Val val;
    } tr[N * 25];
    int root[N], idx;

    friend Val operator+(const Val &lhs, const Val &rhs) {
        return {};
    }

    void push_up(int u) {
        auto &p = tr[u], &l = tr[p.lc], &r = tr[p.rc];
        p.val = l.val + r.val;
    }

    void build(int &u, int l, int r) {
        u = ++idx;
        if (l == r)
            return;

        int mid = l + r >> 1;
        build(tr[u].lc, l, mid), build(tr[u].rc, mid + 1, r);
    }

    void insert(int &u, int v, int l, int r, int k, const Val &val) {
        u = ++idx;
        if (l == r) {
            tr[u].val = val;
            return;
        }

        int mid = l + r >> 1;
        if (k <= mid) {
            tr[u].rc = tr[v].rc;
            insert(tr[u].lc, tr[v].lc, l, mid, k, val);
        } else {
            tr[u].lc = tr[v].lc;
            insert(tr[u].rc, tr[v].rc, mid + 1, r, k, val);
        }

        push_up(u);
    }

    Val query(int u, int l, int r, int ql, int qr) {
        if (ql <= l && r <= qr)
            return tr[u].val;

        int mid = l + r >> 1;
        Val res = {};
        if (ql <= mid)
            res = query(tr[u].lc, l, mid, ql, qr);
        if (mid < qr)
            res = res + query(tr[u].rc, mid + 1, r, ql, qr);
        return res;
    }
};

// 二分求第 k 小
int query(int u, int v, int l, int r, int k) {
    if (l == r)
        return l;

    int mid = l + r >> 1, t = mid - l + 1 - (tr[tr[u].lc].sum - tr[tr[v].lc].sum);
    if (k <= t)
        return query(tr[u].lc, tr[v].lc, l, mid, k);
    else
        return query(tr[u].rc, tr[v].rc, mid + 1, r, k - t);
}

//树链剖分
Val query_path(int rt, int u, int v) {
    Val res = {};
    while (qt.top[u] != qt.top[v]) {
        if (qt.depth[qt.top[u]] < qt.depth[qt.top[v]])
            swap(u, v);

        res = res + query(root[rt], 1, n, qt.first[qt.top[u]], qt.first[u]);
        u = qt.fa[qt.top[u]];
    }

    if (qt.depth[u] > qt.depth[v])
        swap(u, v);
    res = res + query(root[rt], 1, n, qt.first[u], qt.first[v]);
    // 边权
    // res = res + query(root[rt], 1, n, qt.first[qt.son[u]], qt.first[v]);

    return res;
}
```

## 动态开点

- 单点修改，区间查询

```cpp
struct SegTree {
    struct Val { };

    struct Node {
        int lc, rc;
        Val val;
    }tr[N * 40];
    int root, idx;

    friend Val operator+(const Val &lhs, const Val &rhs) {
        return { };
    }

    void modify(int &u, int l, int r, int k, const Val &val) {
        if (!u)
            u = ++idx;

        if (l == r) {
            tr[u].val = tr[u].val + val;
            return;
        }

        auto &p = tr[u];
        int mid = l + r >> 1;
        if (k <= mid)
            modify(p.lc, l, mid, k, val);
        else
            modify(p.rc, mid + 1, r, k, val);

        auto &lc = tr[p.lc], &rc = tr[p.rc];
        p.val = lc.val + rc.val;
    }

    Val query(int u, int l, int r, int ql, int qr) {
        if (!u)
            return {};
        if (ql <= l && r <= qr)
            return tr[u].val;

        int mid = l + r >> 1;
        Val res = {};
        if (l <= mid)
            res = query(tr[u].lc, l, mid, ql, qr);
        if (mid < r)
            res = res + query(tr[u].rc, mid + 1, r, ql, qr);

        return res;
    }
};
```

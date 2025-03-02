## 合法状态
- 唯一原则：可以转移到不合法状态，但是不能从不合法状态转移或者不合法状态没有影响

## 数位dp
- debug建议打表

## 基环树dp

- 无自环`i = (from ^ 1)`
- 有自环建一条边，并把自环点标记
- 如果不是基环树(即不止一个环)不能这样找
- 每次 `cidx` 置 0，通过 `cidx` 判断有无环

```cpp
int h[N], e[N << 1], ne[N << 1], idx;
bool book[N], ins[N];
int pu[N], pw[N];
int cir[N], ed[N], cidx;

void add(int a, int b) {
    e[idx] = b, ne[idx] = h[a], h[a] = idx++;
}

void dfs_c(int u, int from = -1) {
    book[u] = ins[u] = true;

    for (int i = h[u]; ~i; i = ne[i]) {
        int sn = e[i];
        if (i == (from ^ 1))
            continue;

        pu[sn] = u;

        if (!book[sn])
            dfs_c(sn, i);
        else if (ins[sn]) {
            for (int ne = u; ne != sn; ne = pu[ne])
                cir[++cidx] = ne;
            cir[++cidx] = sn;
        }
    }

    ins[u] = false;
}

// 多个环
cidx++;
ed[cidx] = ed[cidx - 1];
for (int ne = u; ne != sn; ne = pu[ne])
    cir[++ed[cidx]] = ne;
cir[++ed[cidx]] = sn;
```

### 四边形不等式dp
#### 二维
- 状态方程形如$f_i=\min\{f_j+v_{i,j}\}$，且$v_{i,j}$满足
  - 任取$a\leq b\leq c\leq d$，满足$v_{a,d}+v_{b,c}\geq v_{a,c}+v_{b,d}$
  - 任取$i<j$，有$i<i+1\leq j<j+1$，满足$v_{i,j+1}+v_{i+1,j}\geq v_{i,j}+v_{i+1,j+1}$
  
  则若通过$j$求出的$i$的值，比$j^`<j$都更好，则$j$对于$i^`>i$，都比$j^`<j$更好
- 也就是说，如果已经求出了$j^`<j$的当前最优解，则此时的$j$能更新的区间具有二分性

#### 三维
- 状态方程满足
  - 形如$f_{i,j}=\min\{f_{i,k}+f_{k+1,j}+w_{i,j}\}$
  - $f_{i,i}=w_{i,i}=0$
  - $w$满足四边形不等式
  - $\forall a\leq b\leq c\leq d, w{a,d}\geq w_{b,c}$
- 那么$f$也满足四边形不等式
- 令$p_{i,j}$表示$f_{i,j}$的最优解，有$p_{i,j-1}\leq p_{i,j}\leq p_{i+1,j}$
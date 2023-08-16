### 合法状态
- 唯一原则：可以转移到不合法状态，但是不能从不合法状态转移或者不合法状态没有影响

### 数位dp
- debug建议打表

### 基环树dp
- 无自环`i = (from ^ 1)`
- 有自环建反边，并把自环点标记
```cpp
bool book[N], ins[N];
int pu[N], pw[N];
int cir[N], ed[N], cnt;
int s[N];

void dfs_c(int u, int from)
{
    book[u] = ins[u] = true;

    for (int i = h[u]; ~i; i = ne[i])
    {
        int j = e[i];

        if (i == (from ^ 1))
            continue;

        pu[j] = u, pw[j] = w[i];

        if (!book[j])
            dfs_c(j, i);
        else if (ins[j])
        {
            cnt++;
            ed[cnt] = ed[cnt - 1];
            ll sum = w[i];
            for (int i = u; i != j; i = pu[i])
            {
                s[i] = sum;
                sum += pw[i];
                cir[++ed[cnt]] = i;
            }
            s[j] = sum, cir[++ed[cnt]] = j;
        }
    }

    ins[u] = false;
}
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
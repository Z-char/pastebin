#include <bits/stdc++.h>
#ifdef Z_char
    #include "Zdebug.h"
#else 
    unsigned int def_cnt = 0;
    struct Ccerr { template<class T> Ccerr& operator<<(T v) { return *this; } } ccerr;
    #define asert(...) ++def_cnt
    #define cktime(...) ++def_cnt
    #define erprint(...) ++def_cnt
#endif

template<class T> 
inline T read() {
    T x = 0; int f = 1; char ch = getchar();
    for (; !isdigit(ch); ch = getchar()) if (ch == '-') f = -1;
    for (;  isdigit(ch); ch = getchar()) x = x * 10 + ch - 48;
    return x * f;
}
template<class T> inline void read(T &x) { x = read<T>(); }
template<class T, class... Args> inline void read(T &fir, Args&... args) { read(fir), read(args...); }
template<class T> 
inline void write(T x, char ch = '\n') {
    static int stk[32];
    if (x < 0) putchar('-'), x = -x;
    int top = 0;
    do stk[++top] = x % 10, x /= 10; while(x);
    while (top) putchar(stk[top--] + 48);
    putchar(ch);
}
template<class T, class... Args> inline void write(T fir, Args... args) { write(fir, ' '), write(args...); }

const int Md = 998244353;

int n;

inline int add(int x, int y) {
    return (x + y >= Md) ? (x + y - Md) : (x + y);
}
inline int sub(int x, int y) {
    return (x - y < 0) ? (x - y + Md) : (x - y);
}
std::pair<int, int> operator+(std::pair<int, int> a, std::pair<int, int> b) {
    b.second = add(a.second, b.second);
    return b;
}

namespace acAuto {
    const int $N = 1e5 + 5;
    int ch[$N][10], fail[$N], sum[$N], idx;
    void ins(const std::string &str, int val) {
        int p = 0, N = (int)str.length();
        for (int i = 0; i < N; ++i) {
            int chx = str[i] - '0';
            if (!ch[p][chx]) {
                ch[p][chx] = ++idx;
            }
            p = ch[p][chx];
        }
        sum[p] = add(sum[p], val);
    }
    std::pair<int, int> bz[$N][10][20];
    void build() {
        std::queue<int> q;
        for (int i = 0; i < 10; ++i) {
            if (ch[0][i]) {
                q.push(ch[0][i]);
            }
        }
        while (q.size()) {
            int x = q.front(); q.pop();
            for (int i = 0; i < 10; ++i) {
                if (ch[x][i]) {
                    fail[ch[x][i]] = ch[fail[x]][i];
                    q.push(ch[x][i]);
                } else {
                    ch[x][i] = ch[fail[x]][i];
                }
            }
            sum[x] = add(sum[x], sum[fail[x]]);
        }

        for (int k = 0; k < 19; ++k) {
            for (int i = 0; i <= idx; ++i) {
                for (int j = 0; j < 10; ++j) {
                    if (k == 0) {
                        bz[i][j][k] = {ch[i][j], sum[ch[i][j]]};
                    } else {
                        bz[i][j][k] = bz[i][j][k - 1] + bz[bz[i][j][k - 1].first][j][k - 1];
                    }
                }
            }
        }
    }
    std::pair<int, int> jump(int x, int num, int stp) {
        std::pair<int, int> ret{x, 0};
        for (int j = 19; j >= 0; --j) {
            if ((stp >> j) & 1) {
                ret = ret + bz[ret.first][num][j];
            }
        }
        return ret;
    }
}

const int N = 3e5 + 5;

int m;
std::string s;
struct Stack {
    int l, r, num; std::pair<int, int> ret;
    bool operator<(Stack b) const {
        return l < b.l;
    }
} stk[N];
int stop;

signed main() {
    n = read<int>();
    for (int i = 1; i <= n; ++i) {
        std::string s; int w;
        std::cin >> s; w = read<int>();
        acAuto::ins(s, w);
    }
    acAuto::build();
    std::cin >> s;
    int N = (int)s.length();
    stk[0] = {0, 0, -1, {0, 0}};
    {
        std::pair<int, int> ret{0, 0};
        for (int i = 0, j; i < N; i = j + 1) {
            j = i;
            while (j + 1 < N && s[j + 1] == s[j]) {
                ++j;
            }
            stk[++stop] = {i, j, s[i] - '0', ret};
            ret = ret + acAuto::jump(ret.first, s[i] - '0', j - i + 1);
        }
    }
    m = read<int>();
    while (m--) {
        int ty = read<int>();
        if (ty == 1) {
            int l = N - read<int>(), r = N - 1, num = read<int>();
            while (stop && stk[stop].l >= l) {
                --stop;
            }
            if (stop && stk[stop].r >= l) {
                stk[stop].r = l - 1;
            }
            auto &pre = stk[stop];
            if (num == pre.num) {
                stk[stop].r = r;
            } else {
                stk[++stop] = {l, r, num, pre.ret + acAuto::jump(pre.ret.first, pre.num, pre.r - pre.l + 1)};
            }
        } else {
            int l = read<int>() - 1;
            Stack nw = {l, l, -1, {0, 0}}, fnd = *(std::upper_bound(stk + 1, stk + stop + 1, nw) - 1);
            std::pair<int, int> ret = fnd.ret + acAuto::jump(fnd.ret.first, fnd.num, l - fnd.l + 1);
            write(ret.second);
        }
    }
}

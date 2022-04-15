#include <bits/stdc++.h>

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

const int N = 1e5 + 5;

int n, m;
std::vector<int> G[N];
int deg[N];

void topsort() {
	std::queue<int> q;
	for (int i = 1; i <= n; ++i) {
		if (deg[i] == 0) {
			q.push(i);
		}
	}
	while (q.size()) {
		int x = q.front(); q.pop();
		for (const auto &y : G[x]) {
			--deg[y];
			if (!deg[y]) {
				q.push(y);
			}
		}
	}
}
int col[N];
std::vector<int> vec[N];
std::vector<int> G2[N];
std::map<int, int> cnt[N];
int sumc[N], to[N];
void reCol(int x, int c, std::queue<int> &q) {
	for (const auto &y : G2[x]) {
		--cnt[y][col[x]];
		if (!cnt[y][col[x]]) {
			--sumc[y];
		}
	}
	col[x] = c;
	vec[c].push_back(x);
	for (const auto &y : G2[x]) {
		if (!cnt[y][c]) {
			++sumc[y];
		}
		++cnt[y][c];
		if (sumc[y] == 1) {
			q.push(y);
		}
	}
}
void merge(int x, int y, std::queue<int> &q) {
	x = col[x], y = col[y];
	if (x == y) {
		return;
	}
	if (vec[x].size() > vec[y].size()) {
		std::swap(x, y);
	}
	for (const auto &it : vec[x]) {
		reCol(it, y, q);
	}
	vec[x].clear(), vec[x].shrink_to_fit();
}
void solv() {
	for (int i = 1; i <= n; ++i) {
		if (deg[i]) {
			col[i] = i;
			vec[i].push_back(i);
			for (const auto &y : G[i]) {
				if (deg[y]) {
					G2[i].push_back(y);
					to[y] = i;
					++cnt[y][i], ++sumc[y];
				}
			}
		}
	}
	std::queue<int> q;
	for (int i = 1; i <= n; ++i) {
		if (sumc[i] == 1) {
			q.push(i);
		}
	}
	while (q.size()) {
		int x = q.front(); q.pop();
		merge(x, to[x], q);
	}
}

signed main() {
	read(n), read(m);
	for (int i = 1; i <= m; ++i) {
		int u, v;
		read(v), read(u);
		G[u].push_back(v);
		++deg[v];
	}
	topsort();
	solv();
	int q = read<int>();
	while (q--) {
		int x, y;
		read(x), read(y);
		if (!col[x] || !col[y] || col[x] == col[y]) {
			putchar('B');
		} else {
			putchar('H');
		}
	}
}
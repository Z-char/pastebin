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

const int $N = 120;

int R, D, C, offset;
std::vector<int> in[$N][$N], out[$N][$N];
inline void add(int x, int y, int c) {
	out[x][c].push_back(y);
	in[y][c].push_back(x);
}
bool f[$N][$N][$N];

void solv(int x, int y, int c) {
	if (f[x][y][c]) {
		return;
	}
	f[x][y][c] = true;
	for (int i = 0; i < R + offset; ++i) {
		if (f[i][x][c]) {
			solv(i, y, c);
		}
		if (f[y][i][c]) {
			solv(x, i, c);
		}
	} // merge bracket
	for (int &u : in[x][c]) {
		for (int i = 0; i <= C; ++i) {
			if (u >= offset && !out[u][i].empty()) { // u < offset 不需要贪心匹配
				continue;
			}
			for (int &v : out[y][c]) {
				solv(u, v, i);
			}
		}
	}
}

signed main() {
	read(R, D, C);
	offset = R;
	for (int i = 0; i < offset; ++i) {
		for (int j = 0; j < C; ++j) {
			add(i, i + 1, j);
		}
	}
	for (int j = 0; j < C; ++j) {
		add(offset + R - 1, offset + R - 1, j);
	}
	while (D--) {
		int x, y, c;
		read(x, y, c);
		add(x + offset, y + offset, c);
	}
	for (int i = 0; i < R + offset; ++i) {
		for (int j = 0; j < C; ++j) {
			solv(i, i, j);
		}
	}
	for (int i = 0; i > -R; --i) {
		if (f[i + offset][R - 1 + offset][C]) {
			write(-i);
			break;
		}
	}
}
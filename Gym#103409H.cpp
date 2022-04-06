//
#include <bits/stdc++.h>
 
using i64 = int64_t;
 
constexpr int Sig = 2;
constexpr int SZ = 500'001;
 
int ch[SZ][2];
int fail[SZ];
 
int f[30][SZ][2];
i64 tag[30][SZ][2];
 
int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
 
	int n, m;
	std::cin >> n >> m;
	std::vector<std::pair<int, int>> a(n);
	for (int i = 0; i < n; ++i) std::cin >> a[i].first >> a[i].second;
	int tot = 1;
	std::vector<int> pos(m);
	for (int i = 0; i < m; ++i) {
		static std::string s;
		std::cin >> s;
		int p = 0;
		for (char _c : s) {
			int c = _c - '0';
			if (!ch[p][c]) ch[p][c] = tot++;
			p = ch[p][c];
		}
		pos[i] = p;
	}
	std::queue<int> q;
	for (int i = 0; i < Sig; ++i) {
		if (ch[0][i]) {
			q.push(ch[0][i]);
		}
	}
	while (!q.empty()) {
		int u = q.front();
		q.pop();
		for (int i = 0; i < 2; ++i) {
			if (ch[u][i]) {
				q.push(ch[u][i]);
				fail[ch[u][i]] = ch[fail[u]][i];
			} else {
				ch[u][i] = ch[fail[u]][i];
			}
		}
	}
 
	for (int i = 0; i < tot; ++i) {
		for (int d = 0; d < Sig; ++d) {
			f[0][i][d] = ch[i][d];
		}
	}
	for (int j = 0; j < 29; ++j) {
		for (int i = 0; i < tot; ++i) {
			for (int d = 0; d < Sig; ++d) {
				f[j + 1][i][d] = f[j][f[j][i][d]][d ^ 1];
			}
		}
	}
 
 
	int cur = 0;
	auto solve = [&](auto &&self, int l, int r, int d, int L, int R) -> void {
		if (L <= l && r <= R) {
			int t = __builtin_ctz(r - l);
			tag[t][cur][d] += 1;
			cur = f[t][cur][d];
			return;
		}
		int mid = (l + r) / 2;
		if (L < mid) self(self, l, mid, d, L, R);
		if (R > mid) self(self, mid, r, d ^ 1, L, R);
	};
 
	for (auto i : a) {
		solve(solve, 0, 1 << 30, 0, i.first, i.second + 1);
	}
 
	for (int j = 29; j > 0; --j) {
		for (int i = 0; i < tot; ++i) {
			for (int d = 0; d < Sig; ++d) {
				tag[j - 1][i][d] += tag[j][i][d];
				tag[j - 1][f[j - 1][i][d]][d ^ 1] += tag[j][i][d];
			}
		}
	}
	std::vector<i64> ans(tot);
	for (int i = 0; i < tot; ++i) {
		for (int d = 0; d < Sig; ++d) {
			ans[ch[i][d]] += tag[0][i][d];
		}
	}
 
	std::vector<int> deg(tot);
	for (int i = 1; i < tot; ++i) {
		++deg[fail[i]];
	}
	for (int i = 0; i < tot; ++i) {
		if (!deg[i]) q.push(i);
	}
	while (!q.empty()) {
		int u = q.front();
		q.pop();
		int v = fail[u];
		ans[v] += ans[u];
		--deg[v];
		if (!deg[v]) q.push(v);
	}
 
	for (int i = 0; i < m; ++i) {
		std::cout << ans[pos[i]] << '\n';
	}
 
	return 0;
}

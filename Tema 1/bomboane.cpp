#include <bits/stdc++.h>

using namespace std;

#define MOD 1000000007
#define Mmax 2001
#define Nmax 51

struct Interval {
	int x;
	int y;

	Interval() {
		x = y = 0;
	}

	Interval(int x, int y) {
		this->x = x;
		this->y = y;
	}
};

class Task {
 public:
	void solve() {
		read_input();
		print_output(get_result());
	}

 private:
	int dp[Mmax][Nmax];
	vector<Interval> v;
	int n, m;

	static bool comparator(Interval a, Interval b) {
		return a.x > b.x;
	}

	void read_input() {
		int start, end;
		ifstream f("bomboane.in");
		f >> n >> m;
		v.push_back(Interval());
		for (int i = 1; i <= n; i++) {
			f >> start >> end;
			v.push_back(Interval(start, end));
		}
		f.close();
	}

	void base_case() {
		for (int i = 1; i <= m; i++) {
			if (i >= v[1].x && i <= v[1].y && i != 0) {
				dp[i][1] = 1;
			} else {
				dp[i][1] = 0;
			}
		}
	}

	int get_result() {
		int zero_sweets = 0;
		sort(v.begin() + 1, v.end(), comparator);
		base_case();

		if (v[1].x == 0) {
			zero_sweets++;
		}

		for (int j = 2; j <= n; j++) {
			for (int i = 1; i <= m; i++) {
				for (int k = v[j].x; k <= v[j].y; k++) {
					int rest = i - k;
					if (rest > 0) {
						dp[i][j] = (dp[i][j] + dp[rest][j - 1]) % MOD;
					} else if (rest == 0) {
						if (zero_sweets == j - 1) {
							dp[i][j] = (dp[i][j] + 1) % MOD;
						}
						break;
					}
				}
			}
			if (v[j].x == 0) {
				zero_sweets++;
			}
		}
		return dp[m][n];
	}

	void print_output(int result) {
		ofstream g("bomboane.out");
		g << result << endl;
		g.close();
	}
};

int main() {
	Task *task = new Task();
	task->solve();
	delete task;
	return 0;
}

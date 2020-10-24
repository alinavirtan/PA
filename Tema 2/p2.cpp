#include<bits/stdc++.h>
using namespace std;

const int kNmax = 100005;
#define INF 999999999

class Task {
 public:
	void solve() {
		read_input();
		print_output(get_result());
	}

 private:
	int n, m;
	int source, dest;
	vector<pair<int, int>> adj[kNmax + 1];

	void read_input() {
		ifstream fin("p2.in");
		int x, y, e;
		fin >> n >> m;
		fin >> source >> dest;
		for (int i = 1; i <= m; i++) {
			fin >> x >> y >> e;
			adj[x].push_back({y, e});
		}
		fin.close();
	}

	vector<int> topsort() {
		vector<int> topsort;
		stack<int> s;
		vector<int> in(n + 1);

		for (int u = 1; u <= n; u++) {
			for (unsigned v = 0; v < adj[u].size(); v++) {
				int vecin = adj[u][v].first;
				in[vecin]++;
			}
		}

		for (int u = 1; u <= n; u++) {
			if (in[u] == 0) {
				s.push(u);
			}
		}

		while (!s.empty()) {
			int u = s.top();
			s.pop();
			topsort.push_back(u);

			for (unsigned v = 0; v < adj[u].size(); v++) {
				int vecin = adj[u][v].first;
				in[vecin]--;
				if (in[vecin] == 0) {
					s.push(vecin);
				}
			}
		}

		return topsort;
	}

	long long get_result() {
		vector<int> sorted = topsort();

		bool source_found = false;
		vector<long long> dist(n + 1, INF);
		vector<long long> finished(n + 1, false);
		dist[source] = 0;

		for (unsigned i = 0; i < sorted.size(); i++) {
			int u = sorted[i];

			if (u == source) {
				source_found = true;
			}

			if (source_found == false) {
				continue;
			}

			if (u == dest) {
				break;
			}

			if (source_found == true) {
				for (unsigned v = 0; v < adj[u].size(); v++) {
					int vecin = adj[u][v].first;
					int weight = adj[u][v].second;

					if (finished[vecin] == false && dist[u] + weight < dist[vecin]) {
						dist[vecin] = dist[u] + weight;
					}
				}

				finished[u] = true;
			}
		}


		cout << dist[dest] << endl;
		return dist[dest];
	}

	void print_output(long long result) {
		ofstream fout("p2.out");
		fout << result << '\n';
		fout.close();
	}
};

int main() {
	Task *task = new Task();
	task->solve();
	delete task;
	return 0;
}

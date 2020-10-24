#include <bits/stdc++.h>
using namespace std;

const int kNmax = 1000000;
const int kInf = 0x3f3f3f3f;
ifstream fin("p3.in");
ofstream fout("p3.out");

struct Pair {
	int town;
	int energy;

	Pair() {
		this->town = this->energy = 0;
	}

	Pair(int town, int energy) {
		this->town = town;
		this->energy = energy;
	}

	bool operator<(const Pair& x) const {
		return this->energy < x.energy;
	}
};

class Task {
 public:
    void solve() {
        read_input();
        print_output(dijkstra());
    }

 private:
    int n, m;
    int initial_energy;
    vector<Pair> adj[kNmax + 1];
    vector<double> d;

    void read_input() {
    	int x, y, energy;
        fin >> n >> m >> initial_energy;
        for (int i = 1; i <= m; i++) {
            fin >> x >> y >> energy;
            adj[x].push_back(Pair(y, energy));
        }
    }

    vector<int> dijkstra() {
        int v, source = 1;
        priority_queue<Pair> pq;
        vector<bool> viz(n + 1);
        vector<int> p(n + 1, 0);

        for (int i = 0; i <= n; i++) {
            p[i] = 0;
            d.push_back(0);
            viz[i] = false;
        }

        d[source] = initial_energy;
        pq.push(Pair(source, d[source]));
        double weight = 0;
        while (!pq.empty()) {
            int u = pq.top().town;
            pq.pop();
            if (viz[u] == false) {
                viz[u] = true;
                for (auto next : adj[u]) {
                    v = next.town;
                    weight = next.energy;
                    if (d[v] < d[u] * (1 - weight / 100)) {
                        d[v] = d[u] * (1 - weight / 100);
                        p[v] = u;
                        pq.push(Pair(v, d[v]));
                    }
                }
            }
        }
        return p;
    }

    //  formeaza drumul de la sursa la destinatie
    void computePath(vector<int> parent, int j, queue<int> &q) {
        if (parent[j] == 0) {
        	q.push(1);
            return;
        }

        computePath(parent, parent[j], q);

        // adauga nodul in coada
        q.push(j);
    }

    void print_output(vector<int> p) {
        queue<int> q, tmp;

        // formeaza drumul de la sursa la destinatie
        computePath(p, n, q);

        // afiseaza energia ramasa
        fout << fixed << setprecision(4) << d[n] << "\n";

    	//  afiseaza drumul de la sursa la destinatie
    	while (!q.empty()) {
    		fout << q.front() << " ";
    		q.pop();
    	}
        fout << "\n";
    }
};

int main() {
    Task *task = new Task();
    task->solve();
    delete task;
    fin.close();
    fout.close();
    return 0;
}

#include <bits/stdc++.h>
using namespace std;

const int kNmax = 1000000;
const int kInf = 0x3f3f3f3f;
ifstream fin("p1.in");
ofstream fout("p1.out");

class Task {
 public:
    void solve() {
        read_input();
        print_output(get_result());
    }

 private:
    int n, m, k;
    vector<int> lords;
    vector<int> perm;
    vector<int> adj[kNmax + 1];

    void read_input() {
    	int x, y;
        fin >> n >> m >> k;
        for (int i = 1; i <= k; i++) {
            fin >> x;
            lords.push_back(x);
        }

        for (int i = 2; i <= n; i++) {
            fin >> x;
            perm.push_back(x);
        }

        for (int i = 1; i <= m; i++) {
            fin >> x >> y;
            adj[x].push_back(y);
            adj[y].push_back(x);
        }
    }

    void dfsUtil(int start, int visited[]) {
        visited[start] = 1;

        for (unsigned i = 0; i < adj[start].size(); i++) {
            int vecin = adj[start][i];
            if (visited[vecin] == 0) {
                dfsUtil(vecin, visited);
            }
        }
    }

    bool dfs(int mid) {
        int visited[kNmax + 1];
        bool ok;

        // la inceput, toate orasele sunt nevizitate
        for (int i = 1; i <= n; i++) {
            visited[i] = 0;
        }

        //  marchez orasele blocate
        for (int i = 0; i <= mid; i++) {
            visited[perm[i]] = 2;
        }

        dfsUtil(1, visited);

        /* pp ca plecand din orasul lui Robin Hood, nu ajung in niciunul
           din orasele din care pleaca lorzii */
        ok = true;
        for (unsigned i = 0; i < lords.size() && ok == true; i++) {
            int lord = lords[i];
            if (visited[lord] == 1) {
                ok = false;
            }
        }

        return ok;
    }

    int get_result() {
        int mid, left = 0, right = perm.size() - 1;
        int Min = kInf;
        while (left <= right) {
            mid = (left + right) / 2;
            if (dfs(mid) == false) {
                left = mid + 1;
            } else {
                Min = mid + 1;
                right = mid - 1;
            }
        }

        return Min;
    }

    void print_output(int result) {
        cout << result << endl;
        fout << result << "\n";
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

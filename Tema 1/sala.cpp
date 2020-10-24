#include <bits/stdc++.h>

using namespace std;

struct Dumbbell {
	long long weight;
	long long reps;

	Dumbbell() {
		weight = reps = 0;
	}

	Dumbbell(long long weight, long long reps) {
		this->weight = weight;
		this->reps = reps;
	}
};

struct DumbbellComp {
	bool operator()(const Dumbbell &a, const Dumbbell &b) {
		if (a.weight == b.weight) {
			return a.reps > b.reps;
		} else {
			return a.weight > b.weight;
		}
	}
};

class Task {
 public:
	void solve() {
		read_input();
		print_output(bonus());
	}

 private:
	int n, m;
	vector<Dumbbell> v;

	void read_input() {
		ifstream f("sala.in");
		int weight, reps;
		f >> n >> m;
		for (int i = 0; i < n; i++) {
			f >> weight >> reps;
			v.push_back(Dumbbell(weight, reps));
		}
		f.close();
	}

	// sortez descrescator dupa numarul de repetari al fiecarei gantere
	static bool comparator(const Dumbbell &a, const Dumbbell &b) {
		if (a.reps == b.reps) {
			return a.weight > b.weight;
		} else {
			return a.reps > b.reps;
		}
	}

	long long bonus() {
		long long muscles = 0;
		long long no_reps = 0, no_min_weight = 0;
		priority_queue<Dumbbell, vector<Dumbbell>, DumbbellComp> pq;
		sort(v.begin(), v.end(), comparator);

		/* introduc in coada primele m gantere cu cele mai multe repetari
		 * si actualizez numarul cu care isi creste Gigel muschii */
		no_min_weight = v[0].weight;
		for (int i = 0; i < m; i++) {
			pq.push(v[i]);
			no_min_weight = min(no_min_weight, v[i].weight);
			no_reps = no_reps + v[i].reps;
			if (1LL * no_min_weight * no_reps > muscles) {
				muscles = 1LL * no_min_weight * no_reps;
			}
		}

		/* daca gasesc o gantera cu G > gantera cu greutarea minima din coada
		 * inlocuiesc acea gantera din coada cu gantera gasita */
		for (int i = m; i < n; i++) {
			Dumbbell db = pq.top();
			if (v[i].weight > db.weight) {
				pq.pop();
				pq.push(v[i]);
				Dumbbell new_db = pq.top();
				no_min_weight = new_db.weight;
				no_reps = no_reps - db.reps + v[i].reps;

				if (1LL * no_min_weight * no_reps > muscles) {
					muscles = 1LL * no_min_weight * no_reps;
				}
			}
		}

		/* nr de gantere trebuia sa fie <= m, deci scot pe rand gantere din
		 * coada si verific daca pot maximiza dimensiunea muschilor lui Gigel*/
		while (!pq.empty()) {
			Dumbbell db = pq.top();
			pq.pop();
			if (!pq.empty()) {
				Dumbbell new_db = pq.top();
				no_reps = no_reps - db.reps;
				no_min_weight = new_db.weight;
				if (1LL * no_min_weight * no_reps > muscles) {
					muscles = 1LL * no_min_weight * no_reps;
				}
			}
		}

		return muscles;
	}

	void print_output(long long result) {
		ofstream g("sala.out");
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

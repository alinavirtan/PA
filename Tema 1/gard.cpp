#include <bits/stdc++.h>

using namespace std;

struct dimension {
	int start, end, redundant;

	dimension() {
		this->start = 0;
		this->end = 0;
		this->redundant = 0;
	}

	dimension(int start, int end) {
		this->start = start;
		this->end = end;
		this->redundant = 0;
	}
};

class Task {
 public:
	void solve() {
		read_input();
		print_output(get_result());
	}

 private:
	int n;
	vector<dimension> fence;

	void read_input() {
		int start, end;
		ifstream fin("gard.in");
		fin >> n;
		for (int i = 0; i < n; i++) {
			fin >> start >> end;
			fence.push_back(dimension(start, end));
		}
		fin.close();
	}

	static bool comp(dimension a, dimension b) {
		if (a.end == b.end) {
			return a.start > b.start;
		} else {
			return a.end > b.end;
		}
	}

	int get_result() {
		int min_start, redundant = 0;

		/* sortez descrescator dupa capatul superior si, in caz de egalitate, 
		 * descrescator dupa capatul inferior al fiecarei bucati de gard */ 
		sort(fence.begin(), fence.end(), comp);

		/* pentru fiecare bucata de gard, verific daca se incadreaza intr-o
		 * alta bucata de gard */
		min_start = fence[0].start;
		for (int i = 1; i < n; i++) {
			if (fence[i].end <= fence[i - 1].end) {
				if (fence[i].start >= min_start) {
					fence[i].redundant = 1;
					redundant++;
				}
			}
			if (fence[i].end == fence[i - 1].end) {
				if (fence[i].start < min_start
						&& fence[i].start < fence[i - 1].start
						&& fence[i - 1].redundant == 0) {
					fence[i - 1].redundant = 1;
					redundant++;
				}
			}
			// actualizez minimul daca e cazul
			min_start = min(min_start, fence[i].start);
		}
		return redundant;
	}

	void print_output(int result) {
		ofstream fout("gard.out");
		fout << result;
		fout.close();
	}
};

int main() {
	Task *task = new Task();
	task->solve();
	delete task;
	return 0;
}

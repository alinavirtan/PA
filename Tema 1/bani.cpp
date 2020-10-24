#include <bits/stdc++.h>

using namespace std;

#define MOD 1000000007
#define NO_MONEY 5
#define MAX_INT_POW 30
class Task {
 public:
	void solve() {
		read_input();
		print_output();
	}

 private:
	int n, type;

	void read_input() {
		ifstream fin("bani.in");
		fin >> type >> n;
		fin.close();
	}

	// calculez formula 5 * 2^(n-1)
	long long fastPow(int n) {
		long long quotient = n / MAX_INT_POW, rest = n % MAX_INT_POW;
		long long ans = NO_MONEY;
		long long c = 1 << MAX_INT_POW;
		long long r = 1 << rest;

		if (quotient == 0)  {
			return (ans * (r % MOD)) % MOD;
		} else {
			for (int i = 0; i < quotient; i++) {
				ans = (ans * (c % MOD)) % MOD;
			}
			if (rest != 0) {
				ans = (ans * (r % MOD)) % MOD;
			}
		}

		return ans;
	}

	long long solve_type1() {
		if (n == 1) {
			return NO_MONEY;
		}
		return fastPow(n - 1);
	}

	long long solve_type2() {
		/* caz de baza: cand am de depus o bancnota in seif, consider ca 
		 * fiecare bancnota de 10, 50, 100, 200 si 500 depusa individual
		 * reprezinta un mod distinct de a aranja bancnotele
		 * dp[i]=nr de aranjamente ale bancnotele ce se termina in bancnota i,
		 * unde bancnota 0 = 10, bancnota 1 = 50, ..., bancnota 4 = 500
		 */
		vector<long long> dp;
		long long new_dp[NO_MONEY + 1];

		for (int i = 0; i < NO_MONEY; i++) {
			dp.push_back(1);
		}

		/* pentru fiecare bancnota, observ dupa cine e pozitionata:
		 * daca bancnota a urmeaza o bancnotele b, c si d, atunci 
		 * numarul de moduri de a aranja cele i bancnote care se termina
		 * in a este egal cu suma dintre numarul de moduri de a aranja i - 1
		 * bancnote care se termina in b, c si d
		 */
		for (int i = 2; i <= n; i++) {
			new_dp[0] = (dp[1] % MOD + dp[2] % MOD + dp[4] % MOD) % MOD;
			new_dp[1] = (dp[0] % MOD + dp[3] % MOD) % MOD;
			new_dp[2] = (dp[0] % MOD + dp[2] % MOD + dp[3] % MOD) % MOD;
			new_dp[3] = (dp[1] % MOD + dp[4] % MOD) % MOD;
			new_dp[4] = dp[3] % MOD;

			for (int j = 0; j < NO_MONEY; j++) {
				dp[j] = new_dp[j];
			}
		}

		/* dupa ce am adaugat n bancnote, solutia va fi suma modulurilor 
		 * de a aranja bancnotele care se termina in 10,50,100,200 si 500
		 */
		long long sum = 0;
		for (int i = 0; i < NO_MONEY; i++) {
			sum = (sum + dp[i]) % MOD;
		}
		return sum;
	}

	void print_output() {
		ofstream fout("bani.out");
		if (type == 1) {
			fout << solve_type1();
		} else {
			fout << solve_type2();
		}
		fout.close();
	}
};

int main() {
	Task *task = new Task();
	task->solve();
	delete task;
	return 0;
}


#include <bits/stdc++.h>
using namespace std;

struct Robin {
    int x, y, no_wood;

    Robin(int x, int y, int no_wood) {
        this->x = x;
        this->y = y;
        this->no_wood = no_wood;
    }

    Robin() {
        this->x = this->y = this->no_wood = 0;
    }
};

struct MaidMarian {
    int x, y;

    MaidMarian(int x, int y) {
        this->x = x;
        this->y = y;
    }

    MaidMarian() {
        this->x = this->y = 0;
    }
};

struct Wood {
    int startx, starty, endx, endy;

    Wood(int startx, int starty, int endx, int endy) {
        this->startx = startx;
        this->starty = starty;
        this->endx = endx;
        this->endy = endy;
    }

    Wood() {
        this->startx = this->starty = this->endx = this->endy = 0;
    }
};

struct State {
    int time;
    int energy;
    Robin robin;
    string prev_move;

    State(int time, int energy, Robin robin, string move) {
        this->time = time;
        this->energy = energy;
        this->robin = robin;
        this->prev_move = move;
    }

    State() {
        this->time = 0;
        this->energy = 0;
        this->robin = Robin();
        this->prev_move = "";
    }

    bool operator==(const State& s) const {
        return (time == s.time &&
                robin.x == s.robin.x && robin.y == s.robin.y &&
                robin.no_wood == s.robin.no_wood);
    }
};

namespace std {
template <>
struct hash<State> {
    std::size_t operator()(const State& s) const {
        using std::size_t;
        using std::hash;
        using std::string;
        return (hash<int>()(s.time)) ^
                (hash<int>()(s.robin.x)) ^
                (hash<int>()(s.robin.y)) ^
                (hash<int>()(s.robin.no_wood));
    }
};
}  //  namespace std

struct Move {
    char action;
    int next_wood;

    Move() {
        action = 'H';
        next_wood = 0;
    }

    Move(char action, int next_wood) {
        this->action = action;
        this->next_wood = next_wood;
    }
};


class Task {
 public:
    void solve() {
        read_input();
        print_output(get_result());
    }

 private:
    //  input
    int t, n;   // t = timpul, n = nr de busteni
    MaidMarian m;  //  coordonatele lu Maid Marian
    int e1, e2, e3;  // energiile corespunzatoare celor 3 tipuri de actiuni
    vector<Wood> woods;  //  coordonatele tuturor bustenilor
    vector<string> directions;  //  directiile in care se misca bustenii la mij

    //  output
    int minEnergy = 999999999;
    vector<Move> moves;

    //  auxiliare
    queue<State> q;
    unordered_map<State, State> parent;
    unordered_map<State, int> viz;
    State best_state;

    void read_input() {
        ifstream fin("bonus.in");
        int startx, starty, endx, endy;
        char direction;

        fin >> t >> n;
        fin >> m.x >> m.y;
        fin >> e1 >> e2 >> e3;

        for (int i = 0; i < n; i++) {
            fin >> startx >> starty >> endx >> endy;
            woods.push_back(Wood(startx, starty, endx, endy));
        }

        for (int i = 0; i < n; i++) {
            string s = "";
            for (int j = 0; j < t; j++) {
                fin >> direction;
                s = s + direction;
            }

            directions.push_back(s);
        }
        fin.close();
    }

    bool isVertical(Wood w) {
        return w.startx == w.endx;
    }

    bool isHorizontal(Wood w) {
        return w.starty == w.endy;
    }

    bool canMoveNorth(Robin r, Wood w) {
        return r.y < w.endy;
    }

    bool canMoveSouth(Robin r, Wood w) {
        return r.y > w.starty;
    }

    bool canMoveEast(Robin r, Wood w) {
        return r.x < w.endx;
    }

    bool canMoveWest(Robin r, Wood w) {
        return r.x > w.startx;
    }

    void modifyWoodsPosition(int time, State &s) {
        /* direction[i][time] -> directia la care se misca busteanul i
         * la momentul de timp time */
        for (int i = 0; i < n; i++) {
            char dir = directions[i][time];
            if (dir == 'N') {
                woods[i] = Wood(woods[i].startx, woods[i].starty + 1,
                                woods[i].endx, woods[i].endy + 1);
            }

            if (dir == 'S') {
                woods[i] = Wood(woods[i].startx, woods[i].starty - 1,
                             woods[i].endx, woods[i].endy - 1);
            }

            if (dir == 'V') {
                woods[i] = Wood(woods[i].startx - 1, woods[i].starty,
                                     woods[i].endx - 1, woods[i].endy);
            }

            if (dir == 'E') {
                woods[i] = Wood(woods[i].startx + 1, woods[i].starty,
                             woods[i].endx + 1, woods[i].endy);
            }
        }
    }
    void modifyRobinPosition(int time, State &s) {
        /* direction[i][time] -> directia la care se misca busteanul i
         * la momentul de timp time */
            char dir = directions[s.robin.no_wood][time];
            if (dir == 'N') {
                    s = State(s.time, s.energy, Robin(s.robin.x,
                                s.robin.y + 1, s.robin.no_wood), s.prev_move);
            }

            if (dir == 'S') {
                    s = State(s.time, s.energy, Robin(s.robin.x,
                            s.robin.y - 1, s.robin.no_wood), s.prev_move);
            }

            if (dir == 'V') {
                    s = State(s.time, s.energy, Robin(s.robin.x - 1,
                            s.robin.y, s.robin.no_wood), s.prev_move);
            }

            if (dir == 'E') {
                    s = State(s.time, s.energy, Robin(s.robin.x + 1,
                            s.robin.y, s.robin.no_wood), s.prev_move);
            }
        }

    int get_result() {
        string move = "";
        State initialstate = State(0, 0, Robin(woods[0].startx,
                                    woods[0].starty, 0), move);
        int current_time = 0;
        q.push(initialstate);
        viz[initialstate] = 0;
        while (!q.empty()) {
            State s = q.front();
            q.pop();
            if (viz[s] != s.energy) {
                continue;
            }
            /* daca am trecut la momentul urmator de timp, 
             * modific pozitia bustenilor */
            if (s.time > current_time) {
                modifyWoodsPosition(current_time, s);
                current_time = s.time;
            }

            if (s.robin.x == m.x && s.robin.y == m.y) {
                if (minEnergy > s.energy) {
                    minEnergy = s.energy;
                    best_state = s;
                }
            }

            if (s.time >= t && s.energy > minEnergy) {
                continue;
            }

            // busteanul pe care se afla Robin la momentul curent
            Wood current_wood = woods[s.robin.no_wood];
            State newstate;

            // miscari de tipul 1 (consuma e1) ce se pot face din starea s
            string move = "H";
            newstate = State(s.time + 1, s.energy + e1, s.robin, move);
            modifyRobinPosition(current_time, newstate);
            if (viz.find(newstate) == viz.end()) {
                q.push(newstate);
                viz[newstate] = s.energy + e1;
                parent[newstate] = s;
            } else if (viz[newstate] > s.energy + e1) {
                q.push(newstate);
                viz[newstate] = s.energy + e1;
                parent[newstate] = s;
            }

            //  miscari de tipul 2 (consuma e2) ce se pot face din starea s
            if (isVertical(current_wood)) {
                if (canMoveNorth(s.robin, current_wood)) {
                    string move = "N";
                    newstate = State(s.time + 1, s.energy + e2,
                        Robin(s.robin.x, s.robin.y + 1, s.robin.no_wood), move);
                    modifyRobinPosition(current_time, newstate);
                    if (viz.find(newstate) == viz.end()) {
                        viz[newstate] = s.energy + e2;
                        parent[newstate] = s;
                        q.push(newstate);
                    } else if (viz[newstate] > s.energy + e2) {
                        viz[newstate] = s.energy + e2;
                        parent[newstate] = s;
                        q.push(newstate);
                    }
                }

                if (canMoveSouth(s.robin, current_wood)) {
                    string move = "S";
                    newstate = State(s.time + 1, s.energy + e2,
                        Robin(s.robin.x, s.robin.y - 1, s.robin.no_wood), move);
                    modifyRobinPosition(current_time, newstate);
                    if (viz.find(newstate) == viz.end()) {
                        viz[newstate] = s.energy + e2;
                        parent[newstate] = s;
                        q.push(newstate);
                    } else if (viz[newstate] > s.energy + e2) {
                        viz[newstate] = s.energy + e2;
                        parent[newstate] = s;
                        q.push(newstate);
                    }
                }
            }

            if (isHorizontal(current_wood)) {
                if (canMoveEast(s.robin, current_wood)) {
                    string move = "E";
                    newstate = State(s.time + 1, s.energy + e2,
                        Robin(s.robin.x + 1, s.robin.y, s.robin.no_wood), move);
                    modifyRobinPosition(current_time, newstate);
                    if (viz.find(newstate) == viz.end()) {
                        viz[newstate] = s.energy + e2;
                        parent[newstate] = s;
                        q.push(newstate);
                    } else if (viz[newstate] > s.energy + e2) {
                        viz[newstate] = s.energy + e2;
                        parent[newstate] = s;
                        q.push(newstate);
                    }
                }

                if (canMoveWest(s.robin, current_wood)) {
                    string move = "V";
                    newstate = State(s.time + 1, s.energy + e2,
                        Robin(s.robin.x - 1, s.robin.y, s.robin.no_wood), move);
                    modifyRobinPosition(current_time, newstate);
                    if (viz.find(newstate) == viz.end()) {
                        viz[newstate] = s.energy + e2;
                        parent[newstate] = s;
                        q.push(newstate);
                    } else if (viz[newstate] > s.energy + e2) {
                        viz[newstate] = s.energy + e2;
                        parent[newstate] = s;
                        q.push(newstate);
                    }
                }
            }

            //  miscari de tipul 3 (consuma e3) ce se pot face din starea s
            for (int i = 0; i < n; i++) {
                if (i != s.robin.no_wood) {
                    if (woods[i].startx <= s.robin.x &&
                                s.robin.x <= woods[i].endx &&
                                woods[i].starty <= s.robin.y &&
                                s.robin.y <= woods[i].endy) {
                        string move = "J " + to_string(i + 1);
                        newstate = State(s.time + 1, s.energy + e3,
                                    Robin(s.robin.x, s.robin.y, i), move);
                        modifyRobinPosition(current_time, newstate);
                        if (viz.find(newstate) == viz.end()) {
                            viz[newstate] = s.energy + e3;
                            parent[newstate] = s;
                            q.push(newstate);
                        } else if (viz[newstate] > s.energy + e3) {
                            viz[newstate] = s.energy + e3;
                            parent[newstate] = s;
                            q.push(newstate);
                        }
                    }
                }
            }
        }

        return minEnergy;
    }

    void print_output(int minEnergy) {
        ofstream fout("bonus.out");
        fout << minEnergy << endl;
        fout << best_state.time << endl;
        vector<string> ans;
        while (best_state.prev_move != "") {
            ans.push_back(best_state.prev_move);
            best_state = parent[best_state];
        }
        for (int i = ans.size() - 1; i >= 0; i--) {
            fout << ans[i] << endl;
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

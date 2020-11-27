/* vim: set sw=4 sts=4 et foldmethod=syntax : */

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <optional>
#include <set>
#include <sstream>
#include <tuple>
#include <utility>
#include <vector>

using std::cerr;
using std::cout;
using std::endl;
using std::get;
using std::ifstream;
using std::make_optional;
using std::move;
using std::nullopt;
using std::optional;
using std::pair;
using std::set;
using std::string;
using std::stringstream;
using std::tie;
using std::tuple;
using std::vector;

struct Problem
{
    int vertices;
    int budget;
    set<pair<int, int> > adj;
};

enum VertexState
{
    Untouched,
    Burninated,
    Defended
};

using State = vector<VertexState>;

auto read_input(const string & filename) -> optional<pair<Problem, State> >
{
    pair<Problem, State> result;
    ifstream in{ filename };
    bool seen_v = false, seen_f = false, seen_b = false;

    string line;
    while (getline(in, line)) {
        auto p = line.find('=');
        if (p != string::npos) {
            string k, _;
            stringstream line_s{ line };
            line_s >> k >> _;
            if (k == "vertices") {
                int v;
                line_s >> v;
                result.first.vertices = v;
                result.second.resize(v);
                seen_v = true;
            }
            else if (k == "fires") {
                int f;
                while (line_s >> f)
                    result.second[f - 1] = Burninated;
                seen_f = true;
            }
            else if (k == "budget") {
                int b;
                line_s >> b;
                result.first.budget = b;
                seen_b = true;
            }
            else
                return nullopt;
        }
        else {
            p = line.find(',');
            if (p == string::npos)
                return nullopt;

            int a = stoi(line.substr(0, p)), b = stoi(line.substr(p + 1));
            if (a <= 0 || a > result.first.vertices || b <= 0 || b > result.first.vertices)
                return nullopt;

            result.first.adj.emplace(a - 1, b - 1);
            result.first.adj.emplace(b - 1, a - 1);
        }
    }

    if (! (seen_v && seen_f && seen_b))
        return nullopt;

    return make_optional(result);
};

auto progress_burnination(const Problem & problem, const State & state) -> tuple<State, bool, int>
{
    tuple<State, bool, int> result{ state, true, 0 };

    for (const auto & [ f, t ] : problem.adj) {
        if (state[f] == Burninated && state[t] == Untouched) {
            get<0>(result)[t] = Burninated;
            get<1>(result) = false;
        }
    }

    for (auto & s : state)
        if (s == Burninated)
            ++get<2>(result);

    return result;
}

auto solve(int turn, const Problem & problem, const State & state, const vector<vector<int> > & choices,
        int & best_score,
        vector<vector<int> > & best_choices) -> void;

auto show(const string & why, int score, const vector<vector<int> > & choices)
{
    cout << why << ": " << score;
    for (auto & c : choices) {
        cout << " [";
        for (auto & d : c)
            cout << " " << d + 1;
        cout << " ]";
    }
    cout << endl;
}

auto solution(
        const State & state,
        const vector<vector<int> > & choices,
        int & best_score,
        vector<vector<int> > & best_choices) -> void
{
    int burninated = 0;
    for (auto & s : state)
        if (s == Burninated)
            ++burninated;

    if (burninated < best_score) {
        show("new incumbent", burninated, choices);
        best_score = burninated;
        best_choices = choices;
    }
}

auto branch(int turn, const Problem & problem, const State & state, int defenders_to_add,
        int start_defenders_at,
        const vector<vector<int> > & choices,
        int & best_score,
        vector<vector<int> > & best_choices) -> void
{
    if (0 == defenders_to_add) {
        auto [ new_state, done, how_many_burning ] = progress_burnination(problem, state);
        if ((! done) && how_many_burning < best_score)
            solve(turn + 1, problem, new_state, choices, best_score, best_choices);
        else
            solution(new_state, choices, best_score, best_choices);
    }
    else {
        bool did_any = false;
        for (unsigned i = start_defenders_at ; i < state.size() ; ++i) {
            if (state[i] == Untouched) {
                auto new_state = state;
                new_state[i] = Defended;
                auto new_choices = choices;
                new_choices.back().emplace_back(i);
                branch(turn, problem, new_state, defenders_to_add - 1, i + 1, new_choices, best_score, best_choices);
                did_any = true;
            }
        }

        if (! did_any)
            branch(turn, problem, state, 0, 0, choices, best_score, best_choices);
    }
}

auto solve(int turn, const Problem & problem, const State & state, const vector<vector<int> > & choices,
        int & best_score,
        vector<vector<int> > & best_choices) -> void
{
    auto new_choices = choices;
    new_choices.emplace_back();
    branch(turn, problem, state, problem.budget, 0, new_choices, best_score, best_choices);
}

auto main(int argc, char * argv[]) -> int
{
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " infile" << endl;
        return EXIT_FAILURE;
    }

    auto input = read_input(argv[1]);
    if (! input) {
        cerr << "Unspecified unhelpful error reading input" << endl;
        return EXIT_FAILURE;
    }

    auto [ problem, state ] = *input;
    vector<vector<int> > choices, best_choices;
    int best_score = problem.vertices + 1;
    solve(0, problem, state, choices, best_score, best_choices);
    show("solution", best_score, best_choices);

    return EXIT_SUCCESS;
}


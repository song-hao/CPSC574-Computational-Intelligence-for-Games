#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>

#define LIMIT 99

std::vector<std::vector<std::vector<double> > > prob;
std::vector<std::vector<int> > move;
std::vector<std::vector<double> > roll;

void get_all_roll_prob(int goal, int x, int y) {
    for (int target = 2; target <= std::max(2, goal - std::min(x, y)); target++) {
        std::vector<std::vector<double> > prob(goal + 1, std::vector<double>(7, 0.0));
        auto *prob_target = new double[target + 1];
        prob_target[0] = 1.0;
        for (int i = 1; i <= target; i++)
            prob_target[i] = 0.0;
        for (int i = 2; i <= target; i++) {
            for (int j = 2; j <= 6; j++)
                if (i - j >= 0)
                    prob_target[i] += prob_target[i - j] / 6.0;
            prob[i][0] = prob_target[i];
            prob[i][6] = 1.0 - prob[i][0];
            for (int j = 1; j < 6; j++) {
                for (int k = std::min(6, i + j); k >= 2 && k > j; k--)
                    prob[i][j] += prob_target[i + j - k] / 6.0;
                prob[i][6] -= prob[i][j];
            }
        }
        roll[target] = prob[target];
        delete[] prob_target;
    }
}

void win_prob_non_recursive(int goal, int x, int y) {
    for (int n = 1; n < LIMIT; n++) {
        for (int i = goal - 1; i >= x; i--) {
            for (int j = goal - 1; j >= y; j--) {
                double opt_prob = 2.0 * (n % 2 == 0 ? -1 : 1);
                int opt_move = 0;
                if (n % 2 == 0) {
                    for (int s = 2; s <= std::max(2, goal - i); s++) {
                        double one_prob = prob[n - 1][i][j] * roll[s][6];
                        for (int k = s; k < s + 6; k++)
                            one_prob += (i + k >= goal ? 1.0 : prob[n - 1][i + k][j]) * roll[s][k - s];
                        opt_prob = std::max(one_prob, opt_prob);
                        opt_move = one_prob == opt_prob ? s : opt_move;
                    }
                } else {
                    for (int s = 2; s <= std::max(2, goal - j); s++) {
                        double one_prob = prob[n - 1][i][j] * roll[s][6];
                        for (int k = s; k < s + 6; k++)
                            one_prob += (j + k >= goal ? 0.0 : prob[n - 1][i][j + k]) * roll[s][k - s];
                        opt_prob = std::min(one_prob, opt_prob);
                    }
                }
                prob[n][i][j] = opt_prob;
                move[i][j] = opt_move;
            }
        }
    }
}

int main(int argc, char **argv) {
    try {
        if (argc == 4) {
            int x = 0, y = 0, goal = 0;
            std::string str = std::string(argv[1]) + " " + std::string(argv[2]) + " " + std::string(argv[3]);
            std::istringstream iss(str);
            iss >> goal >> x >> y;
            if (!iss || goal < 0 || x < 0 || y < 0) throw -1;
            prob = std::vector<std::vector<std::vector<double> > >(LIMIT, 
                std::vector<std::vector<double> >(goal, std::vector<double>(goal, 0.5)));
            move = std::vector<std::vector<int> >(goal, std::vector<int>(goal, 0));
            roll = std::vector<std::vector<double> >(std::max(2, goal - std::min(x, y)) + 1, std::vector<double>(7, 0.0));
            get_all_roll_prob(goal, x, y);
            win_prob_non_recursive(goal, x, y);
            std::cout << std::setprecision(6) << prob[LIMIT - 1][x][y] << " " << move[x][y] << std::endl;
        }
    } catch (...) {
        return -1;
    }
    return 0;
}


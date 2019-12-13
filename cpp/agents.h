#include "tree.h"
#include "game.h"
#include "utils.h"
class agent{
public:
    Game *game;
    string name;
    agent(Game *game, string name="X1");
    virtual vector<int> play_move();
    virtual void opponent_move(vector<int> pos);
    virtual vector<vector<int > > get_board() = 0;
};

class random_rollout: public agent{
public:
    Tree *tree;
    random_rollout(Game *game, string name, int num_rollouts, double C, int max_depth, double timeout, int num_workers, double gamma, double alpha, double beta, double beta1);
    vector<int> play_move();
    void opponent_move(vector<int> pos);
    vector<vector<int > > get_board();
};

class human_agent: public agent{
public:
    vector<vector<int> > board;
    int turn = 1;
    human_agent(Game *game, string name);
    vector<int> play_move();
    void opponent_move(vector<int> pos);
    vector<vector<int > > get_board();
};

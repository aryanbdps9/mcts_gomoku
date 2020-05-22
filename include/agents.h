#include "tree.h"
// #include "game.h"
#include "utils.h"
#include "argdict.h"

class agent{
public:
    string name;
    int nr, nc, linesize, turn, last_move_at;
    int verbosity = 0;
    agent(string name, argdict gameParams, int turn=1);
    virtual int play_move();
    virtual void opponent_move(int pos);
    virtual vector<vector<int > > get_board() = 0;
    inline virtual void setVerbosity(int v){this->verbosity=v;}
};

class human_agent: public agent{
public:
    vector<vector<int> > board;
    human_agent(string name, argdict gameParams, int turn=1);
    int play_move();
    void opponent_move(int pos);
    vector<vector<int > > get_board();
};

class random_rollout: public agent{
public:
    VanillaTree* tree;
    random_rollout(string name, argdict gameParams, argdict TreeArgDict, int turn=1);
    int play_move();
    void opponent_move(int pos);
    vector<vector<int > > get_board();
    virtual void setVerbosity(int v){
        this->verbosity = v;
        tree->setVerbosity(v);
    }
};
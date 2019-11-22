#include "agents.h"
#include "tree.h"
#include "game.h"

agent::agent(Game *game, string name){
    this->game = game;
	this->name = name;
}
vector<int> agent::play_move(){}
void agent::opponent_move(vector<int> pos){}

random_rollout::random_rollout(Game *game, string name, int num_rollouts, double C, int max_depth, double timeout, int num_workers, double gamma, double alpha, double beta, int mode):agent(game, name){
	tree = new Tree(game, num_rollouts, C, max_depth, timeout, num_workers, gamma, alpha, beta, modeint2enum(mode));
}

vector<int> random_rollout::play_move(){
	vector<int> ret;
	tree->play_one_move(ret);
	return ret;
}

void random_rollout::opponent_move(vector<int> pos){
	tree->player_move(pos);
}

vector<vector<int> > random_rollout::get_board(){
	return tree->root->board;
}

human_agent::human_agent(Game *game, string name, int num_rollouts, double C, int max_depth, double timeout, int num_workers, double gamma, double alpha, double beta, int mode):agent(game, name){
	vector<vector<int> > temp(game->n, vector<int>(game->n, 0));
	board = temp;
}

vector<int>  human_agent::play_move(){
	int N = board.size();
	vector<int> pos(2);
	while (true){
		cout << "Enter your new mark position(pair of space separated integers):\t";
		cin >> pos[0] >> pos[1];
		if (pos[0] < 0 || pos[0] >= N || pos[1] < 0 || pos[1] >= N){
			cerr << "ERROR: Enter a pair of integers within board range\n";
			continue;
		}
		if (board[pos[0]][pos[1]] != 0){
			cerr << "ERROR: Enter a position that is empty\n";
			continue;
		}
		board[pos[0]][pos[1]] = turn;
		turn = ((turn%2)+1);
		return pos;
	}
}

void human_agent::opponent_move(vector<int> pos){
	board[pos[0]][pos[1]] = turn;
	turn = ((turn % 2) + 1);
}

vector<vector<int> > human_agent::get_board(){
	return board;
}
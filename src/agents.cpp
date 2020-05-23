#include "agents.h"
#include "tree.h"

agent::agent(string name, argdict gameParams, int turn): name(name), nr(gameParams.get_int_arg("nr")), nc(gameParams.get_int_arg("nc")), linesize(gameParams.get_int_arg("linesize")), turn(turn), last_move_at(-1){}


int agent::play_move(){}
void agent::opponent_move(int pos){}

random_rollout::random_rollout(string name, argdict gameParams, argdict TreeArgDict, int turn): agent(name, gameParams, turn){
	tree = new VanillaTree(TreeArgDict);
}

int random_rollout::play_move(){
	int next_move = tree->getMove();
	turn = (turn%2)+1;
	return next_move;
}

void random_rollout::opponent_move(int pos){
	tree->opponentMove(pos);
	turn = (turn%2)+1;
}

vector<vector<int> > random_rollout::get_board(){
	return tree->getBoard();
}

human_agent::human_agent(string name, argdict gameParams, int turn): agent(name, gameParams, turn){
	board = vector<vector<int> >(this->nr, vector<int>(this->nc, 0));
}

int  human_agent::play_move(){
	int pos[2];
	while (true){
		cout << "Enter your new mark position(pair of space separated integers):\t";
		cin >> pos[0] >> pos[1];
		if (pos[0] < 0 || pos[0] >= nr || pos[1] < 0 || pos[1] >= nc){
			cerr << "ERROR: Enter a pair of integers within board range[" <<nr << ","<<nc<<"]\n";
			continue;
		}
		if (board[pos[0]][pos[1]] != 0){
			cerr << "ERROR: Enter a position that is empty\n";
			continue;
		}
		board[pos[0]][pos[1]] = turn;
		turn = ((turn%2)+1);
		return pos[0]*nc+pos[1];
	}
}

void human_agent::opponent_move(int pos){
	board[pos/nc][pos%nc] = turn;
	turn = ((turn % 2) + 1);
}

vector<vector<int> > human_agent::get_board(){
	return board;
}


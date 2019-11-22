#include <bits/stdc++.h>
#include "game.h"
#include "tree.h"
#include "agents.h"
#include "utils.h"

using namespace std;

int main(int argc, char* argv[]){
	cout.precision(3);
	int board_size=atoi(argv[1]), linesize=atoi(argv[2]), num_rollouts=atoi(argv[3]), max_depth=atoi(argv[5]), timeout=atoi(argv[6]), mode=atoi(argv[7]), num_workers=atoi(argv[8]), verbose=atoi(argv[11]);

	float exploration_coeff=atof(argv[4]);
	float gamma=atof(argv[9]),alpha=atof(argv[10]),beta=atof(argv[11]);

	int N = board_size;
	float C = exploration_coeff;
	if (verbose > 1)
		cout << "args in cpp:\t" << N << "\t" << linesize << "\t" << num_rollouts << "\t" << C << "\t" << max_depth << "\t" << timeout << "\t" << mode << "\t" << num_workers << "\t" << verbose << endl;

	Game game(N, linesize, gamma, alpha, beta, verbose);
	agent *agent1, *agent2;

	vector<int> players = get_flags(mode, 2);
	// cout << "fobobofo\n" << endl;
	// cout << "verbose " << verbose << endl;
	if (verbose > 1){
		cout << "player1:\t" << players[0];
		cout << ";\tplayer2:" << players[1] << endl;
	}
	if (players.size() != 2){
		if (verbose > 0)
			cerr << "not enough players!\n";
		exit(-1);
	}

	string player_name0 = "player 1", player_name1 = "player 2";
	if (players[0] == 0){
		agent1 = new human_agent(&game, player_name0);
	}
	else{
		agent1 = new random_rollout(&game, player_name0, num_rollouts, C, max_depth, timeout, num_workers, players[0]);
	}

	if ((players[1] - 4 == 0)){ // TODO
		agent2 = new human_agent(&game, player_name1);
	}
	else{
		agent2 = new random_rollout(&game, player_name1, num_rollouts, C, max_depth, timeout, num_workers, players[1]-4);
	}

	agent* current_agent = agent1;
	agent* other_agent = agent2;

	vector<int> mov;
	vector<vector<int> > board = current_agent->get_board();
	vector<vector<int> > board2;// = current_agent->get_board();
	if (verbose > 0)
		print_board(board);
	unsigned int move = 0;
	while(true){
		move++;
		if (verbose > 0)
			cout<<current_agent->name<<"\'s turn"<<endl;
		mov = current_agent->play_move();
		other_agent->opponent_move(mov);
		board = current_agent->get_board();
		// board2 = other_agent->get_board();
		if (verbose > 0)
			print_board(board);
		int judgement = game.judge(board, mov);
		if(judgement){
			if (verbose > 0)
				cout<<current_agent->name<<" won!"<<endl;
			else{
				cout << current_agent->name[7] << endl;
			}
			return 0;
		}
		else{
			if (count_zeros(board) == 0){
				if (verbose > 0)
					cout << "It's a draw!" << endl;
				else
					cout << 0 << endl;
				return 0;
			}
		}
		swap(current_agent, other_agent);
	}
}

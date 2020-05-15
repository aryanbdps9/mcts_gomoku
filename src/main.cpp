#include <bits/stdc++.h>
#include "game.h"
#include "tree.h"
#include "agents.h"
#include "utils.h"

using namespace std;

int main(int argc, char* argv[]){
	cout.precision(3);
	int board_size=atoi(argv[1]), linesize=atoi(argv[2]), mode=atoi(argv[3]), verbose=atoi(argv[4]), num_rollouts_1=atoi(argv[5]), max_depth_1=atoi(argv[6]), timeout_1=atoi(argv[7]), num_workers_1=atoi(argv[8]), num_rollouts_2=atoi(argv[13]), max_depth_2=atoi(argv[14]), timeout_2=atoi(argv[15]), num_workers_2=atoi(argv[16]);

	float exploration_coeff_1 = atof(argv[9]), gamma_1 = atof(argv[10]), alpha_1 = atof(argv[11]), beta_1 = atof(argv[12]);
	float exploration_coeff_2 = atof(argv[17]), gamma_2 = atof(argv[18]), alpha_2 = atof(argv[19]), beta_2 = atof(argv[20]);
	float beta1_1 = atof(argv[21]), beta1_2 = atof(argv[22]);

	int N = board_size;
	float C_1 = exploration_coeff_1, C_2 = exploration_coeff_2;

	// if (verbose > 1)
	// 	cout << "args in cpp:\t" << N << "\t" << linesize << "\t" << num_rollouts << "\t" << C << "\t" << max_depth << "\t" << timeout << "\t" << mode << "\t" << num_workers << "\t" << verbose << endl;

	Game game(N, linesize, verbose);
	agent *agent1, *agent2;

	vector<int> players(2,0);
	players[1] = mode % 2;
	players[0] = (mode >= 2) ? 1 : 0;
	if (verbose > 1){
		cout << "player1:\t" << players[0];
		cout << ";\tplayer2:" << players[1] << endl;
	}
	string player_name0 = "player 1", player_name1 = "player 2";

	if (players[0] == 0){
		agent1 = new human_agent(&game, player_name0);
	}
	else{
		agent1 = new random_rollout(&game, player_name0, num_rollouts_1, C_1, max_depth_1, timeout_1, num_workers_1, gamma_1, alpha_1, beta_1, beta1_1);
	}

	if ((players[1] == 0)){ // TODO
		agent2 = new human_agent(&game, player_name1);
	}
	else{
		agent2 = new random_rollout(&game, player_name1, num_rollouts_2, C_2, max_depth_2, timeout_2, num_workers_2, gamma_2, alpha_2, beta_2, beta1_2);
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
			cout << "last move at " << mov[0] << "\t" << mov[1] << endl;
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

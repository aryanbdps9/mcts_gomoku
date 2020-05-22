#include <bits/stdc++.h>
// #include "game.h"
#include "tree.h"
#include "agents.h"
#include "utils.h"
#include "argdict.h"
#include "judge_and_potential.h"

using namespace std;

int main(int argc, char* argv[]){
	throw NotImplementedException(); // implement setGameover fn
	srand(time(NULL));
	cout.precision(3);
	int board_size=atoi(argv[1]), linesize=atoi(argv[2]), mode=atoi(argv[3]), verbose=atoi(argv[4]), num_rollouts_1=atoi(argv[5]), max_depth_1=atoi(argv[6]), timeout_1=atoi(argv[7]), num_workers_1=atoi(argv[8]), num_rollouts_2=atoi(argv[13]), max_depth_2=atoi(argv[14]), timeout_2=atoi(argv[15]), num_workers_2=atoi(argv[16]);

	float exploration_coeff_1 = atof(argv[9]), gamma_1 = atof(argv[10]), alpha_1 = atof(argv[11]), beta_1 = atof(argv[12]);
	float exploration_coeff_2 = atof(argv[17]), gamma_2 = atof(argv[18]), alpha_2 = atof(argv[19]), beta_2 = atof(argv[20]);
	float beta1_1 = atof(argv[21]), beta1_2 = atof(argv[22]);

	int N = board_size;
	float C_1 = exploration_coeff_1, C_2 = exploration_coeff_2;

	argdict gameParams;
	gameParams.add_int_arg("nr", N);
	gameParams.add_int_arg("nc", N);
	gameParams.add_int_arg("linesize", linesize);
	// cout << "C_1 = " << C_1 << "\tC_2 = " << C_2 << endl;
	// cout << "gameParams(nr, nc, linesize) = " << gameParams.has_int("nr") << endl;
	// cout << "gameParams(nr, nc, linesize) = " << gameParams.get_int_arg("nr") << endl;

	argdict TreeArgDict_1, TreeArgDict_2;
	TreeArgDict_1.add_int_arg("nr", N);
	TreeArgDict_1.add_int_arg("nc", N);
	TreeArgDict_1.add_dbl_arg("C", exploration_coeff_1);
	TreeArgDict_1.add_int_arg("num_rollouts", num_rollouts_1);
	TreeArgDict_1.add_int_arg("num_rollout_workers", num_workers_1);
	TreeArgDict_1.add_int_arg("max_depth", max_depth_1);
	TreeArgDict_1.add_int_arg("timeout", timeout_1);
	TreeArgDict_1.add_int_arg("linesize", linesize);
	TreeArgDict_1.add_dbl_arg("gamma", gamma_1);
	TreeArgDict_1.add_dbl_arg("alpha", alpha_1);
	TreeArgDict_1.add_dbl_arg("beta", beta_1);
	TreeArgDict_1.add_dbl_arg("beta1", beta1_1);
	TreeArgDict_1.add_int_arg("turn", 1);
	
	TreeArgDict_2.add_int_arg("nr", N);
	TreeArgDict_2.add_int_arg("nc", N);
	TreeArgDict_2.add_dbl_arg("C", exploration_coeff_2);
	TreeArgDict_2.add_int_arg("num_rollouts", num_rollouts_2);
	TreeArgDict_2.add_int_arg("num_rollout_workers", num_workers_2);
	TreeArgDict_2.add_int_arg("max_depth", max_depth_2);
	TreeArgDict_2.add_int_arg("timeout", timeout_2);
	TreeArgDict_2.add_int_arg("linesize", linesize);
	TreeArgDict_2.add_dbl_arg("gamma", gamma_2);
	TreeArgDict_2.add_dbl_arg("alpha", alpha_2);
	TreeArgDict_2.add_dbl_arg("beta", beta_2);
	TreeArgDict_2.add_dbl_arg("beta1", beta1_2);
	TreeArgDict_2.add_int_arg("turn", 1);

	// cout << "C_1ta = " << TreeArgDict_1.get_dbl_arg("C") << endl;

	// if (verbose > 1)
	// 	cout << "args in cpp:\t" << N << "\t" << linesize << "\t" << num_rollouts << "\t" << C << "\t" << max_depth << "\t" << timeout << "\t" << mode << "\t" << num_workers << "\t" << verbose << endl;

	// Game game(N, linesize, verbose);
	agent *agent1, *agent2;

	vector<int> players(2,0);
	players[1] = mode % 2;
	players[0] = (mode >= 2) ? 1 : 0;
	printf("mode  = %d\n", mode);
	if (verbose > 1){
		cout << "player1:\t" << players[0];
		cout << ";\tplayer2:" << players[1] << endl;
	}
	string player_name0 = "player 1", player_name1 = "player 2";

	if (players[0] == 0){
		printf("creating human agent\n");
		agent1 = new human_agent(player_name0, gameParams, 1);
		printf("created human agent\n");
	}
	else{
		agent1 = new random_rollout(player_name0, gameParams, TreeArgDict_1, 1);
	}

	if ((players[1] == 0)){ // TODO
		agent2 = new human_agent(player_name1, gameParams, 2);
	}
	else{
		agent2 = new random_rollout(player_name1, gameParams, TreeArgDict_2, 2);
	}

	agent1->setVerbosity(verbose);
	agent2->setVerbosity(verbose);

	agent* current_agent = agent1;
	agent* other_agent = agent2;

	int mov;
	vector<vector<int> > board = current_agent->get_board();
	vector<vector<int> > board2;// = current_agent->get_board();
	if (verbose > 0)
		print_board(board);
	unsigned int move = 0;
	while(true){
		move++;
		if (verbose > 0)
			cout<<current_agent->name<<"\'s turn"<<endl;
		// cout << "calling play_move!" << endl;
		mov = current_agent->play_move();
		// cout << "called play_move!" << endl;
		other_agent->opponent_move(mov);
		board = current_agent->get_board();
		// board2 = other_agent->get_board();
		if (verbose > 0)
			print_board_w_last_move(board, mov);
			// print_board(board);
			cout << "last move at " << mov / N << "\t" << mov % N << endl;
		int judgement = judge(board, mov, linesize);
		if(judgement){
			if (verbose > 0)
				cout<<current_agent->name<<" won!"<<endl;
			else{
				cout << current_agent->name[7] << endl;
			}
			return 0;
		}
		else if (count_zeros(board) == 0){
			if (verbose > 0)
				cout << "It's a draw!" << endl;
			else
				cout << 0 << endl;
			return 0;
		}
		swap(current_agent, other_agent);
	}
}


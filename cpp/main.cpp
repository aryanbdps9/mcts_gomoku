#include <bits/stdc++.h>
#include "game.h"
#include "tree.h"
#include "agents.h"
#include "utils.h"

using namespace std;

int main(int argc, char* argv[]){
	cout.precision(3);
	int board_size=atoi(argv[1]), linesize=atoi(argv[2]), num_rollouts=atoi(argv[3]), max_depth=atoi(argv[5]), timeout=atoi(argv[6]), selfplay=atoi(argv[7]), num_workers=atoi(argv[8]);
	float exploration_coeff=atof(argv[4]);
	float gamma=atof(argv[9]),alpha=atof(argv[10]),beta=atof(argv[11]);

	int N = board_size;
	float C = exploration_coeff;
	cout << N << "\t" << linesize << "\t" << num_rollouts << "\t" << C << "\t" << max_depth << "\t" << timeout << "\t" << selfplay << "\t" << num_workers << endl;

	Game game(N, linesize, gamma, alpha, beta);
	agent *agent1, *agent2;

	cout << "Do you want to play first? [y/N]: ";
	char c;
	cin >> c;
	string name1="X", name2="O";
	if (!(c == 'Y' || c == 'y')){
		swap(name1, name2);
	}
	if (!selfplay){
		agent1 = new human_agent(&game, name1, num_rollouts, C, max_depth, timeout, num_workers);
	}
	else{
		agent1 = new random_rollout(&game, name1, num_rollouts, C, max_depth, timeout, num_workers);
	}
	agent2 = new random_rollout(&game, name2, num_rollouts, C, max_depth, timeout, num_workers);
	if (!(c == 'Y' || c == 'y')){
		swap(agent1, agent2);
	}

	agent* current_agent = agent1;
	agent* other_agent = agent2;

	vector<int> mov;
	vector<vector<int> > board = current_agent->get_board();
	vector<vector<int> > board2;// = current_agent->get_board();
	print_board(board);
	while(true){
		cout<<current_agent->name<<"\'s turn"<<endl;
		mov = current_agent->play_move();
		other_agent->opponent_move(mov);
		board = current_agent->get_board();
		// board2 = other_agent->get_board();
		print_board(board);
		int judgement = game.judge(board, mov);
		if(judgement){
			cout<<current_agent->name<<" won!"<<endl;
			return 0;
		}
		else{
			if (count_zeros(board) == 0){
				cout << "It's a draw!" << endl;
				return 0;
			}
		}
		swap(current_agent, other_agent);
	}

}

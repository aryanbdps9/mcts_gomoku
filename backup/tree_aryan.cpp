#include <bits/stdc++.h>

// ###########################

class Node{
public:
	Tree *tree;
	vector< vector<int> > board;
	Node *parent;
	vector<Node*> children;
    vector<vector<int> > actions;
	double value = 0.0;
	int visits = 0;
	int depth;
	int self.turn;
	vector<int>parent_action(2);
	int gameover;

	Node(Tree tree, vector< vector<int> > board, int parent_action=-1, Node * parent=NULL, int gameover = 0, int turn = -1){
		self.tree = tree;
		self.board = board;
		self.parent = parent;
		self.depth = parent==NULL?1:parent->depth+1;
		self.turn = parent==NULL?turn:(parent->depth%2)+1;
		self.parent_action = parent_action;
		self.gameover = gameover;
	}

	void select(){
		visits++;
		tree->T++;
		if (depth >= tree->maxdepth){
			value = perform_rollouts(board, turn, parent_action, tree);
			return;
		}

		if (children.size() == 0){
			Node.generate_children(this, children, actions);
		if (children.size() == 0){
			value = 0.0;
			return;
		}
		int opp_move = ((turn%2)+1);
		int bestUCT, best_idx = -1;
		float uctval;
		int idx = -1;
		for (auto child: children){
			idx += 1;
			if (child->gameover == turn){
				best_idx = idx;
				gameover = turn;
				break;
			}
			else if (child->gameover == opp_move){
				uctval = (0-2) + 0;
			}
			else{
				int uct_opp, exploration_bonus;
				child.calcUCT(uct_opp, exploration_bonus);
				uctval = (0-uct_opp) + exploration_bonus;
			}
			if (best_idx < 0 || uctval >= bestUCT){
				bestUCT = uctval;
				best_idx = idx;
			}
		}

		if (gameover == turn){
			value = 1;
			return;
		}

		children[best_idx]->select();

		// SETTING VALUE
		value = 0.0;
		for (auto child : children) value += child->value*child->visits;
		value /= visits;
		return;
	}

	void calcUCT(int& uct_opp, int& exploration_bonus){
		uct_opp = value;
		exploration_bonus = tree->C * sqrt(2*log(tree.T+1) / (visits+1));
		return;
	}
}

void rollout_workers(vector<vector<int> > board, int current_player, vector<int> last_move_at, Tree* tree, int* ret){
	while (true){
		if (last_move_at[0] < 0 || last_move_at[1] < 0){
			judgement = tree->game->judge(board, last_move_at);
			if (!judgement){
				*ret = judgement;
				return;
			}
		}
		next_move_at = tree->game->policies[current_player-1]->get_action(board, current_player);
		if (next_move_at[0] < 0 || next_move_at[1] < 0){
			return 0;
		}
		board[next_move_at[0], next_move_at[1]] = current_player;
		current_player = ((current_player%2)+1);
	}
}

double perform_rollouts(vector<vector<int> > board, int current_player, vector<int> last_move_at, Tree *tree){
	int num_rollouts = tree->num_rollouts;
	int num_workers = tree->num_workers;
	int curvals[num_workers];
	thread workers[num_workers];
	int jobs_left = tree->num_rollouts;
	double val = 0.0d;
	int opponent = ((current_player%2)+1);
	while(jobs_left > 0){
		int batch_size = 0;
		for(int i = 0; i < num_workers && i < jobs_left; i++, batch_size++){
			workers[i] = thread(rollout_workers, board, current_player, last_move_at, tree, curvals+i);
		}
		for(int i = 0; i < batch_size; i++){
			workers[i].join();
			int cv = curvals[i];
			if (cv == current_player) val++;
			else if (cv == opponent) val--;
			else: val+=0;
		}
		jobs_left -= batch_size;
	}
	val /= num_rollouts;
	return val;
}

#include<bits/stdc++.h>
#include "tree.h"
#include "utils.h"

using namespace std;


void rollout_workers(vector<vector<int> > board, int current_player, vector<int> last_move_at, Tree* tree, int* ret){
	int judgement;
	while (true){
		if (!(last_move_at[0] < 0 || last_move_at[1] < 0)){
			judgement = tree->game->judge(board, last_move_at);
			if (judgement){
				*ret = judgement;
				return;
			}
		}
		last_move_at = tree->game->policies[current_player-1]->get_action(board, current_player);
		if (last_move_at[0] < 0 || last_move_at[1] < 0){
			*ret = 0;
			return;
		}
		board[last_move_at[0]][last_move_at[1]] = current_player;
		current_player = ((current_player%2)+1);
	}
}

double perform_rollouts(vector<vector<int> > board, int current_player, vector<int> last_move_at, Tree *tree){
	int num_rollouts = tree->num_rollouts;
	int num_workers = tree->num_workers;
	int curvals[num_workers];
	thread workers[num_workers];
	int jobs_left = tree->num_rollouts;
	double val = 0.0;
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
			else val+=0;
		}
		jobs_left -= batch_size;
	}
	val /= num_rollouts;
	return val;
}

Tree::Tree(Game * game, int num_rollouts, double C, int max_depth, int timeout, int num_workers){
	this->game = game;
	this->num_rollouts = num_rollouts;
	this->C = C;
	this->maxdepth = max_depth;
	this->timeout = timeout;
	this->num_workers = num_workers;
	int n = this->game->n;
	vector<int> parent_action = {-1,-1};
	vector<vector<int> > board(n, vector<int> (n, 0));
	this->root = new Node(this, board, parent_action, NULL,0, 0, 1);
}

vector<vector<int> > Tree::play_one_move(vector<int> &mymove){
	int num_selects = 0;
	auto t_start = chrono::high_resolution_clock::now();
	auto t_end = chrono::high_resolution_clock::now();
	auto exec_time = chrono::duration_cast<chrono::duration<double>>(t_end - t_start) .count();
	while(exec_time < this->timeout){
		t_end = chrono::high_resolution_clock::now();
		exec_time = chrono::duration_cast<chrono::duration<double>>(t_end - t_start) .count();
		root->select();
		num_selects++;
	}


	vector<Node *> children = root->children;
	int num_child = children.size();
	double best_val = -1e9;
	int best_idx = -1;
	for(int i=0; i<num_child; i++){
		Node * child = children[i];
		double val = -child->value;
		if(best_idx == -1 or val >= best_val){
			best_val = val;
			best_idx = i;
		}
	}

	Node * best_child = children[best_idx];
	root->print_value();

	
	cout << "printing VAL/EB mat\n";
	print_mat(root->get_ValToEBRatio_mat());

	cout << "printing exploration_bonus mat\n";
	print_mat(root->get_ExpBon_mat());
	
	cout << "printing value mat\n";
	print_mat(root->get_Val_mat());

	cout << "printing UCT mat\n";
	print_mat(root->get_UCT_mat());

	cout << "printing visit mat\n";
	print_mat(root->get_visit_mat());

	root = best_child;
	root->parent = NULL;
	mymove = root->parent_action;
	root->parent_action = {-1, -1};
	root->offset_depth(-1);
	cout<<"performed "<<num_selects<<" iterations"<<endl;
	return root->board;
}

vector<vector<int> > Tree::player_move(vector<int> place){
	Node * newroot = NULL;
	vector<Node *> children = root->children;
	vector<vector<int> > actions = root->actions;
	int num_child = children.size();
	for(int i=0; i<num_child; i++){
		if(actions[i][0] == place[0] && actions[i][1] == place[1]){
			newroot = children[i];
		}
	}
	if (newroot == NULL){
		vector<vector<int> > newboard = root->board;
		newboard[place[0]][place[1]] = root->turn;
		int potential = game->potential(newboard, place);
		// int potential = 0;
		// Node * child = new Node(node->tree, newboard, position, node, potential, 0, -1);
		newroot = new Node(root->tree, newboard, place, root, potential);
	}
	root = newroot;
	root->parent = NULL;
	root->parent_action = {-1, -1};
	root->offset_depth(-1);
	return root->board;
}

Node::Node(Tree * tree, vector< vector<int> > board, vector<int> parent_action, Node * parent, int potential, int gameover, int turn){
	this->tree = tree;
	this->board = board;
	this->parent = parent;
	this->depth = parent==NULL?1:parent->depth+1;
	this->turn = parent==NULL?turn:(parent->turn%2)+1;
	this->parent_action = parent_action;
	this->gameover = gameover;
	this->potential = potential;
}

void Node::generate_children(Node * node, vector<Node*> &children, vector<vector<int> > &actions){
	vector< vector<int> > board = node->board;
	int N = board.size();
	int c = 0;
	for(int i=0; i<N; i++){
		for(int j=0; j<N; j++){
			if(board[i][j] == 0){
				c++;
				vector< vector<int> > newboard = board;
				newboard[i][j] = node->turn;
				vector<int> position = {i,j};
				int potential = node->tree->game->potential(newboard, position);
				// int potential = 0;
				Node * child = new Node(node->tree, newboard, position, node, potential, 0, -1);
				int judgement = child->tree->game->judge(newboard, position);
				if(judgement == node->turn){
					child->gameover = node->turn;
					children.clear();
					actions.clear();
					children.push_back(child);
					actions.push_back(position);
					return;
				}
				children.push_back(child);
				actions.push_back(position);
			}
		}
	}
}

void Node::offset_depth(int offset){
	this->depth += offset;
	int N = children.size();
	for(int i=0; i<N; i++){
		children[i]->offset_depth(offset);
	}
}

void Node::select(){
	visits++;
	tree->T++;
	if (depth >= tree->maxdepth){
		value = perform_rollouts(board, turn, parent_action, tree);
		return;
	}

	if (children.size() == 0)
		Node::generate_children(this, children, actions);
	if (children.size() == 0){
		value = 0.0;
		return;
	}
	int opp_move = ((turn%2)+1);
	int best_idx = -1;
	double bestUCT;
	double uctval;
	int idx = -1;
	for (auto child: children){
		idx += 1;
		if (child->gameover == turn){
			best_idx = idx;
			gameover = turn;
			break;
		}
		else if (child->gameover == opp_move){
			// uctval = (0-2) + 0;
			double uct_opp, exploration_bonus;
			child->calcUCT(uct_opp, exploration_bonus);
			uctval = (0 - uct_opp) + exploration_bonus;
			// uctval = (0-2) + 0;
		}
		else{
			double uct_opp, exploration_bonus;
			child->calcUCT(uct_opp, exploration_bonus);
			uctval = (0-uct_opp) + exploration_bonus;
		}
		if (best_idx < 0 || uctval > bestUCT){
			bestUCT = uctval;
			best_idx = idx;
		}
	}

	if (gameover == turn){
		// cout << "value 100\n";
		// value = 100.0;
		value = 1.0;
		return;
	}

	children[best_idx]->select();

	// SETTING VALUE
	value = 0.0;
	for (auto child : children){
		double len_reward, val_reward;
		len_reward = tree->game->alpha*(child->potential - this->potential);
		val_reward = tree->game->gamma * child->value;
		value += (len_reward+val_reward) * child->visits;
		if (child->gameover){
			cout << "child [" << child->parent_action[0] << "," << child->parent_action[1] << "]\t" << "len reward " << len_reward << "\tval_reward" << val_reward << endl;
		}
	}
	// for (auto child : children) value += (tree->game->alpha*(child->potential - this->potential) + tree->game->gamma * child->value)*child->visits;
	// for (auto child : children) value += child->value*child->visits;
	value /= visits;
	return;
}

void Node::print_value(){

	int N = children.size();
	cout << "print_value: N" << N << endl;
	cout << "[";
	for(int i=0; i<N; i++){
		double val = children[i]->value;
		if(val != 0.0){
			cout<<"["<<actions[i][0]<<", "<<actions[i][1]<<"]: "<<val<<"\t";
		}
	}
	cout << "]\n";
}

void Node::calcUCT(double & uct_opp, double & exploration_bonus){
	uct_opp = value;
	exploration_bonus = tree->C * sqrt(2*log(tree->T+1) / (visits+1));
	return;
}
double Node::calcExplornBonus(){
	return tree->C * sqrt(2*log(tree->T+1) / (visits+1));
}

vector<vector<int> > Node::get_visit_mat(){
	vector<vector<int> > ret;
	int n = board.size();
	ret.resize(n);
	int r, c;
	for(int i = 0; i < n; i++){
		ret[i].resize(n);
	}
	for (int i = 0; i < children.size(); i++){
		r = actions[i][0];
		c = actions[i][1];
		ret[r][c] = children[i]->visits;
	}
	return ret;
}
vector<vector<double> > Node::get_UCT_mat(){
	vector<vector<double> > ret;
	int n = board.size();
	ret.resize(n);
	int r, c;
	for(int i = 0; i < n; i++){
		ret[i].resize(n);
	}
	for (int i = 0; i < children.size(); i++){
		r = actions[i][0];
		c = actions[i][1];
		ret[r][c] = children[i]->calcExplornBonus() - children[i]->value;
	}
	return ret;
}
vector<vector<double> > Node::get_ExpBon_mat(){
	vector<vector<double> > ret;
	int n = board.size();
	ret.resize(n);
	int r, c;
	for(int i = 0; i < n; i++){
		ret[i].resize(n);
	}
	for (int i = 0; i < children.size(); i++){
		r = actions[i][0];
		c = actions[i][1];
		ret[r][c] = children[i]->calcExplornBonus();
	}
	return ret;
}
vector<vector<double> > Node::get_Val_mat(){
	vector<vector<double> > ret;
	int n = board.size();
	ret.resize(n);
	int r, c;
	for(int i = 0; i < n; i++){
		ret[i].resize(n);
	}
	for (int i = 0; i < children.size(); i++){
		r = actions[i][0];
		c = actions[i][1];
		ret[r][c] = children[i]->value;
	}
	return ret;
}

vector<vector<double> > Node::get_ValToEBRatio_mat(){
	vector<vector<double> > ret;
	int n = board.size();
	ret.resize(n);
	int r, c;
	for(int i = 0; i < n; i++){
		ret[i].resize(n);
	}
	for (int i = 0; i < children.size(); i++){
		r = actions[i][0];
		c = actions[i][1];
		ret[r][c] = children[i]->value / children[i]->calcExplornBonus();
	}
	return ret;
}
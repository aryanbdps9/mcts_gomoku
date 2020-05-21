#include<bits/stdc++.h>
#include "tree.h"
#include "utils.h"
#include "shuffle_set.h"

using namespace std;

template <typename TreeType>
BaseNode<TreeType>::BaseNode(): tree(nullptr), parent(nullptr), value(0.0), turn(0), parent_action(-1){}

template <typename TreeType>
BaseNode<TreeType>::BaseNode(int parent_action, BaseNode<TreeType>* parent): tree(parent->tree), board(board), parent(parent), parent_action(parent_action){
	assert(parent != nullptr);
	turn = (parent->turn%2)+1;
	int nr = board.size();//, nc = board[0].size();
	board[parent_action/nr][parent_action%nr] = parent->turn;
}

template <typename TreeType>
BaseNode<TreeType>::BaseNode(TreeType* tree, vector<vector<int> > board, uint32_t turn): tree(tree), board(board), parent(nullptr), parent_action(-1), turn(turn){}

template <typename TreeType>
BaseNode<TreeType>::~BaseNode(){
	for (auto &child : children) {
		delete child.second;
	}
}

template <typename TreeType>
bool BaseNode<TreeType>::killAllExcept(int action){
	for (auto &child : children) {
		if (child.first == action) continue;
		delete child.second;
	}
	if (children.find(action) != children.end()){
		auto goodchild = children[action];
		children.clear();
		children[action] = goodchild;
	}
	return true;
}

template <typename TreeType>
int BaseNode<TreeType>::select(){
	if (children.size() == 0) return -1;
	auto first_child = *children.begin();
	int best_action = first_child.first;
	double best_uct = first_child.second->getUCT();

	double current_uct;
	for (auto &child : children){
		current_uct = child.second->getUCT();
		if (current_uct >= best_uct){
			best_uct = current_uct;
			best_action = child.first;
		}
	}
	return best_action;
}

template <typename TreeType>
double BaseNode<TreeType>::getUCT(){
	double v = -value;
	double exploration_bonus = tree->C * sqrt(2*log(parent->visits + 1) / (visits+1.0));
	return v + exploration_bonus;
}

template <typename TreeType>
bool BaseNode<TreeType>::isEmptyLocn(int a){
	int nc = board[0].size();
	return board[a/nc][a%nc] == 0;
}

template <typename TreeType>
bool BaseNode<TreeType>::isLeaf(){
	return children.size() == 0;
}

template <typename TreeType>
bool BaseNode<TreeType>::isRoot(){
	return parent == nullptr;
}

template <typename TreeType>
void BaseNode<TreeType>::inc_visit(){
	visits++;
}

template <typename TreeType>
uint32_t BaseNode<TreeType>::get_visits(){
	return visits;
}

template <typename TreeType>
VanillaNode<TreeType>::~VanillaNode(){}

template <typename TreeType>
double VanillaNode<TreeType>::getUCT(){
	assert(this->parent != nullptr);
	int opp_move = ((this->turn%2)+1);
	double val_opp, exploration_bonus, total_child_potential;
	total_child_potential = this->tree->beta1 ? static_cast<VanillaNode<TreeType>*>(this->parent)->total_child_potential : 0.0;
	//////
	// if (this->beta1 != 0){
		// total_child_potential =  this->parent->total_child_potential;
	// 	assert(this->parent != nullptr);
	// }
	// else total_child_potential = 0;
	//////
	this->calcUCT(val_opp, exploration_bonus, total_child_potential);
	if (this->gameover == this->turn){ // remember, this is child, calcing UCT for parent!
		return (0-1) + exploration_bonus;
	} else if (this->gameover == opp_move){
		return std::numeric_limits<double>::max() / 16;
	}
	return (0-val_opp) + exploration_bonus;
}

template <typename TreeType>
void VanillaNode<TreeType>::calcUCT(double& uct_opp, double& exploration_bonus, int total_child_potential){
	uct_opp = this->value;
	exploration_bonus = this->tree->C * sqrt(2*log(this->parent->visits + 1.0) / (this->visits+1.0));

	exploration_bonus += (this->tree->beta * (potential)) / (1.0 + this->visits);
	if (total_child_potential > 0 && this->tree->beta1 != 0){
		double p_sa = double(potential) / double(total_child_potential);
		assert(this->parent != NULL);
		exploration_bonus += this->tree->beta1 * p_sa * sqrt(this->parent->visits) / (1.0 + this->visits);
	}
	return;
}

template <typename TreeType>
int BaseNode<TreeType>::expand(){
	if (!children.empty()) return 0;
	int nr = board.size(), nc = board[0].size();
	int count = 0;
	for (int i = 0; i < nr * nc; i++){
		if (board[i/nr][i%nr] != 0) continue;
		children[i] = new BaseNode<TreeType>(i, this);
		count++;
	}
	return count;
}


template <typename TreeType>
int VanillaNode<TreeType>::updateTCP(){
	throw NotImplementedException();
}


template <typename TreeType>
void VanillaNode<TreeType>::offsetDepth(int offset){
	throw NotImplementedException();
}

template <typename TreeType>
VanillaNode<TreeType>::VanillaNode():BaseNode<TreeType>(){
	this->depth = -1;
	this->potential = 0;
}

template <typename TreeType>
VanillaNode<TreeType>::VanillaNode(int parent_action, VanillaNode<TreeType>* parent): BaseNode<TreeType>(parent_action, parent){
	this->depth = parent->depth+1;
	this->potential=this->tree->potential_fn(this->board, parent_action, this->gameover);
}

template <typename TreeType>
VanillaNode<TreeType>::VanillaNode(TreeType* tree, vector<vector<int> > board, uint32_t turn): BaseNode<TreeType>(tree, board, turn){
	this->depth=1;
	this->potential = 0;
	this->gameover = 0;
}

template <typename TreeType>
int VanillaNode<TreeType>::expand(){
	if (!this->children.empty()) return 0;
	int nr = this->board.size(), nc = this->board[0].size();
	int count = 0;
	VanillaNode<TreeType>* new_node;
	this->total_child_potential = 0;
	for (int i = 0; i < nr * nc; i++){
		if (this->board[i/nr][i%nr] != 0) continue;
		new_node = new VanillaNode<TreeType>(i, this);
		this->children[i] = new_node;
		this->total_child_potential += new_node->getPotential();
		count++;
	}
	return count;
}

BaseTree::BaseTree(): C(sqrt(2)){}

// BaseTree::BaseTree(BaseNode<BaseTree>* root): root(root){}

BaseTree::BaseTree(double C): C(C){}

BaseTree::~BaseTree(){
	// delete root;
}

int BaseTree::gameJudge(vector<vector<int> > &board, int last_action){
	throw NotImplementedException();
	return 0;
}

/* 
bool BaseTree::changeRoot(int action){
	if (!root->isEmptyLocn(action)) return false;
	root->killAllExcept(action);
	BaseNode<BaseTree>* newroot;
	if (root->children.find(action) != root->children.end()){
		newroot = root->children[action];
		root->children.erase(action);
	} else{
		newroot = root;
	}
	return true;
}
 */

VanillaTree::VanillaTree():num_rollout_workers(4), num_rollouts(20), max_depth(5), timeout(10000){}

VanillaTree::VanillaTree(int num_rollouts, double C, int max_depth, int timeout, int num_workers, double gamma, double alpha, double beta, double beta1): BaseTree(C), num_rollouts(num_rollouts), max_depth(max_depth), timeout(timeout), num_rollout_workers(num_rollout_workers), gamma(gamma), alpha(alpha), beta(beta), beta1(beta1){}

VanillaTree::~VanillaTree(){
	throw NotImplementedException();
}

void VanillaTree::playout(){
	/*
	TODO: consider what happens when you encounter a node with game over
	Also consider the Multithreaded scenario, where, you might delete a subtree and some worker is in that subtree at that time.
	*/
	VanillaNode<VanillaTree>* current_node = root;
	int current_action;
	current_node->expand();
	unsigned int depth = 1;
	while(!current_node->isLeaf()){
		current_action = current_node->select(); // TODO use lock for MT
		current_node->inc_visit(); // TODO use lock for MT
		current_node = static_cast<VanillaNode<VanillaTree>*>( current_node->children[current_action]);
		depth++;
		if (depth < max_depth) current_node->expand();
	}
	current_node->inc_visit(); // TODO use lock for MT

	double leaf_value = getRolloutValue(current_node);
	current_node = static_cast<VanillaNode<VanillaTree>*>(current_node->parent);
	double temp_value, len_reward, val_reward;

	while (current_node != nullptr){
		temp_value = 0.0;
		for (auto [action, child]: current_node->children){
			len_reward = alpha*static_cast<VanillaNode<VanillaTree>*>(child)->potential;
			val_reward = -(gamma * child->value);
			temp_value += (len_reward+val_reward)*child->visits;
		}
		current_node->set_value(temp_value/current_node->visits); // TODO use lock for MT
		current_node = static_cast<VanillaNode<VanillaTree>*>(current_node->parent);
	}
}

int VanillaTree::getMove(){
	uint64_t num_selects = 0;
	auto t_start = chrono::high_resolution_clock::now();
	auto t_end = chrono::high_resolution_clock::now();
	auto exec_time = chrono::duration_cast<chrono::duration<double> >(t_end - t_start) .count();
	while(exec_time < this->timeout){
		t_end = chrono::high_resolution_clock::now();
		exec_time = chrono::duration_cast<chrono::duration<double> >(t_end - t_start) .count();
		playout();
		num_selects++;
	}
	double best_val = std::numeric_limits<double>::min();
	double best_action = -1;
	for (auto [action, child]: root->children){
		if (best_action == -1 || best_val <= child->value){
			best_val = child->value;
			best_action = action;
		}
	}

	root->killAllExcept(best_action);
	VanillaNode<VanillaTree>* newroot = static_cast<VanillaNode<VanillaTree>*>(root->children[best_action]);
	root->children.clear();
	delete root;
	newroot->parent = nullptr;
	newroot->parent_action = -1;

	root = newroot;
	root->offsetDepth(-1);

	return best_action;
}

void rollout_worker(const vector<vector<int> > &board_, int last_move_at, int budget_steps, const int max_rollout_len, int (*judgement) (vector<vector<int> >&, int), int *total_rollouts_performed, int *total_rollouts_completed, int* total_reward_plr1){
	vector<vector<int> > board;
	int num_rollouts_performed = 0, num_rollouts_completed = 0, num_plr1_wins = 0, num_plr1_loss = 0, num_draws = 0;
	int game_end_result;
	int last_plr, next_plr;
		int outcome;

	const int nr = board_.size();
	int last_action, next_action;
	const vector<int> free_actions_vec = free_locns(board_);
	shuffle_set<int> free_actions;
	
	while(budget_steps > 0){
		board = board_;
		last_action = last_move_at;
		free_actions.init(free_actions_vec.begin(), free_actions_vec.end());
		for (int steps_in_a_rollout = 0; steps_in_a_rollout < max_rollout_len; steps_in_a_rollout++){
			last_plr = board[last_action/nr][last_action%nr];
			next_plr = (last_plr%2) + 1;
			next_action = free_actions.take_random_elem();

			board[next_action/nr][next_action%nr] = next_plr;
			last_plr = next_plr;
			budget_steps--;
			last_action = next_action;

			outcome = (*judgement)(board, next_action);
			if (outcome != 0 || free_actions.empty()){
				num_rollouts_completed++;
				num_plr1_wins += int(outcome == 1);
				num_plr1_loss += int(outcome == 0);
				if (free_actions.empty()){
					num_draws++;
				}
				break;
			}
		}
		num_rollouts_performed++;
	}
	// int *total_rollouts_performed, int *total_rollouts_completed, int* total_reward_plr1
	*total_rollouts_performed = num_rollouts_performed;
	*total_rollouts_completed = num_rollouts_completed;
	*total_reward_plr1 = (num_plr1_wins - num_plr1_loss);
}

double VanillaTree::getRolloutValue(VanillaNode<VanillaTree>* leaf){
	/*
	Each worker needs to spit out the following things:
	- Total number of rollouts performed
	- Total number of rollouts completed
	- Sum of rewards of all rollouts wrt player 1.
	*/
	throw NotImplementedException();
	return 0.0;
}















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

Tree::Tree(Game * game, int num_rollouts, double C, int max_depth, int timeout, int num_workers, double gamma, double alpha, double beta, double beta1){
	this->game = game;
	this->num_rollouts = num_rollouts;
	this->C = C;
	this->maxdepth = max_depth;
	this->timeout = timeout;
	this->num_workers = num_workers;
	this->gamma = gamma;
	this->alpha = alpha;
	this->beta = beta;
	this->beta1 = beta1;
	int n = this->game->n;
	vector<int> parent_action = {-1,-1};
	vector<vector<int> > board(n, vector<int> (n, 0));
	this->root = new Node(this, board, parent_action, NULL, 0, 0, 1);
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

	if (game->verbose > 1){
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
	
		cout<<"performed "<<num_selects<<" iterations"<<endl;
	}

	root = best_child;
	root->parent = NULL;
	mymove = root->parent_action;
	root->parent_action = {-1, -1};
	root->offset_depth(-1);
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
					child->value = -1;
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
	double prior_bonus = 0.0;
	int total_child_potential = 0;
	if (tree->beta1 != 0.0){
		for (auto child: children){
			total_child_potential += child->potential;
		}
	}
	for (auto child: children){
		idx += 1;
		if (child->gameover == turn){
			best_idx = idx;
			gameover = turn;
			double len_reward = tree->alpha * child->potential;
			value = 1.0 + len_reward;
			return;
		}
		else if (child->gameover == opp_move){
			double uct_opp, exploration_bonus;
			child->calcUCT(uct_opp, exploration_bonus, total_child_potential);
			uctval = (0-1) + exploration_bonus;
		}
		else{
			double uct_opp, exploration_bonus;
			child->calcUCT(uct_opp, exploration_bonus, total_child_potential);
			uctval = (0-uct_opp) + exploration_bonus;
		}

		if (best_idx < 0 || uctval > bestUCT){
			bestUCT = uctval;
			best_idx = idx;
		}
	}

	children[best_idx]->select();

	// SETTING VALUE
	value = 0.0;
	for (auto child : children){
		double len_reward, val_reward;
		len_reward = tree->alpha*child->potential;
		val_reward = -(tree->gamma * child->value);
		value += (len_reward+val_reward) * child->visits;
	}
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

void Node::calcUCT(double & uct_opp, double & exploration_bonus, int total_child_potential){
	uct_opp = value;
	exploration_bonus = tree->C * sqrt(2*log(tree->T+1) / (visits+1.0));

	exploration_bonus += (tree->beta * (potential)) / (1.0 + visits);
	if (total_child_potential > 0){
		double p_sa = double(potential) / double(total_child_potential);
		assert(parent != NULL);
		exploration_bonus = tree->beta1 * p_sa * sqrt(parent->visits) / (1.0 + visits);
		// double temp = (tree->beta1 * potential) / ((1.0 + visits) * total_child_potential);
		// exploration_bonus += temp * (sqrt(visits) / (1 + visits));
	}
	return;
}

double Node::calcExplornBonus(int total_child_potential){
	double uct_opp, expbon;
	calcUCT(uct_opp, expbon, total_child_potential);
	return expbon;
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
	
	int total_child_potential = 0;
	if (tree->beta1 != 0.0){
		for (auto child: children){
			total_child_potential += child->potential;
		}
	}
	for (int i = 0; i < children.size(); i++){
		r = actions[i][0];
		c = actions[i][1];
		ret[r][c] = -children[i]->value + children[i]->calcExplornBonus(total_child_potential);
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
	int total_child_potential = 0;
	if (tree->beta1 != 0.0){
		for (auto child: children){
			total_child_potential += child->potential;
		}
	}
	for (int i = 0; i < children.size(); i++){
		r = actions[i][0];
		c = actions[i][1];
		ret[r][c] = children[i]->calcExplornBonus(total_child_potential);
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
		ret[r][c] = -children[i]->value;
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
	
	int total_child_potential = 0;
	if (tree->beta1 != 0.0){
		for (auto child: children){
			total_child_potential += child->potential;
		}
	}
	
	for (int i = 0; i < children.size(); i++){
		r = actions[i][0];
		c = actions[i][1];
		ret[r][c] = children[i]->value / children[i]->calcExplornBonus(total_child_potential);
	}
	return ret;
}
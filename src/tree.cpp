#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <assert.h>
#include <thread>
#include <math.h>
#include <numeric>
#include <mutex>

#include "tree.h"
#include "utils.h"
#include "shuffle_set.h"
#include "judge_and_potential.h"
#include "argdict.h"


using namespace std;

////////////////////////////////////////////////////////////////

template <typename TreeType>
BaseNode<TreeType>::BaseNode(): tree(nullptr), parent(nullptr), value(0.0), turn(0), parent_action(-1){}

template <typename TreeType>
BaseNode<TreeType>::BaseNode(int parent_action, BaseNode<TreeType>* parent): tree(parent->tree), parent(parent), parent_action(parent_action){
	assert(parent != nullptr);
	turn = (parent->turn%2)+1;
}

template <typename TreeType>
BaseNode<TreeType>::BaseNode(TreeType* tree, uint32_t turn): tree(tree), parent(nullptr), parent_action(-1), turn(turn){}

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
	cout << "calling BaseNode::select()" << endl;
	if (children.size() == 0) return -1;
	auto first_child = *children.begin();
	int best_action = first_child.first;
	double best_uct = first_child.second->getUCT();

	double current_uct;
	double best_cnt = 1.0;
	for (auto &child : children){
		current_uct = child.second->getUCT();
		if (current_uct > best_uct){
			best_uct = current_uct;
			best_action = child.first;
			best_cnt = 1.0;
		}
		else if(current_uct == best_uct){
			best_cnt++;
			double r = get_random();
			if (r <= 1.0/best_cnt){
				best_action = child.first;
			}
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

	this->calcUCT(val_opp, exploration_bonus, total_child_potential);
	if (this->gameover == this->turn){ // remember, this is child, calcing UCT for parent!
		return std::numeric_limits<double>::lowest() / 16;
		// return (0-1) + exploration_bonus;
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
		double p_sa = (potential*1.0)/(total_child_potential);
		assert(this->parent != NULL);
		exploration_bonus += this->tree->beta1 * p_sa * sqrt(this->parent->visits) / (1.0 + this->visits);
	}
	return;
}

template <typename TreeType>
int VanillaNode<TreeType>::select(){
	// throw NotImplementedException(); // use gameover to select
	if (this->children.size() == 0) return -1;
	auto first_child = *(this->children).begin();
	int best_action = -1;
	// double best_uct = first_child.second->getUCT();
	double best_uct = std::numeric_limits<double>::lowest();
	double gameovercnt = 0;

	double current_uct;
	double best_cnt = 1.0;
	for (auto &child : this->children){
		if (static_cast<VanillaNode<TreeType>*>(child.second)->gameover == this->turn){
			gameovercnt++;
			double r = get_random();
			if (r <= 1.0/gameovercnt){
				best_action = child.first;
				assert (best_action >= 0);
			}
		}
		if (gameovercnt > 0) continue;
		current_uct = child.second->getUCT();
		if (best_action == -1 || current_uct > best_uct){
			best_uct = current_uct;
			best_action = child.first;
			best_cnt = 1.0;
		}
		else if(current_uct == best_uct){
			best_cnt++;
			double r = get_random();
			if (r <= 1.0/best_cnt){
				best_action = child.first;
			}
		}
	}
	if (best_action < 0){
		cout << "this->children.size() = " << this->children.size() << endl;
		cout << "gameovercnt = " << gameovercnt << endl;
	}
	assert (best_action >= 0);
	return best_action;
}

template <typename TreeType>
int BaseNode<TreeType>::expand(vector<vector<int> > &board){
	if (!children.empty()) return 0;
	int nr = board.size(), nc = board[0].size();
	int count = 0;
	for (int i = 0; i < nr * nc; i++){
		if (board[i/nc][i%nc] != 0) continue;
		children[i] = new BaseNode<TreeType>(i, this);
		count++;
	}
	return count;
}

template <typename TreeType>
void VanillaNode<TreeType>::childGameOver(int newgameover){
	// this->gameover = newgameover;
	// if (this->gameover != 0){
	// 	this->value = this->gameover == this->turn ? 1 : -1;
	// }

	if (newgameover == this->turn){
		this->gameover = newgameover;
		this->value = 1;
		if (this->parent != nullptr){
			static_cast<VanillaNode<TreeType>*>(this->parent)->childGameOver(newgameover);
		}
		return;
	}
	this->gameoverCount++;
	if (this->gameoverCount < this->children.size()) return;

	this->gameover = newgameover;
	this->value = -1;
	if (this->parent != nullptr)
		static_cast<VanillaNode<TreeType>*>(this->parent)->childGameOver(newgameover);
}

template <typename TreeType>
void VanillaNode<TreeType>::offsetDepth(int offset){
	depth += offset;
	for (auto action: this->children){
		static_cast<VanillaNode<VanillaTree>*>(action.second)->offsetDepth(offset);
	}
}

template <typename TreeType>
VanillaNode<TreeType>::VanillaNode(int parent_action, VanillaNode<TreeType>* parent, vector<vector<int> > &board): BaseNode<TreeType>(parent_action, parent){
	this->depth = parent->depth+1;
	// cout << "here 5124" << endl;
	this->potential=this->tree->potential_fn(board, parent_action, this->gameover, this->tree->linesize);
	// cout << "here 214" << endl;
	if (this->gameover != 0){
		if (this->gameover == this->turn) this->value = 1;
		else this->value = -1;
	}
}

template <typename TreeType>
VanillaNode<TreeType>::VanillaNode(TreeType* tree, uint32_t turn): BaseNode<TreeType>(tree, turn){
	this->depth=1;
	this->potential = 0;
	this->gameover = 0;
}

template <typename TreeType>
int VanillaNode<TreeType>::expand(vector<vector<int> > &board){
	if (!this->children.empty()) return 0;
	int nr = board.size(), nc = board[0].size();
	int count = 0;
	VanillaNode<TreeType>* new_node;
	this->total_child_potential = 0;
	for (int i = 0; i < nr * nc; i++){
		// cout << "i = " << i << endl;
		if (board[i/nc][i%nc] != 0) continue;
		// cout << "here 23423432" << endl;
		board[i/nc][i%nc] = this->turn;
		new_node = new VanillaNode<TreeType>(i, this, board);
		board[i/nc][i%nc] = 0;
		// cout << "here" << endl;
		this->children[i] = new_node;
		this->total_child_potential += new_node->getPotential();
		count++;
		// if (new_node->gameover == this->turn) {
		if (new_node->gameover != 0){
			this->childGameOver(this->turn);
			// this->killAllExcept(i);
			// break;
		}
	}
	return count;
}

BaseTree::BaseTree(): C(sqrt(2)){
	cerr << "WARNING! Initialising C to sqrt(2)\n";
}

VanillaTree::VanillaTree():num_rollout_workers(4), num_rollouts(20), max_depth(5), timeout(10000), linesize(5), potential_fn(calc_potential){
	cerr << "WARNING! Don't use the VanillaTree::VanillaTree() if you are not very sure of what are you doing.\n";
	throw NotImplementedException();
}

VanillaTree::VanillaTree(int linesize, int nr, int nc, int turn, int num_rollouts, double C, int max_depth, int timeout, int num_workers, double gamma, double alpha, double beta, double beta1, int potfn_v): BaseTree(C), num_rollouts(num_rollouts), max_depth(max_depth), timeout(timeout), num_rollout_workers(num_rollout_workers), gamma(gamma), alpha(alpha), beta(beta), beta1(beta1), linesize(linesize){
	switch (potfn_v){
	case 1:
		potential_fn = calc_potential;
		break;
	case 2:
		potential_fn = calc_potential_disruptive;
		break;
	case 3:
		potential_fn = calc_potential2;
		break;
	case 4:
		potential_fn = calc_potential_disruptive2;
		break;
	case 5:
		potential_fn = uniform_potential;
		break;
	default:
		potential_fn = calc_potential;
		break;
	}
	this->board = vector<vector<int> >(nr, vector<int>(nc, 0));
	root = new VanillaNode<VanillaTree>(this, turn);
	this->root->expand(this->board);
}

VanillaTree::VanillaTree(argdict VanillaTreeArgDict): BaseTree(VanillaTreeArgDict.get_dbl_arg("C")), num_rollouts(VanillaTreeArgDict.get_int_arg("num_rollouts")), num_rollout_workers(VanillaTreeArgDict.get_int_arg("num_rollout_workers")), max_depth(VanillaTreeArgDict.get_int_arg("max_depth")), timeout(VanillaTreeArgDict.get_int_arg("timeout")), linesize(VanillaTreeArgDict.get_int_arg("linesize")){
	int potfn_v = VanillaTreeArgDict.has_int("potfn_v") ? VanillaTreeArgDict.get_int_arg("potfn_v") : 1;

	switch (potfn_v){
	case 1:
		potential_fn = calc_potential;
		break;
	case 2:
		potential_fn = calc_potential_disruptive;
		break;
	case 3:
		potential_fn = calc_potential2;
		break;
	case 4:
		potential_fn = calc_potential_disruptive2;
		break;
	case 5:
		potential_fn = uniform_potential;
		break;
	default:
		potential_fn = calc_potential;
		break;
	}

	int nc = VanillaTreeArgDict.get_int_arg("nc");
	int nr = VanillaTreeArgDict.get_int_arg("nr");
	int turn = VanillaTreeArgDict.get_int_arg("turn");

	this->board = vector<vector<int> > (nr, vector<int>(nc, 0));
	this->root = new VanillaNode<VanillaTree>(this, turn);
	this->root->expand(this->board);

	if (VanillaTreeArgDict.has_dbl("gamma"))
		this->gamma = VanillaTreeArgDict.get_dbl_arg("gamma");
	if (VanillaTreeArgDict.has_dbl("alpha"))
		this->alpha = VanillaTreeArgDict.get_dbl_arg("alpha");
	if (VanillaTreeArgDict.has_dbl("beta"))
		this->beta = VanillaTreeArgDict.get_dbl_arg("beta");
	if (VanillaTreeArgDict.has_dbl("beta1"))
		this->beta1 = VanillaTreeArgDict.get_dbl_arg("beta1");
}


VanillaTree::~VanillaTree(){
	delete root;
}

void VanillaTree::playout(){
	// WARNING! Call only when root can have children!!!
	/*
	TODO: consider what happens when you encounter a node with game over
	Also consider the Multithreaded scenario, where, you might delete a subtree and some worker is in that subtree at that time.
	*/
	VanillaNode<VanillaTree>* current_node = root;
	assert (root != nullptr);
	current_node->expand(this->board);
	current_node->inc_visit(); // TODO use lock for MT

	int current_action;
	unsigned int depth = 1;
	vector<vector<int> > board_temp = this->board;
	int nc = this->board[0].size();

	while(!current_node->isLeaf()){
		current_action = current_node->select(); // TODO use lock for MT
		board_temp[current_action/nc][current_action%nc] = current_node->turn;
		current_node = static_cast<VanillaNode<VanillaTree>*>( current_node->children[current_action]);
		current_node->inc_visit(); // TODO use lock for MT

		depth++;
		if (current_node->gameover != 0) break;
		if (depth < max_depth && current_node->isLeaf()) current_node->expand(board_temp);
	}
	double leaf_value;

	if (current_node->isLeaf() && current_node->gameover == 0){
		leaf_value = getRolloutValue(current_node, board_temp);
		current_node->set_value(leaf_value); // Notice how previous rollouts aren't being utilised. This is laziness :)
	}

	current_node = static_cast<VanillaNode<VanillaTree>*>(current_node->parent);
	double temp_value, len_reward, val_reward;

	while (current_node != nullptr){
		if (current_node->visits == 0 || current_node->gameover != 0){
			current_node = static_cast<VanillaNode<VanillaTree>*>(current_node->parent);
			continue;
		}
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
	// WARNING! Call iff the match hasn't tied
	uint64_t num_selects = 0;
	auto t_start = chrono::high_resolution_clock::now();
	auto t_end = chrono::high_resolution_clock::now();
	auto exec_time = chrono::duration_cast<chrono::duration<double> >(t_end - t_start) .count();
	// cout << "before playout!" << endl;
	while(exec_time < this->timeout){
		t_end = chrono::high_resolution_clock::now();
		exec_time = chrono::duration_cast<chrono::duration<double> >(t_end - t_start) .count();
		for (int i = 0; i < 100; i++){
			playout();
			num_selects++;
		}
	}
	double best_val = std::numeric_limits<double>::lowest();
	int best_action = -1;
	double best_cnt = 1.0;
	for (auto [action, child]: root->children){
		if (best_action == -1 || best_val < child->visits){
			best_val = child->visits;
			best_action = action;
			best_cnt = 1.0;
		}
		else if(best_val == child->visits){
			best_cnt++;
			double r = get_random();
			if (r <= 1.0/best_cnt){
				best_action = action;
			}
		}
		// if (best_action == -1 || best_val <= -child->value){
		// 	best_val = -child->value;
		// 	best_action = action;
		// }
	}
	int nr = board.size(), nc = board[0].size();

	if (this->verbose > 1){
		cout << "printing GAMEOVER mat\n";
		print_mat(get_gameover_mat(this));

		cout << "printing UCT mat\n";
		print_mat(get_UCT_mat(this));

		cout << "printing EB mat\n";
		print_mat(get_EB_mat(this));


		cout << "printing POTENTIAL mat\n";
		print_mat(get_potential_mat(this));

		cout << "printing VALUE mat\n";
		print_mat(get_Val_mat(this));

		cout << "printing VISIT mat\n";
		print_mat(get_visit_mat(this));
		
		cout << endl;
	}
	cout << "num_selects = " << num_selects << endl;

	board[best_action/nc][best_action%nc] = root->turn;
	root->killAllExcept(best_action);
	cout << "after killAllExcept!" << endl;
	VanillaNode<VanillaTree>* newroot = static_cast<VanillaNode<VanillaTree>*>(root->children[best_action]);
	root->children.clear();
	delete root;
	root = newroot;
	root->parent = nullptr;
	// root->parent_action = -1;
	root->offsetDepth(-1);

	return best_action;
}

void VanillaTree::opponentMove(int pos){
	int nc = board[0].size();
	board[pos/nc][pos%nc] = this->root->turn;
	if (root->children.find(pos) == root->children.end()){
		VanillaNode<VanillaTree>* oldroot = root;
		root = new VanillaNode<VanillaTree>(pos, oldroot, board);
		delete oldroot;
		root->parent = nullptr;
		root->offsetDepth(-1);
	}
	else{
		root->killAllExcept(pos);
		VanillaNode<VanillaTree>* newroot = static_cast<VanillaNode<VanillaTree>*>(root->children[pos]);
		root->children.clear();
		delete root;
		root = newroot;
		root->parent = nullptr;
		root->offsetDepth(-1);
	}
}

void rollout_worker(const vector<vector<int> > &board_, int linesize, int last_move_at, int budget_steps, const int max_rollout_len, function<int(vector<vector<int> >&, int, int)>judgement, int *total_rollouts_performed, int *total_rollouts_completed, int* total_reward_plr1){
	vector<vector<int> > board;
	int num_rollouts_performed = 0, num_rollouts_completed = 0, num_plr1_wins = 0, num_plr1_loss = 0, num_draws = 0;
	int game_end_result;
	int last_plr, next_plr;
		int outcome;

	const int nr = board_.size();
	const int nc = board_[0].size();
	int last_action, next_action;
	const vector<int> free_actions_vec = free_locns(board_);
	shuffle_set<int> free_actions;
	
	while(budget_steps > 0){
		board = board_;
		last_action = last_move_at;
		free_actions.init(free_actions_vec.begin(), free_actions_vec.end());
		for (int steps_in_a_rollout = 0; steps_in_a_rollout < max_rollout_len; steps_in_a_rollout++){
			last_plr = board[last_action/nc][last_action%nc];
			next_plr = (last_plr%2) + 1;
			next_action = free_actions.take_random_elem();

			board[next_action/nc][next_action%nc] = next_plr;
			last_plr = next_plr;
			budget_steps--;
			last_action = next_action;

			outcome = judgement(board, next_action, linesize);
			if (outcome != 0 || free_actions.empty()){
				num_rollouts_completed++;
				num_plr1_wins += int(outcome == 1);
				num_plr1_loss += int(outcome == 2);
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

double VanillaTree::getRolloutValue(VanillaNode<VanillaTree>* leaf, vector<vector<int> > &board){
	/*
	Each worker needs to spit out the following things:
	- Total number of rollouts performed
	- Total number of rollouts completed
	- Sum of rewards of all rollouts wrt player 1.
	*/
	int max_steps_to_game_end = count_elems(board, 0);
	if (max_steps_to_game_end == 0){
		cerr << "VanillaTree::getRolloutValue: called on tied match!";
		return 0.0;
	}
	int total_budget = num_rollouts * max_steps_to_game_end;
	int total_budget_per_worker = total_budget / num_rollout_workers;

	// int total_rollouts_performed[num_rollout_workers], total_rollouts_completed[num_rollout_workers], total_reward_plr1[num_rollout_workers];
	int *total_rollouts_performed = new int[num_rollout_workers];
	int *total_rollouts_completed = new int[num_rollout_workers];
	int *total_reward_plr1 = new int[num_rollout_workers];

	thread *workers = new thread[num_rollout_workers];
	for(int i = 0; i < num_rollout_workers; i++){
		workers[i] = thread(rollout_worker, std::ref(board), linesize, leaf->parent_action, total_budget_per_worker, max_rollout_len, this->gameJudge, total_rollouts_performed+i, total_rollouts_completed+i, total_reward_plr1+i);
	}
	for(int i = 0; i < num_rollout_workers; i++){
		workers[i].join();
	}
	delete[] workers;

	int total_completed_rollouts = std::accumulate(total_rollouts_completed, total_rollouts_completed+num_rollout_workers, 0);
	delete[] total_rollouts_completed;
	delete[] total_rollouts_performed;

	if (total_completed_rollouts == 0){
		delete[] total_reward_plr1;
		return 0.0;
	}

	int total_reward = std::accumulate(total_reward_plr1, total_reward_plr1+num_rollout_workers, 0);
	delete[] total_reward_plr1;

	int last_action = leaf->parent_action;
	int nr = board.size(), nc = board[0].size();
	int current_plr = 1+((board[last_action/nc][last_action%nc])%2);
	if (current_plr == 2) total_reward *= -1;
	return (1.0*total_reward) / (1.0 * total_completed_rollouts);
}


InfiDepthTree::InfiDepthTree(): VanillaTree(), childless_visit_limit(1){}

InfiDepthTree::InfiDepthTree(int linesize, int nr, int nc, int turn, int num_rollouts, double C, int childless_visit_limit, int timeout, int num_workers, double gamma, double alpha, double beta, double beta1, int potfn_v): VanillaTree(linesize, nr, nc, turn, num_rollouts, C, 2, timeout, num_rollout_workers, gamma, alpha, beta, beta1, potfn_v), childless_visit_limit(childless_visit_limit){
}

InfiDepthTree::InfiDepthTree(argdict InfiDepthTreeArgDict): VanillaTree(InfiDepthTreeArgDict), childless_visit_limit(InfiDepthTreeArgDict.get_int_arg("childless_visit_limit")){}

InfiDepthTree::~InfiDepthTree(){}

void InfiDepthTree::playout(){
	VanillaNode<VanillaTree>* current_node = root;
	assert (root != nullptr);
	current_node->expand(this->board);
	current_node->inc_visit(); // TODO use lock for MT

	int current_action;
	vector<vector<int> > board_temp = this->board;
	int nc = this->board[0].size();

	while(!current_node->isLeaf()){
		current_action = current_node->select(); // TODO use lock for MT
		board_temp[current_action/nc][current_action%nc] = current_node->turn;
		current_node = static_cast<VanillaNode<VanillaTree>*>( current_node->children[current_action]);
		current_node->inc_visit(); // TODO use lock for MT

		if (current_node->gameover != 0) break;
	}
	double leaf_value;

	if (current_node->isLeaf() && current_node->gameover == 0){
		if (current_node->get_visits() > this->childless_visit_limit)
			current_node->expand(board_temp);
		if (current_node->gameover == 0){
			leaf_value = getRolloutValue(current_node, board_temp);
			current_node->set_value(leaf_value); // Notice how previous rollouts aren't being utilised. This is laziness :)
		}
	}

	current_node = static_cast<VanillaNode<VanillaTree>*>(current_node->parent);
	double temp_value, len_reward, val_reward;

	while (current_node != nullptr){
		if (current_node->visits == 0 || current_node->gameover != 0){
			current_node = static_cast<VanillaNode<VanillaTree>*>(current_node->parent);
			continue;
		}
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


// template <typename TreeType>
// GameOverPropagatorNode<TreeType>::GameOverPropagatorNode(){}

// template <typename TreeType>
// GameOverPropagatorNode<TreeType>::GameOverPropagatorNode(int parent_action, GameOverPropagatorNode<TreeType>* parent):VanillaNode<TreeType>(parent_action, parent){
// }

// template <typename TreeType>
// GameOverPropagatorNode<TreeType>::GameOverPropagatorNode(TreeType* tree, vector<vector<int> > board, uint32_t turn): VanillaNode<TreeType>(tree, board, turn){
// }

// template <typename TreeType>
// GameOverPropagatorNode<TreeType>::~GameOverPropagatorNode(){
// }

// template <typename TreeType>
// void GameOverPropagatorNode<TreeType>::childGameOver(int newgameover){
// 	if (newgameover == this->turn){
// 		this->gameover = newgameover;
// 		if (this->parent != nullptr){
// 			this->parent->childGameOver(newgameover);
// 		}
// 		return;
// 	}
// 	this->gameoverCount++;
// 	if (this->gameoverCount < this->children.size()) return;

// 	this->gameover = newgameover;
// 	if (this->parent != nullptr)
// 		this->parent->childGameOver(newgameover);
// }